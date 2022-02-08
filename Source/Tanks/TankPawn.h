// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameStructs.h"
#include "Damageable.h"
#include "HealthComponent.h"
#include "BasePawn.h"
#include "TankPawn.generated.h"

UCLASS()
class TANKS_API ATankPawn : public ABasePawn, public IDamageable
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATankPawn();

protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	float MoveSpeed = 1000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	float RotationSpeed = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float MovementSmootheness = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float RotationSmootheness = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret")
	float TurretRotationSmootheness = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Move params|Patrol points", Meta = (MakeEditWidget = true))
	TArray<FVector> PatrollingPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Move params|Accurency")
	float MovementAccuracy = 50;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void TakeDamage(const FDamageData& DamageData) override;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MoveForward(float InAxisValue);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void RotateRight(float InAxisValue);

	UFUNCTION(BlueprintCallable, Category = "Turret")
	void SetTurretTargetPosition(const FVector& TargetPosition);

	UFUNCTION(BlueprintCallable, Category = "Turret")
	void FireSpecial();

	UFUNCTION(BlueprintCallable, Category = "Turret")
	void ChangingCannon();

	UFUNCTION(BlueprintPure, Category = "Turret")
	class ACannon* GetCannon() const;

	UFUNCTION(BlueprintPure, Category = "Turret")
	FVector GetTurretForwardVector();

	UFUNCTION(BlueprintCallable, Category = "Turret")
	TArray<ACannon*> GetCannons() const;

	UFUNCTION(BlueprintCallable, Category = "Scoring")
	void AddScoreForKill(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Scoring")
	float GetCurrentScore() const;

	UFUNCTION(BlueprintPure, Category = "AI|MoveParams")
	TArray<FVector> GetPatrollingPoints()
	{
		return PatrollingPoints;
	}

	UFUNCTION(BlueprintPure, Category = "AI|MoveParams")
	float GetMovementAccuracy()
	{
		return MovementAccuracy;
	}


private:

	float CurrentMoveForwardAxis = 0.f;
	float TargetMoveForwardAxis = 0.f;
	float CurrentRotateRightAxis = 0.f;
	float TargetRotateRightAxis = 0.f;

	float CurrentScore = 0.f;

	FVector TurretTargetPosition;
};