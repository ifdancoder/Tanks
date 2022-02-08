// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameStructs.h"
#include <Camera/CameraShakeBase.h>
#include "Cannon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FScoreOnKill, float, Amount);

UCLASS()
class TANKS_API ACannon : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UArrowComponent* ProjectileSpawnPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float FireRate = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (EditCondition = "Type == ECannonType::FireTrace", EditConditionHides), Category = "Fire params")
	float FireRange = 1000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	int Ammo = 30;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = 1), Category = "Fire params")
	int SerialShots = 4;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (EditCondition = "SerialShots > 1", EditConditionHides), Category = "Fire params")
	float TimeToReloadSeries = 0.25;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
	float Damage = 1.f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UParticleSystemComponent* ShootEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UAudioComponent* AudioEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	class UForceFeedbackEffect* ShootForceEffect;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCameraShakeBase> ShootShake;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	ECannonType Type = ECannonType::FireProjectile;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (EditCondition = "Type == ECannonType::FireProjectile", EditConditionHides), Category = "Fire params")
	TSubclassOf<class AProjectile> ProjectileClass;
private:
	FTimerHandle ReloadTimerHandle;
	FTimerHandle SeriesTimerHandle;

	bool bIsReadyToFire = false;

public:
	ACannon();

	UPROPERTY()
	FScoreOnKill ScoreOnKill;

	UFUNCTION()
	void GetScoreOnKill(float Amount);

	void Fire();
	void FireSpecial();
	int GetAmmoNow();
	void Shot();
	bool IsReadyToFire();
	void SetVisibility(bool bIsVisible);
	void AddAmmo(int InNumAmmo);
	void SetAmmo(int NumAmmo);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	void Reload();

	int ShotsLeft;
	int AmmoNow;
};