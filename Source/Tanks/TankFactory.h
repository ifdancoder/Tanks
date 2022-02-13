// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Damageable.h"
#include "TankPawn.h"
#include <Engine/TargetPoint.h>
#include "MapLoader.h"
#include <Particles/ParticleSystemComponent.h>
#include <Components/AudioComponent.h>
#include "TankFactory.generated.h"

UCLASS()
class TANKS_API ATankFactory : public AActor, public IDamageable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATankFactory();

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* BuildingMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* BuildingMeshDestroyed;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UArrowComponent* TankSpawnPoint;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UBoxComponent* HitCollider;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UHealthComponent* HealthComponent;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn tanks params")
	TSubclassOf<ATankPawn> SpawnTankClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawn tanks params")
	float SpawnTankRate = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Maximum number of tanks")
	int MaxTanks = 2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Destroyed params")
	bool bIsDestroyed = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn tanks params")
	TArray<ATargetPoint*> TankWayPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn tanks params")
	class AMapLoader* MapLoader;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UParticleSystemComponent* SpawnTankVisualEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UAudioComponent* SpawnTankAudioEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UParticleSystemComponent* DestroyingVisualEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UAudioComponent* DestroyingAudioEffect;

	virtual void BeginPlay() override;

	UFUNCTION()
	void SpawnNewTank();

	UFUNCTION()
	void CustomSpawning();

	UFUNCTION()
	void Die();

	UFUNCTION()
	void TakeDamage(const FDamageData& DamageData) override;

	UFUNCTION()
	void DamageTaked(float DamageValue);

	UFUNCTION()
	void Destroying();

private:
	FTimerHandle SpawnTankTimerHandle;
	FTimerHandle CustomSpawnTimerHandle;
};
