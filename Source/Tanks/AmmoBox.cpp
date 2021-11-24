// Fill out your copyright notice in the Description page of Project Settings.

#include "AmmoBox.h"
#include "Components/StaticMeshComponent.h"
#include "TankPawn.h"
#include "Cannon.h"

// Sets default values
AAmmoBox::AAmmoBox()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AAmmoBox::OnMeshOverlapBegin);
	Mesh->SetCollisionProfileName(FName("OverlapOnlyPawn"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetGenerateOverlapEvents(true);
}

void AAmmoBox::OnMeshOverlapBegin(class UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATankPawn* PlayerPawn = Cast<ATankPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (OtherActor == PlayerPawn)
	{
		ACannon* Cannon = PlayerPawn->GetActiveCannon();
		if (Cannon && Cannon->GetClass() == CannonClass)
		{
			Cannon->AddAmmo(NumAmmo);
		}
		else
		{
			PlayerPawn->SetupCannon(CannonClass);
		}
		Destroy();
	}
}