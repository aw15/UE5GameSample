// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitResource.h"

#include "DefenceCastle/BaseClass/DcGameInstance.h"
#include "Engine/ObjectLibrary.h"

DEFINE_DC_GET(UUnitResource)
{
	DC_GET_RET(UDcGameInstance, GameInstance, WorldContextObject, nullptr)
	return GameInstance->UnitResource;
}

UUnitResource::UUnitResource()
{
}

void UUnitResource::Initialize()
{
}

TSoftObjectPtr<UAnimMontage> UUnitResource::GetUnitBasicAttackAnimMontage(const EUnitType UnitType)
{
	if(static_cast<int8>(UnitType) < UnitAnimMontage.Num())
	{
		return UnitAnimMontage[static_cast<int8>(UnitType)];
	}

	return NullObjectPtr;
}
