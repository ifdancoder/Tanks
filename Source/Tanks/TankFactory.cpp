// Fill out your copyright notice in the Description page of Project Settings.


#include "TankFactory.h"
#include <Components/ArrowComponent.h>
#include <Components/BoxComponent.h>
#include "HealthComponent.h"
#include "Tanks.h"

// Sets default values
ATankFactory::ATankFactory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = sceneComp;

	BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Building Mesh"));
	BuildingMesh->SetupAttachment(sceneComp);

	BuildingMeshDestroyed = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Building Mesh Destroyed"));
	BuildingMeshDestroyed->SetupAttachment(sceneComp);

	TankSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon setup point"));
	TankSpawnPoint->SetupAttachment(sceneComp);

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
	HitCollider->SetupAttachment(sceneComp);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health component"));
	HealthComponent->OnDie.AddDynamic(this, &ATankFactory::Die);
	HealthComponent->OnHealthChanged.AddDynamic(this, &ATankFactory::DamageTaked);

	SpawnTankVisualEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Spawn Tank Visual Effect"));
	SpawnTankVisualEffect->SetupAttachment(TankSpawnPoint);

	SpawnTankAudioEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("Spawn Tank Audio Effect"));
	SpawnTankAudioEffect->SetupAttachment(TankSpawnPoint);

	DestroyingVisualEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Destroying Factory Visual Effect"));
	DestroyingVisualEffect->SetupAttachment(BuildingMesh);

	DestroyingAudioEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("Destroying Factory Audio Effect"));
	DestroyingAudioEffect->SetupAttachment(BuildingMesh);

}

// Called when the game starts or when spawned
void ATankFactory::BeginPlay()
{
	Super::BeginPlay();

	SpawnTankVisualEffect->DeactivateSystem();
	GetWorld()->GetTimerManager().SetTimer(CustomSpawnTimerHandle, this, &ATankFactory::CustomSpawning, SpawnTankRate, false);
	
	BuildingMeshDestroyed->SetHiddenInGame(true);
}

void ATankFactory::SpawnNewTank()
{
	SpawnTankAudioEffect->Play();
	FTransform SpawnTransform(TankSpawnPoint->GetComponentRotation(), TankSpawnPoint->GetComponentLocation(), FVector(1.f));
	ATankPawn* NewTank = GetWorld()->SpawnActorDeferred<ATankPawn>(SpawnTankClass, SpawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	NewTank->SetPatrollingPoints(TankWayPoints);

	NewTank->FinishSpawning(SpawnTransform);
	SpawnTankVisualEffect->DeactivateSystem();
}

void ATankFactory::CustomSpawning()
{
	if (!MaxTanks && !bIsDestroyed)
	{
		return;
	}

	SpawnTankVisualEffect->ActivateSystem();

	MaxTanks--;
	GetWorld()->GetTimerManager().SetTimer(SpawnTankTimerHandle, this, &ATankFactory::SpawnNewTank, SpawnTankRate / 2, false);

	GetWorld()->GetTimerManager().SetTimer(CustomSpawnTimerHandle, this, &ATankFactory::CustomSpawning, SpawnTankRate, false);
}

void ATankFactory::Die()
{
	if (MapLoader)
	{
		MapLoader->SetActivated(true);
	}
	Destroying();
}

void ATankFactory::TakeDamage(const FDamageData& DamageData)
{
	HealthComponent->TakeDamage(DamageData);
}

void ATankFactory::DamageTaked(float DamageValue)
{
	UE_LOG(LogTanks, Log, TEXT("Factory %s has taken damage: %f"), *GetName(), DamageValue);
}

void ATankFactory::Destroying()
{
	if (!bIsDestroyed)
	{
		bIsDestroyed = true;
		DestroyingVisualEffect->ActivateSystem();
		DestroyingAudioEffect->Play();
		SpawnTankVisualEffect->DeactivateSystem();
		BuildingMesh->SetHiddenInGame(true);
		BuildingMeshDestroyed->SetHiddenInGame(false);
		HitCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetWorld()->GetTimerManager().ClearTimer(SpawnTankTimerHandle);
		GetWorld()->GetTimerManager().ClearTimer(CustomSpawnTimerHandle);
	}
}

