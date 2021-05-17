// Fill out your copyright notice in the Description page of Project Settings.


#include "Cursor_BPL.h"
#include "CamMouseCursorSettings.h"
#include "GameAnalogCursor.h"

void UCursor_BPL::EnableVirtualCursor(class APlayerController* PC)
{
	FGameAnalogCursor::EnableAnalogCursor(PC, TSharedPtr<SWidget>());
}

void UCursor_BPL::DisableVirtualCursor(class APlayerController* PC)
{
	FGameAnalogCursor::DisableAnalogCursor(PC);
}

bool UCursor_BPL::IsCursorOverInteractableWidget()
{
	TSharedPtr<FGameAnalogCursor> Analog = GetDefault<UCamMouseCursorSettings>()->GetAnalogCursor();
	if (Analog.IsValid())
	{
		return Analog->IsHovered();
	}
	return false;
}

void UCursor_BPL::LeftClick()
{
	TSharedPtr<FGameAnalogCursor> Analog = GetDefault<UCamMouseCursorSettings>()->GetAnalogCursor();
	if (Analog.IsValid())
	{
		return Analog->TriggerMouseLMB();
	}
	else
	{
		FViewportClient* Client = GEngine->GameViewport->Viewport->GetClient();
		FKey MouseLMB = EKeys::LeftMouseButton;
		Client->InputKey(GEngine->GameViewport->Viewport, 0, MouseLMB, EInputEvent::IE_Pressed);
		Client->InputKey(GEngine->GameViewport->Viewport, 0, MouseLMB, EInputEvent::IE_Released);
	}
}

void UCursor_BPL::RightClick()
{
	TSharedPtr<FGameAnalogCursor> Analog = GetDefault<UCamMouseCursorSettings>()->GetAnalogCursor();
	if (Analog.IsValid())
	{
		return Analog->TriggerMouseRMB();
	}
	else
	{
		FViewportClient* Client = GEngine->GameViewport->Viewport->GetClient();
		FKey MouseRMB = EKeys::RightMouseButton;
		Client->InputKey(GEngine->GameViewport->Viewport, 0, MouseRMB, EInputEvent::IE_Pressed);
		Client->InputKey(GEngine->GameViewport->Viewport, 0, MouseRMB, EInputEvent::IE_Released);
	}
}

void UCursor_BPL::WheelInput(float Input,float DeltaTime)
{
	TSharedPtr<FGameAnalogCursor> Analog = GetDefault<UCamMouseCursorSettings>()->GetAnalogCursor();
	if (Analog.IsValid())
	{
		return Analog->TriggerWheel(Input,DeltaTime);
	}
	else
	{
		FViewportClient* Client = GEngine->GameViewport->Viewport->GetClient();
		FKey MouseAxis = EKeys::MouseWheelAxis;
		Client->InputAxis(GEngine->GameViewport->Viewport, 0, MouseAxis, FMath::Clamp(Input, -1.f, 1.f), DeltaTime);
	}
}

void UCursor_BPL::MoveMouse(FVector2D Input, float DeltaTime)
{
	TSharedPtr<FGameAnalogCursor> Analog = GetDefault<UCamMouseCursorSettings>()->GetAnalogCursor();
	if (Analog.IsValid())
	{
		return Analog->SetInputVector(Input);
	}
	else
	{
		FViewportClient* Client = GEngine->GameViewport->Viewport->GetClient();
		FKey const MouseX = EKeys::MouseX;
		FKey const MouseY = EKeys::MouseY;
		Client->InputAxis(GEngine->GameViewport->Viewport, 0, MouseX, Input.X, DeltaTime);
		Client->InputAxis(GEngine->GameViewport->Viewport, 0, MouseY, Input.Y, DeltaTime);
	}
}
