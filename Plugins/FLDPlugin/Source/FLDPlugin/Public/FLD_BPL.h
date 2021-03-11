// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FLD_BPL.generated.h"


UCLASS()
class FLDPLUGIN_API UFLD_BPL : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static bool importDllAndDllFunctions(FString folder, FString name);

    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static int32 InitOpenCV(int32 CamIndex, FString Folder, FString FaceDetectorFilePath, FString FaceMarkModelFilePath, int32 MouseWheelFieldWidth = 200, int32 MouseWheelFieldHeight = 100);
    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static bool IsEyeOpen(int32 FaceIndex = 0, bool CheckLeftEye = true, float EAR = 0.2f);
    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static bool IsMouthOpen(int32 FaceIndex = 0, float MAR = 0.6f);
    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static bool CalculateFacialLandmarks();
    UFUNCTION(BlueprintCallable, Category = "My DLL Library")
        static bool GetFrame(UPARAM(ref) TArray<FColor>& Frame);
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
