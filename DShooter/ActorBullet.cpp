// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorBullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CharacterDShooter.h"

AActorBullet::AActorBullet():
	MovementSpeed(1000.f), Damage(15.f), ImpulseForce(1000.f)
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->OnComponentHit.AddDynamic(this, &AActorBullet::OnHit);
	RootComponent = Mesh;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->InitialSpeed = MovementSpeed;
	ProjectileMovement->MaxSpeed = MovementSpeed;

	InitialLifeSpan = 3.f;
}	

void AActorBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AActorBullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AActor* MyOwner = GetOwner();
	if (!MyOwner) return;

	if ((OtherActor) && (OtherActor != this) && (OtherActor != MyOwner) && (OtherActor != MyOwner->GetOwner()) ){
		UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwner->GetInstigatorController(), this, DamageType);

		auto HitCharacter = Cast<ACharacterDShooter>(OtherActor);
		if (HitCharacter && HitCharacter->GetHitEffect()) {
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitCharacter->GetHitEffect(),
													Hit.Location, GetActorRotation(), {0.6,0.6,0.6});
		}
	}
	else {
		if (ImpactEffect) {
			UGameplayStatics::SpawnEmitterAtLocation(this, ImpactEffect, GetActorLocation()); // Should spawn praticle based on surface type
		}
	}

	Destroy();
}
