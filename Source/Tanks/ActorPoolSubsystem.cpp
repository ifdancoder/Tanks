// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorPoolSubsystem.h"
#include "Logging/LogMacros.h"
#include "Tanks.h"

AActor* UActorPoolSubsystem::RetreiveActor(UClass* Class, const FTransform& Transform)
{
	if (!Class)
	{
		return nullptr;
	}

	TArray<AActor*>* Pool = InactiveActors.Find(Class);
	AActor* Result = nullptr;

	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Yellow, TEXT("Retrieving the actor from the pool"));

	if (Pool && Pool->Num() > 0)
	{
		Result = Pool->Pop();
		FVector CorrectedScale = Transform.GetScale3D() * Result->GetActorScale();
		Result->SetActorTransform(FTransform(Transform.GetRotation(), Transform.GetLocation(), CorrectedScale), false, nullptr, ETeleportType::ResetPhysics);
		Result->SetActorTickEnabled(true);
	}
	else
	{
		Result = GetWorld()->SpawnActor(Class, &Transform);
		if (!Result)
		{
			return nullptr;
		}

		Actors.Add(Result);
		Result->OnDestroyed.AddDynamic(this, &UActorPoolSubsystem::OnActorDestroyed);
	}

	return Result;
}

void UActorPoolSubsystem::ReturnActor(AActor* Actor)
{
	if (!Actor || !IsActorInPool(Actor))
	{
		return;
	}

	TArray<AActor*>& Pool = InactiveActors.FindOrAdd(Actor->GetClass());

	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Yellow, TEXT("Returning the actor to the pool"));

	if (!Pool.Contains(Actor))
	{
		Actor->SetActorTickEnabled(false);
		Pool.Add(Actor);
	}
}

bool UActorPoolSubsystem::IsActorInPool(AActor* Actor) const
{
	return Actors.Contains(Actor);
}

void UActorPoolSubsystem::OnActorDestroyed(AActor* Actor)
{
	check(IsActorInPool(Actor));
	TArray<AActor*>* Pool = InactiveActors.Find(Actor->GetClass());
	if (Pool && Pool->Contains(Actor))
	{
		Pool->Remove(Actor);
	}

	Actors.Remove(Actor);
}