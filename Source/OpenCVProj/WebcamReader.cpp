// Fill out your copyright notice in the Description page of Project Settings.


#include "WebcamReader.h"

#include "OpenCV_BFL.h"

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

	UOpenCV_BFL::importDllAndDllFunctions("MyPlug", "OpenCVDLL.dll");


	
	int32 i = UOpenCV_BFL::InitOpenCV(CameraID, "MyPlug","haarcascade_frontalface_alt2.xml", "lbfmodel.yaml",
							200, 100);
	UE_LOG(LogTemp, Warning, TEXT("%d"), i);
	// Open the stream
	
	if (UOpenCV_BFL::IsCamOpened())
	{
		// Initialize stream
		UOpenCV_BFL::CalculateFacialLandmarks();
		UOpenCV_BFL::GetFrameSize(VideoSize.X, VideoSize.Y);
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

	if(UOpenCV_BFL::CalculateFacialLandmarks())
	{
		DoProcessing();
	}	
	UpdateTexture();
	OnNextVideoFrame();
	
}

void AWebcamReader::DoProcessing_Implementation()
{
	if (ShouldResize && UOpenCV_BFL::IsCamOpened())
	{
		UOpenCV_BFL::ResizeFrame(FinalVideoSize.X, FinalVideoSize.Y);
	}
}

void AWebcamReader::UpdateTexture()
{
	if (UOpenCV_BFL::IsCamOpened())
	{
		// Copy Mat data to Data array
		UOpenCV_BFL::GetFrame(Data);

		// Update texture 2D
		UpdateTextureRegions(VideoTexture, (int32)0, (uint32)1, VideoUpdateTextureRegion, (uint32)(4 * VideoSize.X), (uint32)4, (uint8*)Data.GetData(), false);
	}
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

