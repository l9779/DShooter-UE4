// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AmmoType.h"
#include "ActorBullet.generated.h"

UCLASS()
class DSHOOTER_API AActorBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActorBullet();

	virtual void Tick(float Deltatime) override;

private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo Properties", meta = (AllowPrivateAccess = true))
		EAmmoType AmmoType;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Ammo Properties", meta = (AllowPrivateAccess = true))
		class UProjectileMovementComponent* ProjectileMovement;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo Properties", meta = (AllowPrivateAccess = true))
		float MovementSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo Properties", meta = (AllowPrivateAccess = true))
		float Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo Properties", meta = (AllowPrivateAccess = true))
		float ImpulseForce;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;
	/*Effect that spawn when bullet hits surfaces and not characters*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* ImpactEffect; // talvez no futuro, pode usar só isso com SurfaceType em vez de GetHitEffect() no personagem.

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UDamageType> DamageType;
public:
	FORCEINLINE float GetImpulseForce() const { return ImpulseForce; }
};
