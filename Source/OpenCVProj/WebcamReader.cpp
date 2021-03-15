// Fill out your copyright notice in the Description page of Project Settings.


#include "WebcamReader.h"

#include "FLD_BPL.h"
// Fill out your copyright notice in the Description page of Project Settings.


// Sets default values
AWebcamReader::AWebcamReader()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize OpenCV and webcam properties
	CameraID = 0;

	//VideoSize = FVector2D(0, 0);
	ShouldResize = false;
	//ResizeDeminsions = FVector2D(320, 240);
}

// Called when the game starts or when spawned
void AWebcamReader::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(ValidateTimer_TimerHandler, this, &AWebcamReader::ValidateFunction, TimeRateValidate, true);
	
	int32 i = UFLD_BPL::InitOpenCV(CameraID, "Data","deploy.prototxt", "res10_300x300_ssd_iter_140000_fp16.caffemodel", "lbfmodel.yaml",
							200, 100);
	//int32 i = UFLD_BPL::InitOpenCV(CameraID, "Data", "shape_predictor_68_face_landmarks.dat", "lbfmodel.yaml",
		//						200, 100);
	UE_LOG(LogTemp, Warning, TEXT("%d"), i);
	// Open the stream
	
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

		// Do first frame
		DoProcessing();
		UpdateTexture();
		OnNextVideoFrame();
	}
	
}

// Called every frame
void AWebcamReader::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if(UFLD_BPL::CalculateFacialLandmarks())
	{
		DoProcessing();
	}	
	UpdateTexture();
	OnNextVideoFrame();
	
}

void AWebcamReader::DoProcessing_Implementation()
{
	if (ShouldResize && UFLD_BPL::IsCamOpened())
	{
		UFLD_BPL::ResizeFrame(FinalVideoSize.X, FinalVideoSize.Y);
	}

	static bool LastMouthState = false;
	
	if(!LastMouthState && IsMouseFieldSelected && !IsMouthClose)
	{
		IsMouseFieldSelected = false;
		
		UFLD_BPL::SetIsSelectedNosePositionForMouseControl(false);
	}
	else if(!LastMouthState && !IsMouseFieldSelected && !IsMouthClose)
	{
		IsMouseFieldSelected = true;
		
		TArray<FVector2D> FacialLandmarks;
		UFLD_BPL::GetFacialLandmarks(0, FacialLandmarks);

		float const MouseFieldX = FacialLandmarks[30].X - MouseFieldSize.X / 2.f;
		float const MouseFieldY = FacialLandmarks[30].Y - MouseFieldSize.Y / 2.f;
		
		UFLD_BPL::SetMouseField(MouseFieldX, MouseFieldY, MouseFieldSize.X, MouseFieldSize.Y);
		UFLD_BPL::SetIsSelectedNosePositionForMouseControl(true);
	}

	LastMouthState = UFLD_BPL::IsMouthOpen(0);
	
}

void AWebcamReader::UpdateTexture()
{
	if (UFLD_BPL::IsCamOpened())
	{
		// Copy Mat data to Data array
		UFLD_BPL::GetFrame(Data);

		// Update texture 2D
		UpdateTextureRegions(VideoTexture, (int32)0, (uint32)1, VideoUpdateTextureRegion, (uint32)(4 * VideoSize.X), (uint32)4, (uint8*)Data.GetData(), false);
	}
}

void AWebcamReader::ValidateFunction_Implementation()
{
	UFLD_BPL::CalculateFacialLandmarks();
	

	
	if (CurrentTime > ValidateLeftEyeOpenTime)
	{
		CurrentTime = 0.f;
		IsLeftEyeOpen = LeftRacingTime > 0;
		LeftRacingTime = 0.f;
	}
	else
	{
		float current_ear = 0.f;
		bool const CurrentMouthState = UFLD_BPL::IsEyeOpen(current_ear,0,true,EAR);
		UE_LOG(LogTemp, Warning, TEXT("%f"), current_ear);
		LeftRacingTime = CurrentMouthState ? LeftRacingTime + TimeRateValidate : LeftRacingTime - TimeRateValidate;
	}
	CurrentTime += TimeRateValidate;
	/*
	if(CurrentTime> ValidateMouthCloseTime)
	{
		CurrentTime = 0.f;
		IsMouthClose = MouthRacingTime < 0;
		MouthRacingTime = 0.f;
	}
	else
	{
		bool const CurrentMouthState = UFLD_BPL::IsMouthOpen();
		MouthRacingTime = CurrentMouthState ? MouthRacingTime + TimeRateValidate : MouthRacingTime - TimeRateValidate;
	}
	*/
	
	
	/*
	bool CurrentMouthState = UFLD_BPL::IsMouthOpen();
	if(!CurrentMouthState == LastMouthCloseState)
	{
		CheckMouthCloseTime += TimeRateValidate;
	}
	else
	{
		CheckMouthCloseTime = 0.f;
	}
	
	if (CheckMouthCloseTime >= ValidateMouthCloseTime)
	{
		IsMouthClose = LastMouthCloseState;
		CheckMouthCloseTime = 0.f;
	}
	
	LastMouthCloseState = !CurrentMouthState;

	
	bool CurrentLeftEyeState = UFLD_BPL::IsEyeOpen(0);
	if (CurrentLeftEyeState == LastLeftEyeOpenState)
	{
		CheckLeftEyeOpenTime += TimeRateValidate;
	}
	else
	{
		CheckLeftEyeOpenTime = 0.f;
	}

	if (CheckLeftEyeOpenTime >= ValidateLeftEyeOpenTime)
	{
		IsLeftEyeOpen = LastLeftEyeOpenState;
		CheckLeftEyeOpenTime = 0.f;
	}

	LastLeftEyeOpenState = CurrentLeftEyeState;
	*/
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

