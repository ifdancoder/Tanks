// Fill out your copyright notice in the Description page of Project Settings.

#include "Cannon.h"
#include "Tanks.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Projectile.h"
#include "DrawDebugHelpers.h"
#include "Logging/LogMacros.h"
#include "ActorPoolSubsystem.h"

// Sets default values
ACannon::ACannon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceeneCpm = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceeneCpm;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon mesh"));
	Mesh->SetupAttachment(RootComponent);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawn point"));
	ProjectileSpawnPoint->SetupAttachment(Mesh);

	ShotsLeft = 0, AmmoNow = 0;
}

void ACannon::Fire()
{
	if (!bIsReadyToFire)
	{
		return;
	}

	if (AmmoNow)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Yellow, TEXT("Ordinary fire started"));
		--AmmoNow;
	}

	ShotsLeft = 1;
	Shot();
}

void ACannon::FireSpecial()
{
	if (!bIsReadyToFire)
	{
		return;
	}

	if (AmmoNow)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Yellow, TEXT("Rapid fire started"));
		--AmmoNow;
	}

	ShotsLeft = SerialShots;
	Shot();
}

int ACannon::GetAmmoNow()
{
	return AmmoNow;
}

void ACannon::Shot()
{
	bIsReadyToFire = false;
	if(AmmoNow)
	{
		if (Type == ECannonType::FireProjectile)
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Green, TEXT("Fire - projectile"));

			UActorPoolSubsystem* Pool = GetWorld()->GetSubsystem<UActorPoolSubsystem>();
			FTransform SpawnTransform(ProjectileSpawnPoint->GetComponentRotation(), ProjectileSpawnPoint->GetComponentLocation(), FVector::OneVector);
			AProjectile* Projectile = Cast<AProjectile>(Pool->RetreiveActor(ProjectileClass, SpawnTransform));

			if (Projectile)
			{
				Projectile->Start();
			}
		}
		else if (Type == ECannonType::FireTrace)
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Green, TEXT("Fire - trace"));

			FHitResult HitResult;
			FVector TraceStart = ProjectileSpawnPoint->GetComponentLocation();
			FVector TraceEnd = ProjectileSpawnPoint->GetComponentLocation() + ProjectileSpawnPoint->GetForwardVector() * FireRange;
			FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
			TraceParams.bReturnPhysicalMaterial = false;
			if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, TraceParams))
			{
				DrawDebugLine(GetWorld(), TraceStart, HitResult.Location, FColor::Green, false, 0.5f, 0, 5.f);
				if (HitResult.Actor.IsValid() && HitResult.Component.IsValid(), HitResult.Component->GetCollisionObjectType() == ECC_Destructible)
				{
					HitResult.Actor->Destroy();
				}
			}
			else
			{
				DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 0.5f, 0, 5.f);
			}
		}

		if (--ShotsLeft)
		{
			GetWorld()->GetTimerManager().SetTimer(SeriesTimerHandle, this, &ACannon::Shot, TimeToReloadSeries, false);
		}
		else
		{
			GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, TimeToReloadSeries, false);
		}
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, TimeToReloadSeries, false);
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Red, TEXT("Not enough ammo"));
	}
}

bool ACannon::IsReadyToFire()
{
	return bIsReadyToFire;
}

void ACannon::SetVisibility(bool bIsVisible)
{
	Mesh->SetHiddenInGame(!bIsVisible);
}

void ACannon::AddAmmo(int InNumAmmo)
{
	AmmoNow = FMath::Clamp(AmmoNow + InNumAmmo, 0, Ammo);
	UE_LOG(LogTanks, Log, TEXT("AddAmmo(%d)! NumAmmo: %d"), InNumAmmo, AmmoNow);
}

// Called when the game starts or when spawned
void ACannon::BeginPlay()
{
	Super::BeginPlay();

	bIsReadyToFire = true;
	AmmoNow = Ammo;
}

void ACannon::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(SeriesTimerHandle);
}

void ACannon::Reload()
{
	bIsReadyToFire = true;
}