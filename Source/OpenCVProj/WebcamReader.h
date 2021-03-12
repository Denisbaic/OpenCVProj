// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"
#include "WebcamReader.generated.h"

#define SET_VALIDATE_PROPERTIES(PropertyName, DefaultValue)				\
	bool Last##PropertyName##State = DefaultValue;						\
	float Check##PropertyName##Time;									

UCLASS()
class OPENCVPROJ_API AWebcamReader : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AWebcamReader();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// The device ID opened by the Video Stream
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Webcam)
		int32 CameraID;

	// If the webcam images should be resized every frame
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = SizeConfig)
		bool ShouldResize;
	
	// The targeted resize width and height (width, height)
	UPROPERTY(BlueprintReadWrite, Category = SizeConfig)
		FIntPoint FinalVideoSize;
	

	// Blueprint Event called every time the video frame is updated
	UFUNCTION(BlueprintImplementableEvent, Category = Webcam)
		void OnNextVideoFrame();

	UFUNCTION(BlueprintNativeEvent, Category = Webcam)
		void DoProcessing();

	void UpdateTexture();
		
	// The videos width and height (width, height)
	UPROPERTY(BlueprintReadOnly, Category = Webcam)
		FIntPoint VideoSize;
	
	// The current video frame's corresponding texture
	UPROPERTY(BlueprintReadOnly, Category = Webcam)
		UTexture2D* VideoTexture;

	// The current data array
	UPROPERTY(BlueprintReadOnly, Category = Webcam)
		TArray<FColor> Data;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = MouseFieldConfig)
		bool IsMouseFieldSelected;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = MouseFieldConfig)
		FVector2D MouseFieldSize;



	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Validate)
		float TimeRateValidate;
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
		void ValidateFunction();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Validate) 
		float ValidateMouthCloseTime;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = FaceSigns)
		bool IsMouthClose = true;
	float MouthRacingTime;

	float CurrentTime;

	
	SET_VALIDATE_PROPERTIES(MouthClose, true)
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Validate) \
		float ValidateLeftEyeOpenTime;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = FaceSigns)\
		bool IsLeftEyeOpen = true;
	float LeftRacingTime;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Validate)
		float EAR=0.2f;
	
	SET_VALIDATE_PROPERTIES(LeftEyeOpen, true)
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Validate) \
		float ValidateRightEyeOpenTime;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = FaceSigns)\
		bool IsRightEyeOpen = true;	
	SET_VALIDATE_PROPERTIES(RightEyeOpen, true)
	
protected:

	
	FTimerHandle ValidateTimer_TimerHandler;
	
	// Use this function to update the texture rects you want to change:
	// NOTE: There is a method called UpdateTextureRegions in UTexture2D but it is compiled WITH_EDITOR and is not marked as ENGINE_API so it cannot be linked
	// from plugins.
	// FROM: https://wiki.unrealengine.com/Dynamic_Textures
	void UpdateTextureRegions(UTexture2D* Texture, int32 MipIndex, uint32 NumRegions, FUpdateTextureRegion2D* Regions, uint32 SrcPitch, uint32 SrcBpp, uint8* SrcData, bool bFreeData);

	// Pointer to update texture region 2D struct
	FUpdateTextureRegion2D* VideoUpdateTextureRegion;
};
