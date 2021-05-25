// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAnalogCursor.h"

#include "GameAnalogCursor.h"

#include "CamMouseCursorSettings.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Engine/UserInterfaceSettings.h"

bool IsWidgetInteractable(const TSharedPtr<SWidget> Widget)
{
	return Widget.IsValid() && Widget->IsInteractable();
}

////////////////////////////////////////////////////////////////////////////
// FGameAnalogCursor
////////////////////////////////////////////////////////////////////////////

void FGameAnalogCursor::EnableAnalogCursor(class APlayerController* PC, TSharedPtr<SWidget> WidgetToFocus)
{
	if (PC)
	{
		TSharedPtr<FGameAnalogCursor> AnalogCursor = MakeShareable(new FGameAnalogCursor(PC));
		FSlateApplication::Get().RegisterInputPreProcessor(AnalogCursor);

		GetMutableDefault<UCamMouseCursorSettings>()->SetAnalogCursor(AnalogCursor);

		//setup the new input mode
		FInputModeGameAndUI NewInputMode;
		//NewInputMode.SetLockMouseToViewport(true);
		NewInputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockOnCapture);

		NewInputMode.SetWidgetToFocus(WidgetToFocus);
		PC->SetInputMode(NewInputMode);
	}
}

void FGameAnalogCursor::DisableAnalogCursor(class APlayerController* PC)
{
	if (PC)
	{
		if (FSlateApplication::IsInitialized())
		{
			FSlateApplication::Get().UnregisterInputPreProcessor(GetMutableDefault<UCamMouseCursorSettings>()->GetAnalogCursor());
			FSlateApplication::Get().SetCursorRadius(0.0f);
		}

		FInputModeGameOnly NewInputMode;
		PC->SetInputMode(NewInputMode);
	}
}

FGameAnalogCursor::FGameAnalogCursor(class APlayerController* PC)
	: Velocity(FVector2D::ZeroVector)
	, HoveredWidgetName(NAME_None)
	, bIsUsingAnalogCursor(false)
	, PlayerContext(PC)
{
	ensure(PlayerContext.IsValid());
}

void FGameAnalogCursor::Tick(float const  DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor)
{
	if (PlayerContext.IsValid())
	{
		FVector2D const  ViewportSize = UWidgetLayoutLibrary::GetViewportSize(PlayerContext.GetPlayerController());
		float const  DPIScale = GetDefault<UUserInterfaceSettings>()->GetDPIScaleBasedOnSize(FIntPoint(FMath::RoundToInt(ViewportSize.X), FMath::RoundToInt(ViewportSize.Y)));

		const UCamMouseCursorSettings* Settings = GetDefault<UCamMouseCursorSettings>();
		
		//set the current position if we haven't already
		/*
		static float const  MouseMoveSizeBuffer = 2.0f;
		FVector2D const  CurrentPositionTruc = FVector2D(FMath::TruncToFloat(CurrentPosition.X), FMath::TruncToFloat(CurrentPosition.Y));
		if (CurrentPositionTruc != Cursor->GetPosition())
		{
			CurrentPosition = Cursor->GetPosition();
			Velocity = FVector2D::ZeroVector;
			LastCursorDirection = FVector2D::ZeroVector;
			bIsUsingAnalogCursor = false;
			FSlateApplication::Get().SetCursorRadius(0.0f);
		}
		*/
		//cache the old position
		FVector2D OldPosition = Cursor->GetPosition();

		//figure out if we should clamp the speed or not
		float const  MaxSpeedNoHover = Settings->GetMaxAnalogCursorSpeed() * DPIScale;
		float const  MaxSpeedHover = Settings->GetMaxAnalogCursorSpeedWhenHovered() * DPIScale;
		float const  DragCoNoHover = Settings->GetAnalogCursorDragCoefficient() * DPIScale;
		float const  DragCoHovered = Settings->GetAnalogCursorDragCoefficientWhenHovered() * DPIScale;


		HoveredWidgetName = NAME_None;
		float DragCo = DragCoNoHover;

		//Part of base class now
		MaxSpeed = MaxSpeedNoHover;

		//see if we are hovered over a widget or not
		FWidgetPath WidgetPath = SlateApp.LocateWindowUnderMouse(OldPosition, SlateApp.GetInteractiveTopLevelWindows());
		if (WidgetPath.IsValid())
		{
			for (int32 i = WidgetPath.Widgets.Num() - 1; i >= 0; --i)
			{
				//grab the widget
				FArrangedWidget& ArrangedWidget = WidgetPath.Widgets[i];
				TSharedRef<SWidget> Widget = ArrangedWidget.Widget;

				//see if it is acceptable or not
				if (IsWidgetInteractable(Widget))
				{
					HoveredWidgetName = Widget->GetType();
					DragCo = DragCoHovered;
					MaxSpeed = MaxSpeedHover;
					break;
				}
			}
		}

		
		
		float const MaxTime = Settings->GetAnalogCursorAccelerationCurve()->GetLastKey().Time;
		Time = Time = FMath::Clamp(Time+ DeltaTime, 0.f, MaxTime);		
		
		//grab the cursor acceleration
		FVector2D const  AccelFromAnalogStick = GetAnalogCursorAccelerationValue(InputVector, DPIScale);

		/*
		if (!Settings->GetAnalogCursorNoAcceleration())
		{
			//Calculate a new velocity. RK4.
			if (!AccelFromAnalogStick.IsZero() || !Velocity.IsZero())
			{
				FVector2D const  A1 = (AccelFromAnalogStick - (DragCo * Velocity)) * DeltaTime;
				FVector2D const  A2 = (AccelFromAnalogStick - (DragCo * (Velocity + (A1 * 0.5f)))) * DeltaTime;
				FVector2D const  A3 = (AccelFromAnalogStick - (DragCo * (Velocity + (A2 * 0.5f)))) * DeltaTime;
				FVector2D const  A4 = (AccelFromAnalogStick - (DragCo * (Velocity + A3))) * DeltaTime;
				FVector2D const NewAccelerationThisFrame = (A1 + (2.0f * A2) + (2.0f * A3) + A4) / 6.0f;
				Velocity += NewAccelerationThisFrame;
			}
		}
		else
		{
			//else, use what is coming straight from the analog stick
			Velocity = AccelFromAnalogStick;
		}
		*/
		
		Velocity = AccelFromAnalogStick;
		
		//if we are smaller than out min speed, zero it out
		float const  VelSizeSq = Velocity.SizeSquared();
		if (VelSizeSq > (MaxSpeed * MaxSpeed))
		{
			//also cap us if we are larger than our max speed
			Velocity = Velocity.GetSafeNormal() * MaxSpeed;
		}

		//store off the last cursor direction
		if (Velocity.IsZero())
		{
			InputVector = FVector2D::ZeroVector;
		}

		//update the new position
		OldPosition += (Velocity * DeltaTime);

		if (!GEngine && GEngine->GameViewport)
			return;
		FViewportClient* Client = GEngine->GameViewport->Viewport->GetClient();
		FKey const MouseX = EKeys::MouseX;
		FKey const MouseY = EKeys::MouseY;
		Client->InputAxis(GEngine->GameViewport->Viewport, 0, MouseX, InputVector.X, DeltaTime);
		Client->InputAxis(GEngine->GameViewport->Viewport, 0, MouseY, InputVector.Y, DeltaTime);
		
		if (TSharedPtr<FSlateUser> SlateUser = SlateApp.GetUser(GetOwnerUserIndex()))
		{
			//update the cursor position
			UpdateCursorPosition(SlateApp, SlateUser.ToSharedRef(), OldPosition);
		}			
	}
	//InputVector = FVector2D::ZeroVector;
}


void FGameAnalogCursor::TriggerMouseLMB()
{
	if (!GEngine)
		return;
	FViewportClient* Client = GEngine->GameViewport->Viewport->GetClient();
	FKey MouseLMB = EKeys::LeftMouseButton;
	Client->InputKey(GEngine->GameViewport->Viewport, 0, MouseLMB, EInputEvent::IE_Pressed);
	Client->InputKey(GEngine->GameViewport->Viewport, 0, MouseLMB, EInputEvent::IE_Released);
	
	//Get our slate application
	FSlateApplication& SlateApp = FSlateApplication::Get();

	//create a pointer event
	FPointerEvent MouseEvent(
		0,
		SlateApp.CursorPointerIndex,
		SlateApp.GetCursorPos(),
		SlateApp.GetLastCursorPos(),
		SlateApp.GetPressedMouseButtons(),
		EKeys::LeftMouseButton,
		0,
		SlateApp.GetPlatformApplication()->GetModifierKeys()
	);

	//send the mouse event to the slate handler
	TSharedPtr<FGenericWindow> GenWindow;
	SlateApp.ProcessMouseButtonDownEvent(GenWindow, MouseEvent);

	SlateApp.ProcessMouseButtonUpEvent(MouseEvent);
}

void FGameAnalogCursor::TriggerMouseRMB()
{
	if (!GEngine)
		return;
	FViewportClient* Client = GEngine->GameViewport->Viewport->GetClient();
	FKey MouseRMB = EKeys::RightMouseButton;
	Client->InputKey(GEngine->GameViewport->Viewport, 0, MouseRMB, EInputEvent::IE_Pressed);
	Client->InputKey(GEngine->GameViewport->Viewport, 0, MouseRMB, EInputEvent::IE_Released);
	
	//Get our slate application
	FSlateApplication& SlateApp = FSlateApplication::Get();

	//create a pointer event
	FPointerEvent MouseEvent(
		0,
		SlateApp.CursorPointerIndex,
		SlateApp.GetCursorPos(),
		SlateApp.GetLastCursorPos(),
		SlateApp.GetPressedMouseButtons(),
		EKeys::RightMouseButton,
		0,
		SlateApp.GetPlatformApplication()->GetModifierKeys()
	);

	//send the mouse event to the slate handler
	TSharedPtr<FGenericWindow> GenWindow;
	SlateApp.ProcessMouseButtonDownEvent(GenWindow, MouseEvent);

	SlateApp.ProcessMouseButtonUpEvent(MouseEvent);
}

void FGameAnalogCursor::TriggerWheel(float InDeltaWheel,float DeltaTime)
{
	if(!GEngine)
		return;
	FViewportClient* Client = GEngine->GameViewport->Viewport->GetClient();
	FKey MouseAxis = EKeys::MouseWheelAxis;

	Client->InputAxis(GEngine->GameViewport->Viewport, 0, MouseAxis, FMath::Clamp(InDeltaWheel, -1.f, 1.f), DeltaTime);
	
	//Get our slate application
	FSlateApplication& SlateApp = FSlateApplication::Get();

	//create a pointer event
	FPointerEvent MouseEvent(
		0,
		SlateApp.CursorPointerIndex,
		SlateApp.GetCursorPos(),
		SlateApp.GetLastCursorPos(),
		SlateApp.GetPressedMouseButtons(),
		EKeys::MouseWheelAxis,
		InDeltaWheel,
		SlateApp.GetPlatformApplication()->GetModifierKeys()
	);
	SlateApp.ProcessMouseWheelOrGestureEvent(MouseEvent, nullptr);
}

FVector2D FGameAnalogCursor::GetAnalogCursorAccelerationValue(const FVector2D& InAnalogValues, float DPIScale)
{
	const UCamMouseCursorSettings* Settings = GetDefault<UCamMouseCursorSettings>();

	FVector2D RetValue = FVector2D::ZeroVector;
	if (const FRichCurve* AccelerationCurve = Settings->GetAnalogCursorAccelerationCurve())
	{
		float const  DeadZoneSize = Settings->GetAnalogCursorDeadZone();
		float const  AnalogValSize = InAnalogValues.Size();
		if (AnalogValSize > DeadZoneSize)
		{			
			RetValue = AccelerationCurve->Eval(Time) * InAnalogValues.GetSafeNormal() * DPIScale;
			RetValue *= Settings->GetAnalogCursorAccelerationMultiplier() * DPIScale;
		}
		else
		{
			Time = 0.f;
		}
	}
	return RetValue;
}
