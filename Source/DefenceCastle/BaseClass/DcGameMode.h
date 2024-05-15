// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DefenceCastle/Predefine/PredefineMacro.h"
#include "GameFramework/GameMode.h"
#include "DcGameMode.generated.h"

UCLASS()
class DEFENCECASTLE_API ADcGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	DECLARE_DC_GET(ADcGameMode);
	// Sets default values for this actor's properties
	ADcGameMode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void StartPlay() override;

public:
	class UWaveSystem* GetWaveSystem() const { return WaveSystem; }

protected:
	UPROPERTY(VisibleAnywhere)
	UWaveSystem* WaveSystem;
};
