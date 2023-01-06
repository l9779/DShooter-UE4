// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimInstanceMain.h"
#include "Kismet/KismetMathLibrary.h"
#include "CharacterDShooter.h"
#include "GameFramework/CharacterMovementComponent.h"

UAnimInstanceMain::UAnimInstanceMain(): 
bIsAcelerating(false), Speed(0.f),
MovementOffsetYaw(0.f), LastMovementOffsetYaw(0.f),
TIPCharacterYaw(0.f), TIPCharacterYawLastFrame(0.f), RootYawOffset(0.f)
{ }

void UAnimInstanceMain::NativeInitializeAnimation() {
	Character = Cast<ACharacterDShooter>(TryGetPawnOwner());
}

void UAnimInstanceMain::UpdateAnimationProperties(float DeltaTime){
	if (!Character) {
		Character = Cast<ACharacterDShooter>(TryGetPawnOwner());
		if (!Character) return;
	}

	FVector Velocity{ Character->GetVelocity() };
	Velocity.Z = 0;
	Speed = Velocity.Size();

	if (Character->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f) {
		bIsAcelerating = true;
	}
	else {
		bIsAcelerating = false;
	}

	const FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(Character->GetVelocity());
	const FRotator AimRotation = Character->GetActorRotation();
	MovementOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation).Yaw;

	//FString ScreenMessage = FString::Printf(TEXT("%f"), MovementOffsetYaw);
	//if (GEngine) { GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::Red, ScreenMessage); }

	if (Character->GetVelocity().Size() > 0.f) {
		LastMovementOffsetYaw = MovementOffsetYaw;
	}

	TurnInPlace();
}

void UAnimInstanceMain::TurnInPlace() {
	if (!Character) return;

	auto Pitch = Character->GetBaseAimRotation().Pitch;

	if (Speed > 0) {
		RootYawOffset = 0;
		TIPCharacterYaw = Character->GetActorRotation().Yaw;
		TIPCharacterYawLastFrame = TIPCharacterYaw;
	}
	else {
		TIPCharacterYawLastFrame = TIPCharacterYaw;
		TIPCharacterYaw = Character->GetActorRotation().Yaw;
		const float TIPYawDelta{ TIPCharacterYaw - TIPCharacterYawLastFrame };

		RootYawOffset = UKismetMathLibrary::NormalizeAxis(RootYawOffset - TIPYawDelta);
	}
}