// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

/**
 *
 */
UCLASS()
class TANKS_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	// Called to bind functionality to input
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaSeconds) override;

private:
	bool bIsMouse = true;
	float HorisontalAxis = 0;
	float VerticalAxis = 0;

	void TurretForward(float InAxisValue);
	void TurretRight(float InAxisValue);
	void MoveForward(float InAxisValue);
	void RotateRight(float InAxisValue);
	void Fire();
	void FireSpecial();
	void ChangingCannon();
	void ChangingMethod();

	UPROPERTY()
		class ATankPawn* TankPawn;

	FVector TargetTurretDirection;
};