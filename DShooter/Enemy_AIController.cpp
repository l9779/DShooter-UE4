// Fill out your copyright notice in the Description page of Project Settings.
#include "Enemy_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "CharacterEnemy.h"

AEnemy_AIController::AEnemy_AIController()
{
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard Component"));
	check(BlackboardComponent);

	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behavior Tree Component"));
	check(BehaviorTreeComponent);
}

void AEnemy_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (!InPawn) return;

	ACharacterEnemy* Enemy = Cast<ACharacterEnemy>(InPawn);
	if (Enemy) {
		if (Enemy->GetBehaviorTree()) {
			BlackboardComponent->InitializeBlackboard(*(Enemy->GetBehaviorTree()->GetBlackboardAsset()));
		}
	}
}
