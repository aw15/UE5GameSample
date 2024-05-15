// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DefenceCastle/Predefine/BaseEnumsAndInterface.h"
#include "DefenceCastle/Predefine/PredefineMacro.h"
#include "DefenceCastle/Predefine/PredefineUnitClass.h"
#include "State/StateManager.h"
#include "UnitAIController.generated.h"

class AUnit;
class AQuestTarget;
struct FUnitSpawnParam;
/**
 * 
 */
UCLASS()
class DEFENCECASTLE_API AUnitAIController : public AAIController, public IEngineEventListener
{
	GENERATED_BODY()

	friend class AUnit;
	friend class FStateMoving;
	friend class FStateAttacking;
	
public:
	AUnitAIController(FObjectInitializer const& ObjInitializer);
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	void OnUnitInitialized(const AUnit* InUnit, const FUnitSpawnParam& InUnitSpawnParams);
	UBlackboardComponent* GetBlackboard() const;

	//AI
	void MoveToLeftMouseButtonPosition();
	void MoveWithFormationToLeftMouseButtonPosition();
	void MoveToLastSelectedEnemy();
	void ResetAttackUnit();

	//Blackboard값입력
	void MoveToTargetLocation(const FVector& TargetLocation);
	void MoveToTargetActor(UObject* TargetActor);

	
	bool MoveToPriorityQuestLocation();
	void SetBtState(const int InState);
	void Stop();
	void AttackUnit(AActor* TargetActor);
	void UpdateMovingType(const EUnitMovingType InUnitMovingType);
	
	virtual void ProcessEngineEvent(const EActionEvent& EngineEvent) override;

	bool IsPlayerControlled() const { return bControlledByPlayer; }

	AUnit* GetNearestUnit() const;

private:
	void SearchEnemyTimerEvent();
	void OnAttackUnitDisappeared(AUnit* EnemyUnit);
	
protected:
	UPROPERTY()
	AUnit* ControlledUnit;
	//BT
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AI")
	class UBehaviorTree* DefaultBTRef;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AI")
	class UBehaviorTreeComponent* BehaviorTreeComponent;
	//Blackboard
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AI")
	UBlackboardComponent* BlackboardPtr;

public:
	bool bSelected = false;
	//int CurrentUnitState = STATE_NONE;
	//EUnitMovingType UnitMovingType = EUnitMovingType::MoveFirst; //일단 플레이어 유닛만 사용.

protected:
	UPROPERTY(VisibleAnywhere)
	TArray<class AQuestTarget*> QuestTargets;
	FTimerHandle SearchEnemyTimerHandle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Property")
	bool bControlledByPlayer = false;
	int OwnerIndex = PLAYER_INDEX; //Unit과 동일하게 유지될것. 편의상 controller에도 위치.

private:
	TArray<FVector> ConvertedQuestLocation;
	TWeakObjectPtr<AUnit> AttackTargetUnit;
	FStateManager StateManager;
};
