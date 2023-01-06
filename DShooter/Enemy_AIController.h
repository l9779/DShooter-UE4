// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Enemy_AIController.generated.h"

UCLASS()
class DSHOOTER_API AEnemy_AIController : public AAIController
{
	GENERATED_BODY()
private:
	UPROPERTY(BlueprintReadWrite, Category = "Ai Behavior", meta = (AllowPrivateAccess = "true"))
	class UBlackboardComponent* BlackboardComponent;
	UPROPERTY(BlueprintReadWrite, Category = "Ai Behavior", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTreeComponent* BehaviorTreeComponent;
public:
	AEnemy_AIController();
	virtual void OnPossess(APawn* InPawn) override;

	FORCEINLINE UBlackboardComponent* GetBlackboardComponent() const { return BlackboardComponent; }
};
