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
	FGameAnalogCursor(class APlayerController* PC, float _Radius);

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
	FORCEINLINE FVector2D GetCurrentPosition() const
	{
		return CurrentPosition;
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

	/** Getter */
	FORCEINLINE FVector2D GetLastCursorDirection() const
	{
		return LastCursorDirection;
	}

	/** Getter */
	FORCEINLINE float GetRadius() const
	{
		return Radius;
	}

	FORCEINLINE void SetInputVector(FVector2D NewInput)
	{
		InputVector = NewInput;
	}

	void TriggerMouseLMB();
	void TriggerMouseRMB();
private:
	
	FVector2D InputVector;
	
	/** Helper Function. Takes in values from the analog stick, returns a vector that represents acceleration */
	FVector2D GetAnalogCursorAccelerationValue(const FVector2D& InAnalogValues, float DPIScale);

	/** Current velocity of the cursor */
	FVector2D Velocity;

	/** Current position of the cursor. Stored outside of ICursor's position to avoid float->int32 truncation */
	FVector2D CurrentPosition;

	/** Unit vector derived from Velocity */
	FVector2D LastCursorDirection;

	/** The name of the hovered widget */
	FName HoveredWidgetName;

	/** Is this thing even active right now? */
	bool bIsUsingAnalogCursor;

	/** The radius of the analog cursor */
	float Radius;

	/** The Player's Context */
	FLocalPlayerContext PlayerContext;
};
