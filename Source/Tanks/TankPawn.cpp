// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPawn.h"
#include <Components/StaticMeshComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include "Math/UnrealMathUtility.h"
#include "Tanks.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ArrowComponent.h"
#include "Cannon.h"
#include "GameStructs.h"
#include "Components/BoxComponent.h"

// Sets default values
ATankPawn::ATankPawn()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring arm"));
	SpringArm->SetupAttachment(BaseMesh);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void ATankPawn::BeginPlay()
{
	Super::BeginPlay();

	Cannons[CurrentCannonIndex]->ScoreOnKill.AddDynamic(this, &ATankPawn::AddScoreForKill);
}

// Called every frame
void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentMoveForwardAxis = FMath::FInterpTo(CurrentMoveForwardAxis, TargetMoveForwardAxis, DeltaTime, MovementSmootheness);
	FVector MoveVector = GetActorForwardVector() * CurrentMoveForwardAxis;
	FVector NewActorLocation = GetActorLocation() + MoveVector * MoveSpeed * DeltaTime;
	SetActorLocation(NewActorLocation, true);

	CurrentRotateRightAxis = FMath::FInterpTo(CurrentRotateRightAxis, TargetRotateRightAxis, DeltaTime, RotationSmootheness);
	float Rotation = GetActorRotation().Yaw + CurrentRotateRightAxis * RotationSpeed * DeltaTime;
	SetActorRotation(FRotator(0.f, Rotation, 0.f));

	//UE_LOG(LogTanks, Verbose, TEXT("Ammo (this cannon): %d"), Cannons[CurrentCannonIndex]->GetAmmoNow());
	//UE_LOG(LogTanks, Verbose, TEXT("CurrentRotateRightAxis: %f"), CurrentRotateRightAxis);

	//UE_LOG(LogTanks, Verbose, TEXT("Active cannon: %d"), Cannons[CurrentCannonIndex]);
	//UE_LOG(LogTanks, Verbose, TEXT("Inactive cannon: %d"), InactiveCannon);

	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TurretTargetPosition);
	FRotator CurrentRotation = TurretMesh->GetComponentRotation();
	TargetRotation.Roll = CurrentRotation.Roll;
	TargetRotation.Pitch = CurrentRotation.Pitch;
	TurretMesh->SetWorldRotation(FMath::RInterpConstantTo(CurrentRotation, TargetRotation, DeltaTime, TurretRotationSmootheness));

	/*for (int i = 0; i < Cannons.Num(); i++)
	{
		UE_LOG(LogTanks, Verbose, TEXT("Cs%d: %s"), i, (*Cannons[0]).IsHidden() ? TEXT("true") : TEXT("false"));
	}*/
	UE_LOG(LogTanks, Log, TEXT("Score: %f"), GetCurrentScore());
}

void ATankPawn::TakeDamage(const FDamageData& DamageData)
{
	//UE_LOG(LogTanks, Log, TEXT("Tank taked damage"));
	HealthComponent->TakeDamage(DamageData);
}

void ATankPawn::MoveForward(float InAxisValue)
{
	TargetMoveForwardAxis = InAxisValue;
}

void ATankPawn::RotateRight(float InAxisValue)
{
	TargetRotateRightAxis = InAxisValue;
}

void ATankPawn::SetTurretTargetPosition(const FVector& TargetPosition)
{
	TurretTargetPosition = TargetPosition;
}

void ATankPawn::FireSpecial()
{
	if (Cannons[CurrentCannonIndex])
	{
		Cannons[CurrentCannonIndex]->FireSpecial();
	}
}

void ATankPawn::ChangingCannon()
{
	if (Cannons.Num() > 1)
	{
		//UE_LOG(LogTanks, Verbose, TEXT("Num: %d"), Cannons.Num());

		//UE_LOG(LogTanks, Verbose, TEXT("1st cannon isn't visible"), Cannons.Num());
		Cannons[CurrentCannonIndex]->SetVisibility(false);
		CurrentCannonIndex = (CurrentCannonIndex + 1) % Cannons.Num();
		//UE_LOG(LogTanks, Verbose, TEXT("2rd cannon is visible and active"), Cannons.Num());
		Cannons[CurrentCannonIndex]->SetVisibility(true);
	}

	//UE_LOG(LogTanks, Verbose, TEXT("Cannon now: %s"), *(Cannons[CurrentCannonIndex]->GetName()));
	/*UE_LOG(LogTanks, Verbose, TEXT("Cannons (%d):"), Cannons.Num());
	for (int i = 0; i < Cannons.Num(); i++)
	{
		UE_LOG(LogTanks, Verbose, TEXT("Cannon %s (%d): Ammo: %d"), *(Cannons[i]->GetName()), i + 1, Cannons[i]->GetAmmoNow());
		UE_LOG(LogTanks, Verbose, TEXT(""));
	}*/
}

class ACannon* ATankPawn::GetCannon() const
{
	return Cannons[CurrentCannonIndex];
}

TArray<ACannon*> ATankPawn::GetCannons() const
{
	return Cannons;
}

float ATankPawn::GetCurrentScore() const
{
	return CurrentScore;
}

void ATankPawn::AddScoreForKill(float Amount)
{
	CurrentScore += Amount;
}