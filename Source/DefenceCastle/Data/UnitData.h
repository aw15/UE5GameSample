// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DefenceCastle/Predefine/PredefineMacro.h"
#include "Engine/DataTable.h"
#include "UnitData.generated.h"

/**
 * 
 */

class AQuestTarget;

UENUM(BlueprintType)
enum class ESkillType : uint8
{
	Melee,	
};

USTRUCT()
struct FSkillData : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESkillType SkillType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int64 Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int64 Heal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackCoolTime = 1;
};

UENUM(BlueprintType)
enum class EUnitType : uint8
{
	Dwarf_Infantry = 0
};

USTRUCT()
struct FUnitData : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName BasicSKillDataKey;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SkillDataKey1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* WoodMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* BagMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* QuiverMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* ShieldMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* WeaponRMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* WeaponLMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* ShoulderPadMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* LegMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* ArmMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* BodyMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* HeadMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovementSpeed = 600.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Hp = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EUnitType UnitType = EUnitType::Dwarf_Infantry;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;
};

USTRUCT(BlueprintType)
struct FUnitSpawnParam
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AQuestTarget*> QuestTargets;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName UnitDataKey;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int OwnerIndex = PLAYER_INDEX;
};
