// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DefenceCastle/Predefine/PredefineMacro.h"
#include "Engine/StreamableManager.h"
#include "DcGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class DEFENCECASTLE_API UDcGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	DECLARE_DC_GET(UDcGameInstance)
	virtual void Init() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UDataManager* DataManager;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UUnitResource* UnitResource;
	FStreamableManager StreamableManager;
};