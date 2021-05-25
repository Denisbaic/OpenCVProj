#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"
#include "WebcamReader.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClickModeChange,bool,IsEnable);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGetInput, FVector2D, Input);

#define SET_VALIDATE_PROPERTIES(PropertyName)				    \
	float PropertyName##RacingTime = 0.f;						\
	float Current##PropertyName##Time = 0.f;					

UCLASS()
class OPENCVPROJ_API AWebcamReader : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AWebcamReader();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ChangeFrameSize();
	
	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// The device ID opened by the Video Stream
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Webcam)
		int32 CameraID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = FaceSigns)
		bool IsNeedToReactOnGestureChanges;
	
	
	// If the webcam images should be resized every frame
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = SizeConfig)
		//bool ShouldResize;
	
	// The targeted resize width and height (width, height)
	//UPROPERTY(BlueprintReadWrite, Category = SizeConfig)
		//FIntPoint FinalVideoSize;
	
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
		FVector2D MouseFieldSize = {200.f,100.f};
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = MouseFieldConfig)
		bool bNeedFlipVerticalAxis = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = MouseFieldConfig)
		bool bNeedFlipHorizontalAxis = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = MouseFieldConfig)
		bool bIsClickMode = false;
	
	UFUNCTION(BlueprintImplementableEvent)
		void Click();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Validate)
		float TimeRateValidate;
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
		void ValidateFunction(float DeltaTime);

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Validate) 
		float ValidateMouthCloseTime;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = FaceSigns)
		bool IsMouthClose = true;
		
	SET_VALIDATE_PROPERTIES(Mouth)
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Validate)
		float ValidateLeftEyeOpenTime;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = FaceSigns)
		bool IsLeftEyeOpen = true;
	
	SET_VALIDATE_PROPERTIES(LeftEye)

	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Validate)
		float ValidateRightEyeOpenTime;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = FaceSigns)
		bool IsRightEyeOpen = true;
	
	SET_VALIDATE_PROPERTIES(RightEye)

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Validate)
		float ValidateSquintTime;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = FaceSigns)
		bool IsSquint = false;

	SET_VALIDATE_PROPERTIES(Squint)

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Validate)
		float ValidateRaisedEyebrowsTime;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = FaceSigns)
		bool IsRaisedEyebrows = false;

	SET_VALIDATE_PROPERTIES(RaisedEyebrows)
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Validate)
	float MAR = 0.6f;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Validate)
	float EAR = 0.2f;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Validate)
	float BAR = 1.57f;
	/*
	UPROPERTY(BlueprintAssignable)
	FOnSquint OnSquint;
	UPROPERTY(BlueprintAssignable)
	FOnLeftEyeClose OnLeftEyeClose;
	UPROPERTY(BlueprintAssignable)
	FOnRightEyeClose OnRightEyeClose;
	UPROPERTY(BlueprintAssignable)
	FOnLeftEyeOpen OnLeftEyeOpen;
	UPROPERTY(BlueprintAssignable)
	FOnRightEyeOpen OnRightEyeOpen;
	*/


	UPROPERTY(BlueprintAssignable)
	FOnClickModeChange OnClickModeChange;

	UPROPERTY(BlueprintAssignable)
	FOnGetInput OnGetInput;
	
	UFUNCTION(BlueprintPure, Category = Validate)
	bool IsScrollModeEnabled() const;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Input)
	bool bIsInCursorMode;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Render)
	float RenderTimeInterval;
protected:
	float _DeltaTime;
	FTimerHandle RenderTimer_TimerHandler;
	FTimerHandle ValidateTimer_TimerHandler;
	
	// Use this function to update the texture rects you want to change:
	// NOTE: There is a method called UpdateTextureRegions in UTexture2D but it is compiled WITH_EDITOR and is not marked as ENGINE_API so it cannot be linked
	// from plugins.
	// FROM: https://wiki.unrealengine.com/Dynamic_Textures
	void UpdateTextureRegions(UTexture2D* Texture, int32 MipIndex, uint32 NumRegions, FUpdateTextureRegion2D* Regions, uint32 SrcPitch, uint32 SrcBpp, uint8* SrcData, bool bFreeData);

	// Pointer to update texture region 2D struct
	FUpdateTextureRegion2D* VideoUpdateTextureRegion;
};
