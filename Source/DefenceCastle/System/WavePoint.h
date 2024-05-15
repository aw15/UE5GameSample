// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DefenceCastle/Data/UnitData.h"
#include "DefenceCastle/Predefine/PredefineMacro.h"
#include "GameFramework/Actor.h"
#include "WavePoint.generated.h"

class AQuestTarget;
struct FUnitSpawnParam;

UCLASS()
class DEFENCECASTLE_API AWavePoint : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWavePoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void SpawnUnit();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Property")
	UClass* SpawnUnitClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Property")
	class UArrowComponent* SpawnBeginningPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Property")
	int SpawnRate = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Property")
	FUnitSpawnParam UnitSpawnParam;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Property")
	int Row = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Property")
	int Column = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Property")
	int RowInterval = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Property")
	int ColumnInterval = 100;
private:
	FTimerHandle SpawnTimerHandle;
};
