// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DefenceCastle/Predefine/PredefineUnitClass.h"
#include "GameFramework/HUD.h"
#include "DcHud.generated.h"

UCLASS()
class DEFENCECASTLE_API ADcHud : public AHUD
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADcHud();
	UFUNCTION(BlueprintImplementableEvent)
	void DragStart();
	UFUNCTION(BlueprintImplementableEvent)
	void DragEnd();

	//유닛정보창
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateUnitControlPanel(const EUnitMovingType UnitMovingType);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintPure)
	void GetActorsInSelectionRectangleForUnit(const FVector2D& FirstPoint, const FVector2D& SecondPoint, TArray<AActor*>& OutActors, bool bActorMustBeFullyEnclosed);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(BlueprintReadWrite)
	bool bDragging = false;
};
