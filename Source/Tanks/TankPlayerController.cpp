// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"
#include "TankPawn.h"

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	TankPawn = Cast<ATankPawn>(GetPawn());
}

void ATankPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward", this, &ATankPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ATankPlayerController::MoveRight);
}

void ATankPlayerController::MoveForward(float InAxisValue)
{
	if (TankPawn) 
	{
		TankPawn->MoveForward(InAxisValue);
	}
}

void ATankPlayerController::MoveRight(float InAxisValue)
{
	if (TankPawn)
	{
		TankPawn->MoveRight(InAxisValue);
	}
}