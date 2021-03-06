// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Kismet/BlueprintFunctionLibrary.h"

#include "Cursor_BPL.generated.h"


class FGameAnalogCursor;
UCLASS()
class FLDPLUGIN_API UCursor_BPL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Game")
		static void EnableVirtualCursor(class APlayerController* PC);

	UFUNCTION(BlueprintCallable, Category = "Game")
		static void DisableVirtualCursor(class APlayerController* PC);

	static TSharedPtr<FGameAnalogCursor> GetCursorPtr();
	
	UFUNCTION(BlueprintPure, Category = "Game")
		static bool IsCursorOverInteractableWidget();

	UFUNCTION(BlueprintCallable, Category = "Game")
		static void LeftClick();

	UFUNCTION(BlueprintCallable, Category = "Game")
		static void RightClick();

	UFUNCTION(BlueprintCallable, Category = "Game")
		static void WheelInput(float Input, float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "Game")
		static void MoveMouse(FVector2D Input, float DeltaTime);

};