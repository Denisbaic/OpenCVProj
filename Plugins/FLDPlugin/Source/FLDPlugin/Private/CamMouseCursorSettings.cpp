// Fill out your copyright notice in the Description page of Project Settings.


#include "CamMouseCursorSettings.h"

UCamMouseCursorSettings::UCamMouseCursorSettings()
{
	MaxAnalogCursorSpeed = 1300.0f;
	MaxAnalogCursorSpeedWhenHovered = 650.0f;
	AnalogCursorDragCoefficient = 1;
	AnalogCursorDragCoefficientWhenHovered = 0.5f;
	AnalogCursorDeadZone = 0.15f;
	AnalogCursorAccelerationMultiplier = 9000.0f;
	
	AnalogCursorAccelerationCurve.EditorCurveData.AddKey(0, 0);
	AnalogCursorAccelerationCurve.EditorCurveData.AddKey(1, 1);
}
