	// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AmmoBox.h"
#include "BasePawn.generated.h"

UCLASS()
class TANKS_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasePawn();

private:
	FTimerHandle DestroyTimerHandle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UArrowComponent* CannonSpawnPoint;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UHealthComponent* HealthComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UBoxComponent* HitCollider;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UArrowComponent* LootSpawnPoint;

	UPROPERTY(EditDefaultsOnly, Category = "Loot params")
	TSubclassOf<class AAmmoBox> LootClass;

	UFUNCTION(BlueprintNativeEvent, Category = "Health")
	void OnHealthChanged(float DamageAmount);

	UFUNCTION(BlueprintNativeEvent, Category = "Health")
	void OnDie();

	void Destroying();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DestroyingDelay")
	float DestroyingDelay = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret")
	TSubclassOf<class ACannon> DefaultCannonClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret")
	int MaxAmmo = 20;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret")
	int MaxCannons = 2;

	UPROPERTY()
	int CurrentCannonIndex = 0;

	UPROPERTY()
	bool bIsDestroyed = false;

	UPROPERTY()
	TArray<class ACannon*> Cannons;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UParticleSystemComponent* VisualEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UAudioComponent* AudioEffect;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Turret")
	void SetupCannon(TSubclassOf<class ACannon> InCannonClass, int AmmoAmount);

	UFUNCTION(BlueprintCallable, Category = "Turret")
	void Fire();

};