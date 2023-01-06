// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponType.h"
#include "AmmoType.h"
#include "ActorWeapon.generated.h"

class USkeletalMeshComponent;

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Falling UMETA(DisplayName = "Falling"),
	EWS_Droped UMETA(DisplayName = "Droped"),
	EWS_Equipped UMETA(DisplayName = "Equipped"),
	EWS_Holstered UMETA(DisplayName = "Holstered"),

	EWS_Max UMETA(DisplayName = "Default MAX")
};

UCLASS()
class DSHOOTER_API AActorWeapon : public AActor
{
	GENERATED_BODY()

	AActorWeapon();

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* Mesh;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void StartFireTimer();
	void ResetFireTimer();

	void StopFalling();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* MuzzleFlash;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components", meta = (AllowPrivateAccess = "true"))
	USoundBase* FireSound;
	/*Plays when Shoot() is called but there's no ammo left.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USoundBase* EmpityMagSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = true))
		EWeaponType WeaponType;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Properties", meta = (AllowPrivateAccess = true))
		EWeaponState WeaponState;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Properties", meta = (AllowPrivateAccess = true))
		TSubclassOf<class AActorBullet> BulletClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = true))
		bool bIsAutomatic;

	float FallingWeaponTime;
	FTimerHandle FallingWeaponTimer;

	/*Current Amount of ammo in the magazine.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	int32 CurrentAmmo;
	/*Max amount of ammo in the magazine.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	int32 AmmoCapacity;

	/*Only used on shotgun, ammount of pellets to come out of the barrel.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	int32 BulletPellets;
	/*Only used on shotgun, determines the spread of the bullets*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"),
	meta = (ClampMin = "0.0", ClampMax = "90.0", UIMin = "0", UIMax = "90"))
	float SpreadRange;
	float MinSpreadRange;

	FTimerHandle AutoFireTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	float AutoFireRate;
	
public:	
	void Shoot();
	void ReloadWeapon(int32 AmmoToAdd);
	void SetWeaponProperties(EWeaponState State);

	//FORCEINLINE void SetWeaponState(EWeaponState NewState) { WeaponState = NewState; }

	FORCEINLINE bool GetIsAutomatic() const { return bIsAutomatic; }
	FORCEINLINE int32 GetCurrentAmmo() const { return CurrentAmmo; }
	FORCEINLINE int32 GetAmmoCapacity() const { return AmmoCapacity; }
};
