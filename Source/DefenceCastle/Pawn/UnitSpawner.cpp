// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitSpawner.h"

#include "Unit.h"
#include "DefenceCastle/BaseClass/DcPlayerController.h"

UUnitSpawner::UUnitSpawner()
{
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;
}

DEFINE_DC_GET(UUnitSpawner)
{
	DC_GET_RET(ADcPlayerController, ControllerRef, WorldContextObject, nullptr)
	return ControllerRef->GetUnitSpawner();
}

bool UUnitSpawner::SpawnUnit(UClass* SpawnUnitClass, const FVector& Location, const FRotator& Rotation, const FUnitSpawnParam& UnitSpawnParam)
{
	if(!GetWorld())
		return false;
	
	AUnit* Unit = GetWorld()->SpawnActor<AUnit>(SpawnUnitClass, Location, Rotation, SpawnParameters);
	if(Unit)
	{
		Unit->Initialize(UnitSpawnParam);
		return true;
	}
	return false;
}
