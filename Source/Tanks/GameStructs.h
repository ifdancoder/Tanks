#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "GameStructs.generated.h"

UENUM(BlueprintType)
enum class ECannonType : uint8
{ 
	FireProjectile = 0 UMETA(Display = "Use projectile"),
	FireTrace = 1 UMETA(Display = "Use trace")
};

USTRUCT(BlueprintType)
struct FDamageData
{
	GENERATED_BODY()

	UPROPERTY()
	float DamageValue;

	UPROPERTY()
	class AActor* Instigator;

	UPROPERTY()
	class AActor* DamageMaker;
};