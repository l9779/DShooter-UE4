// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterDShooter.h"
#include "CharacterEnemy.generated.h"

class USphereComponent;

UCLASS()
class DSHOOTER_API ACharacterEnemy : public ACharacterDShooter
{
	GENERATED_BODY()
	
public:
	ACharacterEnemy();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
protected:
	UFUNCTION()
	void CombatRangeOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void CombatRangeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	void GivePlayerLocation();

private:
	UPROPERTY(EditAnywhere, Category = "AI", meta = (AllowPrivateAccess = true))
		class UBehaviorTree* BehaviorTree;
	UPROPERTY(EditAnywhere, Category = "AI", meta = (AllowPrivateAccess = true))
		AActor* PatrolPoint1;
	UPROPERTY(EditAnywhere, Category = "AI", meta = (AllowPrivateAccess = true))
		AActor* PatrolPoint2;

	class AEnemy_AIController* EnemyController;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	USphereComponent* CombatRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USphereComponent* CommunicationRange;

public:
	FORCEINLINE UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }
	//FORCEINLINE  AEnemy_AIController* GetEnemyController() const { return EnemyController; }
};
