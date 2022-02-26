// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "TankPawn.h"
#include "DrawDebugHelpers.h"

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	TankPawn = Cast<ATankPawn>(GetPawn());
	bShowMouseCursor = true;

	TargetTurretDirection.X = 0;
	TargetTurretDirection.Y = 1;
}

void ATankPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward", this, &ATankPlayerController::MoveForward);
	InputComponent->BindAxis("RotateRight", this, &ATankPlayerController::RotateRight);

	InputComponent->BindAxis("TurretForward", this, &ATankPlayerController::TurretForward);
	InputComponent->BindAxis("TurretRight", this, &ATankPlayerController::TurretRight);

	InputComponent->BindAction("Fire", IE_Pressed, this, &ATankPlayerController::Fire);
	InputComponent->BindAction("FireSpecial", IE_Pressed, this, &ATankPlayerController::FireSpecial);
	InputComponent->BindAction("ChangingCannon", IE_Pressed, this, &ATankPlayerController::ChangingCannon);
	InputComponent->BindAction("ChangingMethod", IE_Pressed, this, &ATankPlayerController::ChangingMethod);
}

void ATankPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!TankPawn)
	{
		return;
	}

	if (bIsMouse)
	{
		FVector WorldMousePosition, WorldMouseDirection;
		DeprojectMousePositionToWorld(WorldMousePosition, WorldMouseDirection);

		TargetTurretDirection = WorldMousePosition - TankPawn->GetActorLocation();
	}
	else
	{
		if (HorisontalAxis || VerticalAxis)
		{
			TargetTurretDirection.X = HorisontalAxis;
			TargetTurretDirection.Y = VerticalAxis;
		}
	}

	TargetTurretDirection.Z = 0.f;
	TargetTurretDirection.Normalize();

	//UE_LOG(LogTemp, Warning, TEXT("%f %f %f"), TargetTurretDirection.X, TargetTurretDirection.Y, TargetTurretDirection.Z);

	FVector TurretTargetPosition = TankPawn->GetActorLocation() + TargetTurretDirection * 1000.f;

	//DrawDebugLine(GetWorld(), TankPawn->GetActorLocation(), TurretTargetPosition, FColor::Green, false, 0.1f, 0, 5.f);

	TankPawn->SetTurretTargetPosition(TurretTargetPosition);
}

void ATankPlayerController::TurretForward(float InAxisValue)
{
	if (TankPawn && !bIsMouse)
	{
		VerticalAxis = InAxisValue;
	}
}

void ATankPlayerController::TurretRight(float InAxisValue)
{
	if (TankPawn && !bIsMouse)
	{
		HorisontalAxis = InAxisValue;
	}
}

void ATankPlayerController::MoveForward(float InAxisValue)
{
	if (TankPawn)
	{
		TankPawn->MoveForward(InAxisValue);
	}
}

void ATankPlayerController::RotateRight(float InAxisValue)
{
	if (TankPawn)
	{
		TankPawn->RotateRight(InAxisValue);
	}
}

void ATankPlayerController::Fire()
{
	if (TankPawn)
	{
		TankPawn->Fire();
	}
}

void ATankPlayerController::FireSpecial()
{
	if (TankPawn)
	{
		TankPawn->FireSpecial();
	}
}

void ATankPlayerController::ChangingCannon()
{
	if (TankPawn)
	{
		TankPawn->ChangingCannon();
	}
}

void ATankPlayerController::ChangingMethod()
{
	if (TankPawn)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Method was changed"));
		bIsMouse = !bIsMouse;
	}
}
