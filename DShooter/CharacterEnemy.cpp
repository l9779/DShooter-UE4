#include "CharacterEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CharacterMain.h"
#include "Components/SphereComponent.h"
//#include "Components/CapsuleComponent.h"

ACharacterEnemy::ACharacterEnemy()
{
	PrimaryActorTick.bCanEverTick = false;

	CombatRange = CreateDefaultSubobject<USphereComponent>(TEXT("Combat Range"));
	CombatRange->SetSphereRadius(1000.f);
	CombatRange->SetupAttachment(GetRootComponent());
	/* DO NOT TOUCH CreateDefauultSubobject of SPHERES PLEASE */
	CommunicationRange = CreateDefaultSubobject<USphereComponent>(TEXT("Communication Range"));
	CommunicationRange->SetSphereRadius(750.f);
	CommunicationRange->ShapeColor = FColor::Blue;
	CommunicationRange->SetupAttachment(GetRootComponent());
}

void ACharacterEnemy::BeginPlay()
{
	Super::BeginPlay();

	EnemyController = Cast<AEnemy_AIController>(GetController());

	if (EnemyController) {
		if (PatrolPoint1) {
			EnemyController->GetBlackboardComponent()->SetValueAsObject(TEXT("Patrol1"), PatrolPoint1);
		}
		if (PatrolPoint2) {
			EnemyController->GetBlackboardComponent()->SetValueAsObject(TEXT("Patrol2"), PatrolPoint2);
		}
		EnemyController->RunBehaviorTree(BehaviorTree);
	}

	CombatRange->OnComponentBeginOverlap.AddDynamic(this, &ACharacterEnemy::CombatRangeOverlap);
	CombatRange->OnComponentEndOverlap.AddDynamic(this, &ACharacterEnemy::CombatRangeEndOverlap);
}

void ACharacterEnemy::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void ACharacterEnemy::CombatRangeOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetHealth() <= 0) return;
	if (!OtherActor) return;
	if (!EnemyController) return;
	auto Character = Cast<ACharacterMain>(OtherActor);

	if (Character && EnemyController->GetBlackboardComponent()) {
		EnemyController->GetBlackboardComponent()->SetValueAsBool(TEXT("PlayerInRange"), true);
	}
}

void ACharacterEnemy::CombatRangeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (GetHealth() <= 0) return;
	if (!OtherActor) return;
	if (!EnemyController) return;
	auto Character = Cast<ACharacterMain>(OtherActor);
	
	if (Character && EnemyController->GetBlackboardComponent()) {
		EnemyController->GetBlackboardComponent()->SetValueAsBool(TEXT("PlayerInRange"), false);
	}
}

void ACharacterEnemy::GivePlayerLocation()
{
	/*TArray<AActor*> OverlappingActors;
	CommunicationRange->GetOverlappingActors(OverlappingActors, ACharacterEnemy::StaticClass());
	for ( AActor* Actor : OverlappingActors) {
		ACharacterEnemy* NearbyEnemy = Cast<ACharacterEnemy>(Actor);

		if (NearbyEnemy && NearbyEnemy->GetEnemyController()->GetBlackboardComponent()){
			const FVector PlayerLocation = EnemyController->GetBlackboardComponent()->GetValueAsVector(TEXT("PlayerLocation"));
			NearbyEnemy->GetEnemyController()->GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerLocation);
		}
	}*/
}