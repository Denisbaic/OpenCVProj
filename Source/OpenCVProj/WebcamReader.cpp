// Fill out your copyright notice in the Description page of Project Settings.


#include "WebcamReader.h"

#include "CamMouseCursorSettings.h"
#include "Cursor_BPL.h"
#include "FLD_BPL.h"
#include "GameAnalogCursor.h"
#include "Kismet/GameplayStatics.h"
// Fill out your copyright notice in the Description page of Project Settings.


// Sets default values
AWebcamReader::AWebcamReader()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize OpenCV and webcam properties
	CameraID = 0;

	IsNeedToReactOnGestureChanges = true;
	IsNeedToProcessDefaultEyebrowsRaised = true;
	IsNeedToProcessDefaultMouth = true;
	IsNeedToProcessDefaultSquint = true;
	//VideoSize = FVector2D(0, 0);
	//ShouldResize = false;
	//ResizeDeminsions = FVector2D(320, 240);
}

// Called when the game starts or when spawned
void AWebcamReader::BeginPlay()
{
	Super::BeginPlay();
		
	int32 i = UFLD_BPL::InitOpenCV(CameraID, "Data","deploy.prototxt", "res10_300x300_ssd_iter_140000_fp16.caffemodel", "lbfmodel.yaml",
							200, 100);

	UE_LOG(LogTemp, Warning, TEXT("%d"), i);
	//UFLD_BPL::LoadDataSet("Data");
	
	// Open the stream
	_DeltaTime = 0.f;
	if (UFLD_BPL::IsCamOpened())
	{
		// Initialize stream
		UFLD_BPL::CalculateFacialLandmarks();
		UFLD_BPL::GetFrameSize(VideoSize.X, VideoSize.Y);
		//size = cv::Size(ResizeDeminsions.X, ResizeDeminsions.Y);
		VideoTexture = UTexture2D::CreateTransient(VideoSize.X, VideoSize.Y);
		VideoTexture->UpdateResource();
		VideoUpdateTextureRegion = new FUpdateTextureRegion2D(0, 0, 0, 0, VideoSize.X, VideoSize.Y);

		// Initialize data array
		Data.Init(FColor(0, 0, 0, 255), VideoSize.X * VideoSize.Y);

		GetWorldTimerManager().SetTimer(RenderTimer_TimerHandler, this, &AWebcamReader::UpdateTexture, RenderTimeInterval, true, 0.0f);
		FTimerDelegate TimerDel;
		TimerDel.BindUFunction(this, FName("ValidateFunction"), TimeRateValidate);
		GetWorldTimerManager().SetTimer(ValidateTimer_TimerHandler, TimerDel, TimeRateValidate,true);

		// Do first frame
		DoProcessing();
	}
	//UCursor_BPL::OnStopInDeadZone.AddUObject(this, &AWebcamReader::OnStopInDeadZone);
}

void AWebcamReader::ChangeFrameSize()
{
	UFLD_BPL::GetFrameSize(VideoSize.X, VideoSize.Y);
	//size = cv::Size(ResizeDeminsions.X, ResizeDeminsions.Y);
	VideoTexture = UTexture2D::CreateTransient(VideoSize.X, VideoSize.Y);
	VideoTexture->UpdateResource();
	VideoUpdateTextureRegion = new FUpdateTextureRegion2D(0, 0, 0, 0, VideoSize.X, VideoSize.Y);

	// Initialize data array
	Data.Init(FColor(0, 0, 0, 255), VideoSize.X * VideoSize.Y);
}

// Called every frame
void AWebcamReader::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	_DeltaTime = DeltaTime;
	//if(UFLD_BPL::CalculateFacialLandmarks())
	//{
		//float temp = GetWorld()->GetDeltaSeconds();
		//ValidateFunction(DeltaTime);
	if(IsNeedToReactOnGestureChanges)
		DoProcessing();
	//}
}

void AWebcamReader::DoProcessing_Implementation()
{
	/*if (ShouldResize && UFLD_BPL::IsCamOpened())
	{
		UFLD_BPL::ResizeFrame(FinalVideoSize.X, FinalVideoSize.Y);
	}*/

	if(IsNeedToProcessDefaultMouth)
	{
		static bool LastMouthState = false;

		if (!LastMouthState && IsMouseFieldSelected && !IsMouthClose)
		{
			IsMouseFieldSelected = false;

			UFLD_BPL::SetIsSelectedNosePositionForMouseControl(false);
		}
		else if (!LastMouthState && !IsMouseFieldSelected && !IsMouthClose)
		{
			IsMouseFieldSelected = true;

			TArray<FVector2D> FacialLandmarks;
			UFLD_BPL::GetFacialLandmarks(0, FacialLandmarks);

			float const MouseFieldX = FacialLandmarks[30].X - MouseFieldSize.X / 2.f;
			float const MouseFieldY = FacialLandmarks[30].Y - MouseFieldSize.Y / 2.f;

			UFLD_BPL::SetMouseField(MouseFieldX, MouseFieldY, MouseFieldSize.X, MouseFieldSize.Y);
			UFLD_BPL::SetIsSelectedNosePositionForMouseControl(true);
		}

		LastMouthState = !IsMouthClose;
	}
	
	if(IsNeedToProcessDefaultEyebrowsRaised)
	{
		static bool EyebrowsLastState = false;

		if (IsMouseFieldSelected && !EyebrowsLastState && IsRaisedEyebrows)
		{
			bIsClickMode = !bIsClickMode;
			OnClickModeChange.Broadcast(bIsClickMode);
		}
		EyebrowsLastState = IsRaisedEyebrows;
	}
	
	
	if (IsMouseFieldSelected)
	{
		FVector2D MouseInput;
		UFLD_BPL::GetMouseDirection(0, MouseInput);

		int32 x, y, width, height;
		UFLD_BPL::GetMouseField(x, y, width, height);

		MouseInput.X /= width/2.f;
		MouseInput.Y /= height/2.f;
		MouseInput.X = bNeedFlipHorizontalAxis ? MouseInput.X : -MouseInput.X;
		MouseInput.Y = bNeedFlipVerticalAxis ? MouseInput.Y : -MouseInput.Y;

		if(IsNeedToProcessDefaultSquint && IsScrollModeEnabled())
		{
			
			UCursor_BPL::MoveMouse({ 0.f,0.f }, _DeltaTime);
			UCursor_BPL::WheelInput(MouseInput.Y, _DeltaTime);
		}
		else if(bIsClickMode)
		{
			UCursor_BPL::MoveMouse({ 0.f,0.f }, _DeltaTime);
		}
		else
		{
			const UCamMouseCursorSettings* Settings = GetDefault<UCamMouseCursorSettings>();
			float const  DeadZoneSize = Settings->GetAnalogCursorDeadZone();
			if(bIsFastClicksMode && MouseInput.Size()<DeadZoneSize)
			{
				OnStopInDeadZone(_DeltaTime);
			}
			else
			{
				OnMove(_DeltaTime);
			}
			UCursor_BPL::MoveMouse(MouseInput, _DeltaTime);
			OnGetInput.Broadcast(MouseInput);
		}
	}
	else
	{
		UCursor_BPL::MoveMouse({ 0.f,0.f }, _DeltaTime);
	}
	
}

void AWebcamReader::UpdateTexture()
{
	if (UFLD_BPL::IsCamOpened())
	{
		// Copy Mat data to Data array
		UFLD_BPL::GetFrame(Data);

		// Update texture 2D
		UpdateTextureRegions(VideoTexture, (int32)0, (uint32)1, VideoUpdateTextureRegion, (uint32)(4 * VideoSize.X), (uint32)4, (uint8*)Data.GetData(), false);
		OnNextVideoFrame();
	}
}

#define EYE_VALIDATE(EyeSide, IsLeftSide)																								\
if (Current##EyeSide##EyeTime > Validate##EyeSide##EyeOpenTime)																			\
{																																		\
	Current##EyeSide##EyeTime = 0.f;																									\
	Is##EyeSide##EyeOpen = EyeSide##EyeRacingTime > 0;																					\
	float current_ear;																													\
	bool const bCurrentEyeState = UFLD_BPL::IsEyeOpen(current_ear, 0, IsLeftSide, EAR);													\
	EyeSide##EyeRacingTime = bCurrentEyeState ? DeltaTime : - DeltaTime;																\
}																																		\
else																																	\
{																																		\
	float current_ear;																													\
	bool const bCurrentEyeState = UFLD_BPL::IsEyeOpen(current_ear, 0, IsLeftSide, EAR);													\
	/*UE_LOG(LogTemp, Warning, TEXT("%f"), current_ear);*/																				\
	EyeSide##EyeRacingTime = bCurrentEyeState ? EyeSide##EyeRacingTime + DeltaTime : EyeSide##EyeRacingTime - DeltaTime;				\
}																																		\
Current##EyeSide##EyeTime += DeltaTime;																									

bool LeftEyeLastState = true;
bool RightEyeLastState = true;
bool MouthCloseLastState = false;
bool SquintLastState = true;
bool RaisedEyebrowsLastState = false;

void AWebcamReader::ValidateFunction_Implementation(float DeltaTime)
{
	UFLD_BPL::CalculateFacialLandmarks();
	/*
	if (CurrentLeftEyeTime > ValidateLeftEyeOpenTime)
	{
		CurrentLeftEyeTime = 0.f;
		IsLeftEyeOpen = LeftEyeRacingTime > 0;
		LeftEyeRacingTime = 0.f;
	}
	else
	{
		float current_ear;
		bool const bCurrentEyeState = UFLD_BPL::IsEyeOpen(current_ear,0,true,EAR);
		UE_LOG(LogTemp, Warning, TEXT("%f"), current_ear);
		LeftEyeRacingTime = bCurrentEyeState ? LeftEyeRacingTime + DeltaTime : LeftEyeRacingTime - DeltaTime;
	}
	CurrentLeftEyeTime += DeltaTime;
	*/
	
	EYE_VALIDATE(Left, true);
	EYE_VALIDATE(Right, false);

	if (IsLeftEyeOpen != LeftEyeLastState)
		OnLeftEyeOpen.Broadcast(IsLeftEyeOpen);

	if (IsRightEyeOpen != RightEyeLastState)
		OnRightEyeOpen.Broadcast(IsRightEyeOpen);
	
	if(IsMouseFieldSelected && bIsClickMode && LeftEyeLastState && RightEyeLastState && !IsLeftEyeOpen && !IsRightEyeOpen)
	{
		Click();
	}
	LeftEyeLastState = IsLeftEyeOpen;
	RightEyeLastState = IsRightEyeOpen;
		
	if (CurrentMouthTime > ValidateMouthCloseTime)
	{
		CurrentMouthTime = 0.f;
		IsMouthClose = MouthRacingTime < 0;
		float CurrentMAR = 0.95f;
		bool const bCurrentMouthState = UFLD_BPL::IsMouthOpen(CurrentMAR,0, MAR);
		
		MouthRacingTime = bCurrentMouthState ? DeltaTime : - DeltaTime;
	}
	else
	{
		float CurrentMAR = 0.95f;
		bool const bCurrentMouthState = UFLD_BPL::IsMouthOpen(CurrentMAR,0, MAR);
		MouthRacingTime = bCurrentMouthState ? MouthRacingTime + DeltaTime : MouthRacingTime - DeltaTime;
	}
	CurrentMouthTime += DeltaTime;

	if (MouthCloseLastState != IsMouthClose)
		OnMouthStateChanged.Broadcast(!IsMouthClose);
	MouthCloseLastState = IsMouthClose;

	/*
	if (CurrentSquintTime > ValidateSquintTime)
	{
		CurrentSquintTime = 0.f;
		IsSquint = SquintRacingTime > 0;
		SquintRacingTime = !IsLeftEyeOpen && !IsRightEyeOpen ? DeltaTime : -DeltaTime;
	}
	else
	{
		SquintRacingTime = !IsLeftEyeOpen && !IsRightEyeOpen ? SquintRacingTime + DeltaTime : SquintRacingTime - DeltaTime;
	}
	
	CurrentSquintTime += DeltaTime;
	*/
	IsSquint = !IsLeftEyeOpen && !IsRightEyeOpen;
	if (SquintLastState != IsSquint)
		OnSquintStateChanged.Broadcast(IsSquint);
	SquintLastState = IsSquint;
	
	if (CurrentRaisedEyebrowsTime > ValidateRaisedEyebrowsTime)
	{
		CurrentRaisedEyebrowsTime = 0.f;
		IsRaisedEyebrows = RaisedEyebrowsRacingTime > 0;
		float CurrentBar = 0.95f;
		RaisedEyebrowsRacingTime = UFLD_BPL::IsEyebrowsRaised(CurrentBar, 0, BAR) ? DeltaTime : - DeltaTime;
	}
	else
	{
		float CurrentBar = 0.95f;
		RaisedEyebrowsRacingTime = UFLD_BPL::IsEyebrowsRaised(CurrentBar, 0, BAR) ? RaisedEyebrowsRacingTime + DeltaTime : RaisedEyebrowsRacingTime - DeltaTime;
		//UE_LOG(LogTemp, Warning, TEXT("%f"), CurrentBar);
	}
	CurrentRaisedEyebrowsTime += DeltaTime;

	if (IsRaisedEyebrows != RaisedEyebrowsLastState)
		OnEyebrowsStateChanged.Broadcast(IsRaisedEyebrows);
	RaisedEyebrowsLastState = IsRaisedEyebrows;
	
}

bool AWebcamReader::IsScrollModeEnabled() const
{
	return !bIsClickMode && IsSquint && IsMouseFieldSelected;
}

float deadzone_time = 0;
void AWebcamReader::OnStopInDeadZone(float DeltaTime)
{
	deadzone_time += DeltaTime;
	if(deadzone_time >1.f)
	{
		UCursor_BPL::LeftClick();
		deadzone_time = 0.f;
	}
	UE_LOG(LogTemp, Warning, TEXT("DEADZONE"));
}

void AWebcamReader::OnMove(float DeltaTime)
{
	deadzone_time = FMath::Clamp(deadzone_time-DeltaTime,0.f,1.f);
}

void AWebcamReader::UpdateTextureRegions(UTexture2D* Texture, int32 MipIndex, uint32 NumRegions, FUpdateTextureRegion2D* Regions, uint32 SrcPitch, uint32 SrcBpp, uint8* SrcData, bool bFreeData)
{
	if (Texture->Resource)
	{
		struct FUpdateTextureRegionsData
		{
			FTexture2DResource* Texture2DResource;
			int32 MipIndex;
			uint32 NumRegions;
			FUpdateTextureRegion2D* Regions;
			uint32 SrcPitch;
			uint32 SrcBpp;
			uint8* SrcData;
		};

		FUpdateTextureRegionsData* RegionData = new FUpdateTextureRegionsData;

		RegionData->Texture2DResource = (FTexture2DResource*)Texture->Resource;
		RegionData->MipIndex = MipIndex;
		RegionData->NumRegions = NumRegions;
		RegionData->Regions = Regions;
		RegionData->SrcPitch = SrcPitch;
		RegionData->SrcBpp = SrcBpp;
		RegionData->SrcData = SrcData;

		ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(
			UpdateTextureRegionsData,
			FUpdateTextureRegionsData*, RegionData, RegionData,
			bool, bFreeData, bFreeData,
			{
			for (uint32 RegionIndex = 0; RegionIndex < RegionData->NumRegions; ++RegionIndex)
			{
				int32 CurrentFirstMip = RegionData->Texture2DResource->GetCurrentFirstMip();
				if (RegionData->MipIndex >= CurrentFirstMip)
				{
					RHIUpdateTexture2D(
						RegionData->Texture2DResource->GetTexture2DRHI(),
						RegionData->MipIndex - CurrentFirstMip,
						RegionData->Regions[RegionIndex],
						RegionData->SrcPitch,
						RegionData->SrcData
						+ RegionData->Regions[RegionIndex].SrcY * RegionData->SrcPitch
						+ RegionData->Regions[RegionIndex].SrcX * RegionData->SrcBpp
						);
				}
			}
			if (bFreeData)
			{
				FMemory::Free(RegionData->Regions);
				FMemory::Free(RegionData->SrcData);
			}
			delete RegionData;
			});
	}
}

