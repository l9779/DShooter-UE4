// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterDShooter.h"
#include "CharacterMain.generated.h"

UCLASS()
class DSHOOTER_API ACharacterMain : public ACharacterDShooter
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

protected:
	ACharacterMain();

	virtual void Tick(float DeltaTime) override;

	void MoveForward(float Value);
	void MoveRight(float Value);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void ReloadPressed();

	void InteractPressed();
	void PickupPressed();

	void OneKeyPressed();
	void TwoKeyPressed();
	void ThreeKeyPressed();

	void GetCursorTrace();

	void FireButtonPressed();
	void FireButtonReleased();
private:

public:

};
