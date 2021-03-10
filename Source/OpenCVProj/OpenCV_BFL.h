// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "OpenCV_BFL.generated.h"

/**
 * 
 */
UCLASS()
class OPENCVPROJ_API UOpenCV_BFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static bool importDLL(FString folder, FString name);


    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static bool importMethodGetInvertedBool();

    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static bool importMethodGetIntPlusPlus();

    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static bool importMethodGetCircleArea();

    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static bool importMethodGetCharArray();

    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static bool importMethodGetVector4();



	
    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static bool importMethodInvertBool();

    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static bool importMethodGetBool();


    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static void InvertBoolFromDLL();

    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static bool GetBoolFromDLL();
	

    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static bool getInvertedBoolFromDll(bool boolState);

    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static int getIntPlusPlusFromDll(int lastInt);

    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static float getCircleAreaFromDll(float radius);

    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static FString getCharArrayFromDll(FString parameterText);

    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static FVector4 getVector4FromDll(FVector4 vector4);


    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
		static bool importMethodInitOpenCV();

    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static bool importMethodShowFrame();

    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static bool importMethodStopOpenCV();

	
    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static int32 startOpenCV(FString folder, FString nameCascade, FString nameModel);

    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static void showFrame();

    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static void stopOpenCV();


    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static bool importStartDLib();
	
    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static bool importMethodShowDLib();


	
    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static int32 startDLib(FString folder, FString predictorFilePath);

    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static void showFrameDLib();

    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static void stopDLib();

    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static bool importDllAndDllFunctions(FString folder, FString name);

    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static int32 InitOpenCV(int32 CamIndex, FString Folder, FString FaceDetectorFilePath, FString FaceMarkModelFilePath, int32 MouseWheelFieldWidth=200, int32 MouseWheelFieldHeight=100);
    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static bool IsEyeOpen(int32 FaceIndex = 0, bool CheckLeftEye = true, float EAR = 0.2f);
    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static bool IsMouthOpen(int32 FaceIndex=0,  float MAR = 0.6f);
    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static bool CalculateFacialLandmarks();
    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static bool GetFrame(UPARAM(ref) TArray<FColor> & Frame);
    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static bool _StopOpenCV();
    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static bool SetIsNeedToShowBBox(bool NewValue);
    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static bool GetIsNeedToShowBBox();
    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static bool SetIsSelectedNosePositionForMouseControl(bool NewValue);
    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static bool GetIsSelectedNosePositionForMouseControl();
    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static bool SetMouseField(int32 x, int32 y, int32 width, int32 height);
    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static bool GetMouseField(UPARAM(ref) int32& x, UPARAM(ref) int32& y, UPARAM(ref) int32& width, UPARAM(ref) int32& height);
    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static bool IsCamOpened();
    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static bool GetFrameSize(int32& width, int32& height);
    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static bool ResizeFrame(int32 width, int32 height);	
    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static bool GetFacialLandmarks(int32 face_index, TArray<FVector2D>& FacialLandmarks);
	
    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static void freeDLL();
};
