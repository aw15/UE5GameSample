// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DrawData.generated.h"

USTRUCT(BlueprintType)
struct FDataKeyProbability
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName DataKey;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Min{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Max{0};
};

USTRUCT(BlueprintType)
struct FDrawData : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FDataKeyProbability> Probabilities;
};
