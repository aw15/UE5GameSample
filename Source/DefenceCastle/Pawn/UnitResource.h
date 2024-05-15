// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DefenceCastle/Data/UnitData.h"
#include "DefenceCastle/Predefine/PredefineMacro.h"
#include "UObject/Object.h"
#include "UnitResource.generated.h"

class UAnimMontage;
/**
 * 
 */
UCLASS(Blueprintable)
class DEFENCECASTLE_API UUnitResource : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_DC_GET(UUnitResource)
	UUnitResource();
	UFUNCTION(BlueprintCallable)
	void Initialize();
	TSoftObjectPtr<UAnimMontage> GetUnitBasicAttackAnimMontage(const EUnitType UnitType);

protected:
	UPROPERTY(EditAnywhere)
	TArray<TSoftObjectPtr<UAnimMontage>> UnitAnimMontage;

	TSoftObjectPtr<UAnimMontage> NullObjectPtr;
};
