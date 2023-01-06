// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterDShooter.h"
//#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayerControllerDShooter.h"
#include "ActorWeapon.h"
#include "ActorBullet.h"
#include "Enemy_AIController.h"
#include "PlayerControllerDShooter.h"
// Sets default values
ACharacterDShooter::ACharacterDShooter(): 
CarryingAmmo(120), Health(100.f), MaxHealth(100.f),
CombatState(ECombatState::ECS_Free), bPullingTrigger(false)
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACharacterDShooter::BeginPlay()
{
	Super::BeginPlay();

	EquippedWeapon = GetWorld()->SpawnActor<AActorWeapon>(WeaponClass);
	EquippedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("hand_rSocket"));
	EquippedWeapon->SetOwner(this);
	EquipWeapon(EquippedWeapon);
}

// Called every frame
void ACharacterDShooter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACharacterDShooter::RotateCharacter(FVector LookAtTarget){
	FVector LookAtTargetCleaned = FVector(LookAtTarget.X, LookAtTarget.Y, GetMesh()->GetComponentLocation().Z);
	FVector StartLocation = GetMesh()->GetComponentLocation();

	const FRotator CharacterRotation = FVector(LookAtTargetCleaned - StartLocation).Rotation();

	if (CombatState != ECombatState::ECS_ActionAnim) {
		SetActorRotation(CharacterRotation);
	} 
}

void ACharacterDShooter::EquipWeapon(AActorWeapon* WeaponToEquip)
{
	WeaponToEquip->SetWeaponProperties(EWeaponState::EWS_Equipped);
}

void ACharacterDShooter::PullTrigger()
{
	if (CombatState != ECombatState::ECS_Free) { return; }
	if (EquippedWeapon) {
		GetEquippedWeapon()->Shoot();
	}
}

void ACharacterDShooter::Dodge()
{
	if (CombatState == ECombatState::ECS_ActionAnim) { return; } // if character is already in animation dont do again.

	UAnimInstance* AnimInstance = Cast<UAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance && DodgeMontage) {
		AnimInstance->Montage_Play(DodgeMontage);
		AnimInstance->Montage_JumpToSection(FName("Roll"));
		CombatState = ECombatState::ECS_ActionAnim;
	}
}

float ACharacterDShooter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToApply = FMath::Min(DamageToApply, Health); // Min() retrun the smallest of the 2 values
	Health -= DamageToApply;

	if (IsDead()) {
		DeathEvent(DamageCauser);
	}
	return DamageToApply;
}

bool ACharacterDShooter::IsDead()
{
	return Health <= 0;
}

void ACharacterDShooter::DeathEvent(AActor* KillerActor)
{
	if (Cast<AEnemy_AIController>(GetController())) {
		DetachFromControllerPendingDestroy();
	}
	if (Cast<APlayerControllerDShooter>(GetController())) {
		GEngine->AddOnScreenDebugMessage(0, 1, FColor::Red, FString::Printf(TEXT("Game over!")));
	}

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->WakeAllRigidBodies();
	GetMesh()->SetSimulatePhysics(true);

	AActorBullet* KillerBullet = Cast<AActorBullet>(KillerActor);

	if (KillerBullet) {
		FVector const BulletImpulse = KillerBullet->GetActorForwardVector() * KillerBullet->GetImpulseForce();
		GetMesh()->AddImpulse(BulletImpulse, FName(TEXT("spine_02")));
		EquippedWeapon->SetWeaponProperties(EWeaponState::EWS_Falling);
		EquippedWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}
}

void ACharacterDShooter::PlayRecoilAnim()
{
	if (CombatState != ECombatState::ECS_Free) { return; }

	UAnimInstance* AnimInstance = Cast<UAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance && RecoilMontage) {
		AnimInstance->Montage_Play(RecoilMontage);
		AnimInstance->Montage_JumpToSection(FName("StartFire"));
	}
}

void ACharacterDShooter::PlayReloadMontage(){
	if (CombatState != ECombatState::ECS_Free) { return; }

	UAnimInstance* AnimInstance = Cast<UAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance && ReloadMontage) {
		CombatState = ECombatState::ECS_Reloading;
		AnimInstance->Montage_Play(ReloadMontage);
		AnimInstance->Montage_JumpToSection(FName("ReloadRifle"));
	}
}

void ACharacterDShooter::SetRolling(bool B)
{
	if (B) { 
		CombatState = ECombatState::ECS_ActionAnim;
	}
	else { 
		CombatState = ECombatState::ECS_Free;
		if (bPullingTrigger) {
			PullTrigger(); // if finished roll animation and bPullingTrigger is true than PullTrigger()
		}
	}
}

void ACharacterDShooter::ExchangeAmmo() // called by notify when animation ends.
{
	CombatState = ECombatState::ECS_Free;
	int32 AmmoToAdd = EquippedWeapon->GetAmmoCapacity() - EquippedWeapon->GetCurrentAmmo();

	if (CarryingAmmo >= AmmoToAdd) {
		CarryingAmmo -= AmmoToAdd;
	}
	else {
		AmmoToAdd = CarryingAmmo;
		CarryingAmmo = 0;
	}

	EquippedWeapon->ReloadWeapon(AmmoToAdd);

	if (bPullingTrigger) {
		PullTrigger(); // if finished reload animation and bPullTrigger is true than PullTrigger()
	}
}