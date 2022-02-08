// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include <Components/ArrowComponent.h>
#include "HealthComponent.h"
#include <Components/BoxComponent.h>
#include "Tanks.h"
#include "Cannon.h"
#include <Components/StaticMeshComponent.h>
#include "Logging/LogMacros.h"
#include <Particles/ParticleSystemComponent.h>
#include <Components/AudioComponent.h>
#include "ActorPoolSubsystem.h"

// Sets default values
ABasePawn::ABasePawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TankMesh"));
	SetRootComponent(BaseMesh);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	CannonSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("CannonSpawn"));
	CannonSpawnPoint->SetupAttachment(TurretMesh);

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("HitCollider"));
	HitCollider->SetupAttachment(RootComponent);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->OnDie.AddDynamic(this, &ABasePawn::OnDie);
	HealthComponent->OnHealthChanged.AddDynamic(this, &ABasePawn::OnHealthChanged);

	VisualEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Shoot Effect"));
	VisualEffect->SetupAttachment(BaseMesh);

	AudioEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Effect"));
	AudioEffect->SetupAttachment(BaseMesh);

	LootSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawn point"));
	LootSpawnPoint->SetupAttachment(BaseMesh);
}

// Called when the game starts or when spawned
void ABasePawn::BeginPlay()
{
	Super::BeginPlay();
	SetupCannon(DefaultCannonClass, MaxAmmo);
	CurrentCannonIndex = 0;
}

// Called every frame
void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABasePawn::OnHealthChanged_Implementation(float DamageAmount)
{
	UE_LOG(LogTemp, Warning, TEXT("%s taked damage: %f"), *GetName(), DamageAmount);
}

void ABasePawn::OnDie_Implementation()
{
	if (!bIsDestroyed)
	{
		VisualEffect->ActivateSystem();
		AudioEffect->Play();

		UActorPoolSubsystem* Pool = GetWorld()->GetSubsystem<UActorPoolSubsystem>();
		FTransform SpawnTransform(LootSpawnPoint->GetComponentRotation(), LootSpawnPoint->GetComponentLocation(), FVector::OneVector);
		AAmmoBox* AmmoBox = Cast<AAmmoBox>(Pool->RetreiveActor(LootClass, SpawnTransform));

		GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &ABasePawn::Destroying, DestroyingDelay, false);

		bIsDestroyed = true;
	}
}

void ABasePawn::Destroying()
{
	Destroy();
	for (const auto Cannon : Cannons)
	{
		Cannon->Destroy();
	}
}

void ABasePawn::Fire()
{
	if (!Cannons[CurrentCannonIndex]) return;

	Cannons[CurrentCannonIndex]->Fire();
}

void ABasePawn::SetupCannon(TSubclassOf<class ACannon> InCannonClass, int AmmoAmount)
{
	if (InCannonClass)
	{

		ACannon* Cannon = nullptr;

		FActorSpawnParameters Params;
		Params.Instigator = this;
		Params.Owner = this;

		Cannon = GetWorld()->SpawnActor <ACannon>(InCannonClass, Params);
		//UE_LOG(LogTanks, Verbose, TEXT("Spawned new cannon: %s"), *(Cannon->GetName()));

		if (!Cannon)
		{
			return;
		}

		Cannon->AttachToComponent(CannonSpawnPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		Cannon->AddAmmo(AmmoAmount);

		bool IsThereNotCannon = !(Cannons.Find(Cannon) + 1);

		if (Cannons.Num() <= MaxCannons || IsThereNotCannon)
		{
			Cannon->SetAmmo(MaxAmmo);
			if (Cannons.Num() == MaxCannons && IsThereNotCannon)
			{
				Cannons[CurrentCannonIndex]->Destroy();
				Cannons.RemoveAt(CurrentCannonIndex);
				Cannons.Insert(Cannon, CurrentCannonIndex);
			}
			else
			{
				if (Cannons.Num())
				{
					Cannons[CurrentCannonIndex]->SetVisibility(false);
				}
				Cannons.Add(Cannon);
				CurrentCannonIndex = (CurrentCannonIndex + 1) % MaxCannons;
			}
			
			//UE_LOG(LogTanks, Verbose, TEXT("Added in cannons new cannon: %s. Ind: %d"), *(Cannon->GetName()), CurrentCannonIndex);
		}
		//UE_LOG(LogTanks, Verbose, TEXT("Cannons[CurrentCannonIndex] now: %s"), *(Cannons[CurrentCannonIndex]->GetName()));
	}
}