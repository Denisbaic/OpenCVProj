#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "WeaponComponent.generated.h"


UCLASS()
class OPENCVPROJ_API UWeaponComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()
public:
	UWeaponComponent();

	virtual void BeginPlay() override;
protected:
	float LastFireTime;
	/*RPM - Bullets per minute fired by weapon*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		float RateOfFire;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon")
		void Fire();
	
	//Derived from RPM
	float TimeBetweenShots;
	
	FTimerHandle TimerHandle_TimeBetweenShots;
public:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		TSubclassOf<UCameraShake> FireCamShake;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		USoundBase* SoundOfFire;
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "Weapon")
		void StartFire();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon")
		void StopFire();
	UFUNCTION(BlueprintCallable)
		FTransform GetMuzzleTransform() const;
};
