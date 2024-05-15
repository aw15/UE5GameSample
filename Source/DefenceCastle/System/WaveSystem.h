// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DefenceCastle/Predefine/PredefineMacro.h"
#include "WaveSystem.generated.h"


class AWavePoint;
/**
 *  적 웨이브를 담당하는 매니저 클래스
 */
UCLASS()
class DEFENCECASTLE_API UWaveSystem final : public UObject
{
	GENERATED_BODY()
public:
	DECLARE_DC_GET(UWaveSystem);
	UWaveSystem();

public:
	void AddPoint(AWavePoint* WavePoint);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<AWavePoint*> SpawnPoints;
};
