// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Framework/Application/AnalogCursor.h"



/**
 * 
 */
class FGameAnalogCursor : public FAnalogCursor
{
public:

	FLDPLUGIN_API static void EnableAnalogCursor(class APlayerController* PC, TSharedPtr<SWidget> WidgetToFocus);
	FLDPLUGIN_API static void DisableAnalogCursor(class APlayerController* PC);

	/** Ctor */
	FGameAnalogCursor(class APlayerController* PC);

	/** Dtor */
	virtual ~FGameAnalogCursor()
	{}

	/** Uses different logic than base cursor */
	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override;

	/** Getter */
	FORCEINLINE FName GetHoveredWidgetName() const
	{
		return HoveredWidgetName;
	}

	/** Getter */
	FORCEINLINE bool IsHovered() const
	{
		return HoveredWidgetName != NAME_None;
	}

	/** Getter */
	FORCEINLINE FVector2D GetVelocity() const
	{
		return Velocity;
	}

	/** Getter */
	FORCEINLINE bool GetIsUsingAnalogCursor() const
	{
		return bIsUsingAnalogCursor;
	}

	FORCEINLINE void SetInputVector(FVector2D NewInput)
	{
		InputVector = NewInput;
	}

	FORCEINLINE void SetVelocityOnInput(float NewVelocityOnInput)
	{
		VelocityOnInput = NewVelocityOnInput;
	}

	float Time = 0.f;
	
	void TriggerMouseLMB();
	void TriggerMouseRMB();
	void TriggerWheel(float InDeltaWheel, float DeltaTime);
	
private:
	
	FVector2D InputVector;
	
	/** Helper Function. Takes in values from the analog stick, returns a vector that represents acceleration */
	FVector2D GetAnalogCursorAccelerationValue(const FVector2D& InAnalogValues, float DPIScale, float DeltaTime);

	/** Current velocity of the cursor */
	FVector2D Velocity;

	/** The name of the hovered widget */
	FName HoveredWidgetName;

	/** Is this thing even active right now? */
	bool bIsUsingAnalogCursor;

	/** The Player's Context */
	FLocalPlayerContext PlayerContext;
	
	float VelocityOnInput;
};
