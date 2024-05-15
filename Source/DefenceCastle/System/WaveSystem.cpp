// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveSystem.h"

#include "DefenceCastle/BaseClass/DcGameMode.h"

DEFINE_DC_GET(UWaveSystem)
{
	DC_GET_RET(ADcGameMode, GameModePtr, WorldContextObject, nullptr)
	return GameModePtr->GetWaveSystem();
}

UWaveSystem::UWaveSystem()
{
}

void UWaveSystem::AddPoint(AWavePoint* WavePoint)
{
	SpawnPoints.Add(WavePoint);
}
