// Fill out your copyright notice in the Description page of Project Settings.


#include "DcGameMode.h"

#include "DefenceCastle/System/WaveSystem.h"
#include "Kismet/GameplayStatics.h"

DEFINE_DC_GET(ADcGameMode)
{
	return Cast<ADcGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
}

ADcGameMode::ADcGameMode()
{
}

// Called when the game starts or when spawned
void ADcGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ADcGameMode::StartPlay()
{
	WaveSystem = NewObject<UWaveSystem>();
	Super::StartPlay();
}
