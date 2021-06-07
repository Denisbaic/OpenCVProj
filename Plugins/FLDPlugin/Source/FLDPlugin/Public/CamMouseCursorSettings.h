// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameAnalogCursor.h"
#include "Engine/DeveloperSettings.h"
#include "Kismet/GameplayStatics.h"

#include "CamMouseCursorSettings.generated.h"


UCLASS(config = Game, defaultconfig)
class FLDPLUGIN_API UCamMouseCursorSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	UCamMouseCursorSettings();

	FORCEINLINE const FRichCurve* GetAnalogCursorAccelerationCurve() const
	{
		return AnalogCursorAccelerationCurve.GetRichCurveConst();
	}

	FORCEINLINE float GetMaxAnalogCursorSpeed() const
	{
		return MaxAnalogCursorSpeed;
	}

	FORCEINLINE float GetMaxAnalogCursorSpeedWhenHovered() const
	{
		return MaxAnalogCursorSpeedWhenHovered;
	}

	FORCEINLINE float GetAnalogCursorDragCoefficient() const
	{
		return FMath::Min<float>(AnalogCursorDragCoefficientWhenHovered, AnalogCursorDragCoefficient);
	}

	FORCEINLINE float GetAnalogCursorDragCoefficientWhenHovered() const
	{
		return FMath::Max<float>(AnalogCursorDragCoefficientWhenHovered, AnalogCursorDragCoefficient);
	}

	FORCEINLINE float GetAnalogCursorAccelerationMultiplier() const
	{
		return AnalogCursorAccelerationMultiplier;
	}

	FORCEINLINE float GetAnalogCursorDeadZone() const
	{
		return AnalogCursorDeadZone;
	}


	FORCEINLINE bool GetAnalogCursorNoAcceleration() const
	{
		return bAnalogCursorNoAcceleration;
	}

	FORCEINLINE void SetAnalogCursor(TSharedPtr<FGameAnalogCursor> InAnalogCursor)
	{
		AnalogCursor = InAnalogCursor;
	}

	FORCEINLINE TSharedPtr<FGameAnalogCursor> GetAnalogCursor() const
	{
		return AnalogCursor.Pin();
	}

private:
	UPROPERTY(config, EditAnywhere, Category = "Analog Cursor", meta = (
		XAxisName = "Strength",
		YAxisName = "Acceleration"))
		FRuntimeFloatCurve AnalogCursorAccelerationCurve;

	/** The max speed of the Analog Cursor */
	UPROPERTY(config, EditAnywhere, Category = "Analog Cursor", meta = (ClampMin = "1.0"))
		float MaxAnalogCursorSpeed;

	/** The max speed of the Analog Cursor when hovering over a widget that is interactable */
	UPROPERTY(config, EditAnywhere, Category = "Analog Cursor", meta = (ClampMin = "1.0"))
		float MaxAnalogCursorSpeedWhenHovered;

	/** The coefficient of drag applied to the cursor */
	UPROPERTY(config, EditAnywhere, Category = "Analog Cursor", meta = (ClampMin = "0.0", ClampMax = "1.0"))
		float AnalogCursorDragCoefficient;

	/** The coefficient of drag applied to the cursor when hovering */
	UPROPERTY(config, EditAnywhere, Category = "Analog Cursor", meta = (ClampMin = "0.0", ClampMax = "1.0"))
		float AnalogCursorDragCoefficientWhenHovered;

	/** Deadzone value for input from the analog stick */
	UPROPERTY(config, EditAnywhere, Category = "Analog Cursor", meta = (ClampMin = "0.0", ClampMax = "1.0"))
		float AnalogCursorDeadZone;

	/** Multiplied by the value spit out by the normalized acceleration curve */
	UPROPERTY(config, EditAnywhere, Category = "Analog Cursor", meta = (ClampMin = "1.0"))
		float AnalogCursorAccelerationMultiplier;

	/** If True, AnalogCursorAccelerationCurve will be used as a Velocity Curve */
	UPROPERTY(config, EditAnywhere, Category = "Analog Cursor")
		bool bAnalogCursorNoAcceleration;

	// Analog Cursor
	TWeakPtr<FGameAnalogCursor> AnalogCursor;
	
};
