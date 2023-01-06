// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterDShooter.generated.h"

class AActorWeapon;
class UAnimMontage;
class AController;

UENUM(BlueprintType)
enum class ECombatState : uint8
{
	ECS_Free UMETA(DisplayName = "Free"),
	//ECS_FireTimer UMETA(DisplayName = "Fire Timer in Progress"),
	ECS_ActionAnim UMETA(DisplayName = "Action Animation"),
	ECS_Reloading UMETA(DisplayName = "Reloading"),
	ECS_Equipping UMETA(DisplayName = "Equipping"),

	ECS_Max UMETA(DisplayName = "Default MAX")
};

UCLASS()
class DSHOOTER_API ACharacterDShooter : public ACharacter
{
	GENERATED_BODY()

protected:
	ACharacterDShooter();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void ExchangeAmmo();

	virtual void RotateCharacter(FVector LookAtTarget);

	virtual void Dodge();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	bool IsDead();
	void DeathEvent(AActor* KillerActor);

	void EquipWeapon(AActorWeapon* WeaponToEquip);
private:
	UPROPERTY(EditAnywhere, Category="Weapons", meta = (AllowPrivateAccess = true))
		TSubclassOf<AActorWeapon> WeaponClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons", meta = (AllowPrivateAccess = true))
		AActorWeapon* EquippedWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = true))
		int32 CarryingAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Montages", meta = (AllowPrivateAccess = true))
		UAnimMontage* RecoilMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Montages", meta = (AllowPrivateAccess = true))
		UAnimMontage* ReloadMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Montages", meta = (AllowPrivateAccess = true))
		UAnimMontage* DodgeMontage;

	// Effect that spawn when character gets hit by bullet
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VFX", meta = (AllowPrivateAccess = true))
	UParticleSystem* HitEffect;

	/*Free, ActionAnim, Reloading, Equipping can go from free to any other state, diferent restrictions on transictioning other to other states. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = true))
		ECombatState CombatState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = true))
		bool bPullingTrigger;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = true))
		float Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = true))
		float MaxHealth;
public:
	UFUNCTION(BlueprintCallable)
	void PullTrigger();
	void PlayRecoilAnim();
	void PlayReloadMontage();

	/*Called by anim notifies, changes the enum state based on the beginnig and ending of animation.*/
	UFUNCTION(BlueprintCallable)
	void SetRolling(bool B);
	FORCEINLINE void SetPullingTrigger(bool B) { bPullingTrigger = B; } // on public so child classes can use

	FORCEINLINE int32 GetCarryingAmmo() const { return CarryingAmmo; }
	FORCEINLINE bool GetPullingTrigger() const { return bPullingTrigger; }
	FORCEINLINE AActorWeapon* GetEquippedWeapon() const { return EquippedWeapon; }
	FORCEINLINE ECombatState GetCombatState() const { return CombatState; }
	FORCEINLINE UParticleSystem* GetHitEffect() const { return HitEffect; }
	FORCEINLINE float GetHealth() const { return Health; }
};
