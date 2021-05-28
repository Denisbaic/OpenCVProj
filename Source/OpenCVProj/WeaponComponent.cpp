// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"
#include "TimerManager.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"

UWeaponComponent::UWeaponComponent()
{
	RateOfFire = 600.f;
}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	TimeBetweenShots = 60.f / RateOfFire;
}

void UWeaponComponent::Fire_Implementation()
{
	APawn* MyOwner = Cast<APawn>(GetOwner());
	if (MyOwner)
	{
		APlayerController* PC = Cast<APlayerController>(MyOwner->GetController());
		if (PC)
		{
			PC->ClientPlayCameraShake(FireCamShake);
		}
	}
	if (SoundOfFire)
		UGameplayStatics::PlaySound2D(GetWorld(), SoundOfFire);
	LastFireTime = GetWorld()->GetTimeSeconds();
}

void UWeaponComponent::StartFire()
{
	float FirstDelay = FMath::Max(LastFireTime + TimeBetweenShots - GetWorld()->TimeSeconds, 0.f);
	GetOwner()->GetWorldTimerManager().SetTimer(TimerHandle_TimeBetweenShots, this, &UWeaponComponent::Fire, TimeBetweenShots, true, FirstDelay);
}

void UWeaponComponent::StopFire()
{
	GetOwner()->GetWorldTimerManager().ClearTimer(TimerHandle_TimeBetweenShots);
}

FTransform UWeaponComponent::GetMuzzleTransform() const
{
	FVector Vec;
	FRotator Rot;
	GetSocketWorldLocationAndRotation(FName("Muzzle"), Vec, Rot);
	
	return FTransform(Rot,Vec,{0.1f,0.1f,0.3f});
}
