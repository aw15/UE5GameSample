// Fill out your copyright notice in the Description page of Project Settings.


#include "WavePoint.h"
#include "WaveSystem.h"
#include "../Pawn/UnitSpawner.h"
#include "Components/ArrowComponent.h"


// Sets default values
AWavePoint::AWavePoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SpawnBeginningPoint = CreateDefaultSubobject<UArrowComponent>("SpawnBeginningPoint");
}

// Called when the game starts or when spawned
void AWavePoint::BeginPlay()
{
	Super::BeginPlay();
	DC_GET(UWaveSystem, WaveSystem, GetWorld())
	WaveSystem->AddPoint(this);
	

	if(GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this,  &AWavePoint::SpawnUnit, SpawnRate, true);
	}

	SpawnUnit(); //TODO 먼저 한번 생성
}

void AWavePoint::SpawnUnit()
{
	DC_GET(UUnitSpawner, UnitSpawner, GetWorld())
	for(int I =0 ; I < Row ; ++I)
	{
		for(int J = 0; J < Column ; ++J)
		{
			UnitSpawner->SpawnUnit(SpawnUnitClass, SpawnBeginningPoint->GetComponentLocation(), SpawnBeginningPoint->GetComponentRotation(), UnitSpawnParam);
		}
	}
}

