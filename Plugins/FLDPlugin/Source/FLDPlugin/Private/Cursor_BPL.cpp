// Fill out your copyright notice in the Description page of Project Settings.


#include "Cursor_BPL.h"
#include "CamMouseCursorSettings.h"
#include "GameAnalogCursor.h"

void UVirtualCursorFunctionLibrary::EnableVirtualCursor(class APlayerController* PC)
{
	FGameAnalogCursor::EnableAnalogCursor(PC, TSharedPtr<SWidget>());
}

void UVirtualCursorFunctionLibrary::DisableVirtualCursor(class APlayerController* PC)
{
	FGameAnalogCursor::DisableAnalogCursor(PC);
}

bool UVirtualCursorFunctionLibrary::IsCursorOverInteractableWidget()
{
	TSharedPtr<FGameAnalogCursor> Analog = GetDefault<UCamMouseCursorSettings>()->GetAnalogCursor();
	if (Analog.IsValid())
	{
		return Analog->IsHovered();
	}

	return false;
}

void UVirtualCursorFunctionLibrary::LeftClick()
{
	TSharedPtr<FGameAnalogCursor> Analog = GetDefault<UCamMouseCursorSettings>()->GetAnalogCursor();
	Analog->TriggerMouseLMB();
}

void UVirtualCursorFunctionLibrary::RightClick()
{
	TSharedPtr<FGameAnalogCursor> Analog = GetDefault<UCamMouseCursorSettings>()->GetAnalogCursor();
	Analog->TriggerMouseRMB();
}
