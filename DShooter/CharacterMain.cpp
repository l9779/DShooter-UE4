#include "CharacterMain.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayerControllerDShooter.h"
#include "ActorWeapon.h"
#include "Kismet/GameplayStatics.h"

ACharacterMain::ACharacterMain()
{
	bUseControllerRotationPitch = true;
	bUseControllerRotationRoll = true;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 900.f;
	CameraBoom->SetWorldRotation(FRotator(-50.f, -50.f, 0.f));
	CameraBoom->bUsePawnControlRotation = false;
	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritYaw = false;
	CameraBoom->bInheritRoll = false;
	CameraBoom->bDoCollisionTest = false;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->SetFieldOfView(80.f);
	FollowCamera->bUsePawnControlRotation = false;
}

void ACharacterMain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GetCursorTrace();
}

void ACharacterMain::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ACharacterMain::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ACharacterMain::MoveRight);

	PlayerInputComponent->BindAction(TEXT("PrimaryAttack"), IE_Pressed, this, &ACharacterMain::FireButtonPressed);
	PlayerInputComponent->BindAction(TEXT("PrimaryAttack"), IE_Released, this, &ACharacterMain::FireButtonReleased);
	PlayerInputComponent->BindAction(TEXT("Reload"), IE_Pressed, this, &ACharacterMain::ReloadPressed);
	PlayerInputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &ACharacterMain::InteractPressed);
	PlayerInputComponent->BindAction(TEXT("Pickup"), IE_Pressed, this, &ACharacterMain::PickupPressed);
	PlayerInputComponent->BindAction(TEXT("1Key"), IE_Pressed, this, &ACharacterMain::OneKeyPressed);
	PlayerInputComponent->BindAction(TEXT("2Key"), IE_Pressed, this, &ACharacterMain::TwoKeyPressed);
	PlayerInputComponent->BindAction(TEXT("3Key"), IE_Pressed, this, &ACharacterMain::ThreeKeyPressed);
	PlayerInputComponent->BindAction(TEXT("Roll"), IE_Pressed, this, &ACharacterMain::Dodge);
}

void ACharacterMain::MoveForward(float Value) {
	if (Value != 0.f) {
		const FRotator Rotation = FollowCamera->GetComponentRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ACharacterMain::MoveRight(float Value) {
	if (Value != 0.f) {
		const FRotator Rotation = FollowCamera->GetComponentRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void ACharacterMain::GetCursorTrace() {
	auto PlayerController = Cast<APlayerControllerDShooter>(GetController());

	if (PlayerController) {
		FHitResult TraceHitResult;
		PlayerController->GetHitResultUnderCursor(ECC_GameTraceChannel1, false, TraceHitResult);
		FVector HitLocation = TraceHitResult.ImpactPoint;

		RotateCharacter(HitLocation);
	} 
}

void ACharacterMain::FireButtonPressed()
{
	SetPullingTrigger(true);
	PullTrigger();
}

void ACharacterMain::FireButtonReleased()
{
	SetPullingTrigger(false);
}

void ACharacterMain::ReloadPressed() {
	if ((GetEquippedWeapon()->GetCurrentAmmo() < GetEquippedWeapon()->GetAmmoCapacity()) 
		&& (GetCarryingAmmo() > 0)) {
		PlayReloadMontage();
	}
}

void ACharacterMain::InteractPressed() {

}

void ACharacterMain::PickupPressed() {

}

void ACharacterMain::OneKeyPressed() {
	UE_LOG(LogTemp, Warning, TEXT("1 pressed"));
}

void ACharacterMain::TwoKeyPressed() {
	UE_LOG(LogTemp, Warning, TEXT("2 pressed"));
}

void ACharacterMain::ThreeKeyPressed() {
	UE_LOG(LogTemp, Warning, TEXT("3 pressed"));
}
