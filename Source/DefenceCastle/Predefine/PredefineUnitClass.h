// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PredefineUnitClass.generated.h"

UENUM(BlueprintType)
enum class EUnitMovingType : uint8
{
	MoveFirst,//기본 상태, 따로 공격하려고 달려들지 않음.
	EnemyFirst //적이 보이면 달려듬.
};

USTRUCT(BlueprintType)
struct FSelectUnitInfoForUnitControlUI
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite)
	FString Name;
	UPROPERTY(BlueprintReadWrite)
	FString CurrentHp;
	UPROPERTY(BlueprintReadWrite)
	FString MovementSpeed;
};
