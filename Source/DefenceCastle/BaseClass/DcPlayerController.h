// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DefenceCastle/Predefine/BaseEnumsAndInterface.h"
#include "DefenceCastle/Predefine/PredefineMacro.h"
#include "DefenceCastle/Predefine/PredefineUnitClass.h"
#include "DcPlayerController.generated.h"


class ADcPlayer;
class APawn;
class UUnitSpawner;
class AUnit;

DECLARE_MULTICAST_DELEGATE(FOnUnselectUnit)

UCLASS()
class DEFENCECASTLE_API ADcPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADcPlayerController();
	DECLARE_DC_GET(ADcPlayerController)

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public: 
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void EdgeScrolling() const;
	virtual void SetupInputComponent() override;
	void CheckAndCleanupUnitParameter(AUnit* Unit);
	
	UUnitSpawner* GetUnitSpawner() const { return UnitSpawner; }

	//UI
	UFUNCTION(BlueprintCallable)
	void OnUIEventUpdateUnitMovingType(const EUnitMovingType UnitMovingType);

protected:
	void Zoom(const float Amount);
	void MoveVertical(const float Amount);
	void MoveHorizontal(const float Amount);
	void LeftMousePressed();
	void LeftMousePressedWithCtrl();
	void LeftMousePressedWithCtrlShift();//DragSelect
	void LeftMouseReleasedWithCtrlShift();//DragSelect
	void LeftMouseReleased();//DragSelect
	void RightMousePressed();
	UFUNCTION(BlueprintCallable)
	void SelectUnitOnHud(const TArray<AActor*>& SelectActors);

private:
	void ProcessSelectMultiple(AUnit* SelectedUnit);
	void ProcessSelectSingle(AUnit* SelectedUnit);
	void ProcessUnSelectAll();


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="System")
	UUnitSpawner* UnitSpawner;
	UPROPERTY(BlueprintReadWrite)
	TWeakObjectPtr<class ADcHud> CastedHud;//blueprint에서 할당
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUnitSelected, const FSelectUnitInfoForUnitControlUI&, SelectUnitInfoForUnitControlUI);
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite)
	FOnUnitSelected OnUnitSelected;

public:
	UPROPERTY(VisibleAnywhere, Category="Property")
	FVector LastRightMouseHitLocation;
	TMap<UObject*, FVector> LastRightMouseHitLocationWithFormation;
	UPROPERTY(VisibleAnywhere, Category="Property")
	FVector LastLeftMouseHitLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Property")
	TWeakObjectPtr<AUnit> LastSelectedUnit;
	UPROPERTY(VisibleAnywhere, Category="Property")
	TWeakObjectPtr<AActor> LastSelectedEnemyUnit;
	TSet<TWeakObjectPtr<AUnit>> MultipleSelectedUnits;
	
	FOnUnselectUnit OnUnselectUnit;

private:
	FSelectUnitInfoForUnitControlUI SelectUnitInfoForUnitControlUIData;
};
