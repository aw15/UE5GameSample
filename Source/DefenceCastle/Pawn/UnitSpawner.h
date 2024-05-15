// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DefenceCastle/Predefine/PredefineMacro.h"
#include "UObject/Object.h"
#include "UnitSpawner.generated.h"

struct FUnitSpawnParam;
/**
 * 
 */
UCLASS()
class DEFENCECASTLE_API UUnitSpawner final : public UObject
{
	GENERATED_BODY()
public:
	UUnitSpawner();
	DECLARE_DC_GET(UUnitSpawner);
	bool SpawnUnit(UClass* SpawnUnitClass, const FVector& Location, const FRotator& Rotation, const FUnitSpawnParam& UnitSpawnParam);

private:
	FActorSpawnParameters SpawnParameters;
};
