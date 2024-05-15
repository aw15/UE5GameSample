// Fill out your copyright notice in the Description page of Project Settings.


#include "DcGameInstance.h"

#include "DataManager.h"
#include "DefenceCastle/Pawn/UnitResource.h"
#include "Kismet/GameplayStatics.h"


DEFINE_DC_GET(UDcGameInstance)
{
	return Cast<UDcGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
}

void UDcGameInstance::Init()
{
	Super::Init();
}
