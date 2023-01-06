// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ActorBullet.h"
#include "AmmoType.h"
#include "CharacterDShooter.h"

// Sets default values
AActorWeapon::AActorWeapon(): 
bIsAutomatic(false),  
CurrentAmmo(20), AmmoCapacity(20),
BulletPellets(5), SpreadRange(35.f),
AutoFireRate(0.25f), FallingWeaponTime(4.f)
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	/*switch (WeaponType) {
	case EWeaponType::EWT_Pistol:
		SetBulletClass(EAmmoType::EAT_PistolBullet);
		break;
	case EWeaponType::EWT_Rifle:
		SetBulletClass(EAmmoType::EAT_RifleBullet);
		break;
	case EWeaponType::EWT_Shotgun:
		SetBulletClass(EAmmoType::EAT_ShotgunBullet);
		break;
	}*/
}

void AActorWeapon::BeginPlay()
{
	Super::BeginPlay();

	MinSpreadRange = SpreadRange * -1;
}

void AActorWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AActorWeapon::SetWeaponProperties(EWeaponState State)
{
	switch (State) {
	case EWeaponState::EWS_Droped:
		Mesh->SetSimulatePhysics(false);
		Mesh->SetEnableGravity(false);
		Mesh->SetVisibility(true);
		Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	case EWeaponState::EWS_Equipped:
		Mesh->SetSimulatePhysics(false);
		Mesh->SetEnableGravity(false);
		Mesh->SetVisibility(true);
		Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	case EWeaponState::EWS_Falling:
		Mesh->SetSimulatePhysics(true);
		Mesh->SetEnableGravity(true);
		Mesh->SetVisibility(true);
		Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);

		SetOwner(this);
		GetWorldTimerManager().SetTimer(FallingWeaponTimer, this, &AActorWeapon::StopFalling, FallingWeaponTime);
		break;
	case EWeaponState::EWS_Holstered:
		Mesh->SetSimulatePhysics(false);
		Mesh->SetEnableGravity(false);
		Mesh->SetVisibility(false);
		Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	}
}


void AActorWeapon::Shoot()
{
	auto OwnerPawn = Cast<ACharacterDShooter>(GetOwner());

	if (CurrentAmmo > 0) {
		if (OwnerPawn) { OwnerPawn->PlayRecoilAnim(); }

		if (BulletClass) {
			const FVector SpawnLocation{ Mesh->GetSocketLocation("MuzzleSocket") };
			const FRotator SpawnRotation{ Mesh->GetSocketRotation("MuzzleSocket") };

			if (WeaponType == EWeaponType::EWT_Shotgun) { // if its a shotgun then fire in cone shape
				for (int32 i = 1; i <= BulletPellets; i++)
				{
					float RandAngle = FMath::FRandRange(MinSpreadRange, SpreadRange);
					FRotator SpawnRotationWithAngle = SpawnRotation;
					SpawnRotationWithAngle.Yaw += RandAngle;
					AActorBullet* TempBullet = GetWorld()->SpawnActor<AActorBullet>(BulletClass, SpawnLocation, SpawnRotationWithAngle);
					TempBullet->SetOwner(this);
				}
			}
			else { // Other types of weapon fire a single bullet in a line.
				AActorBullet* TempBullet = GetWorld()->SpawnActor<AActorBullet>(BulletClass, SpawnLocation, SpawnRotation);
				TempBullet->SetOwner(this);
			}

			if (MuzzleFlash) {
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, SpawnLocation, SpawnRotation);
			}
			if (FireSound) {
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, SpawnLocation);
			}
		}

		StartFireTimer();
		CurrentAmmo--;
	}

	else if (OwnerPawn->GetCarryingAmmo() > 0) { // if CurrentAmmo = 0 but player is carrying ammo -> automatic reload.
		OwnerPawn->PlayReloadMontage();
	}

	else if (EmpityMagSound) { // if CurrentAmmo = 0 and player is not carrying ammo -> play click sound.
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), EmpityMagSound, GetActorLocation());
	}
}

void AActorWeapon::StartFireTimer()
{
	GetWorldTimerManager().SetTimer(AutoFireTimer, this, &AActorWeapon::ResetFireTimer, AutoFireRate);
}

void AActorWeapon::ResetFireTimer()
{
	auto OwnerPawn = Cast<ACharacterDShooter>(GetOwner());
	if (!OwnerPawn) return;

	if (OwnerPawn->GetCombatState() == ECombatState::ECS_ActionAnim) { return; } // if in between auto fire the character rolls stop shooting

	if (OwnerPawn->GetPullingTrigger() && bIsAutomatic) {
		Shoot();
	}
}

void AActorWeapon::StopFalling()
{
	SetWeaponProperties(EWeaponState::EWS_Droped);
}

void AActorWeapon::ReloadWeapon(int32 AmmoToAdd)
{
	CurrentAmmo += AmmoToAdd;
}