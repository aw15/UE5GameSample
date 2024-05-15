// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitAIController.h"

#include "Unit.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DefenceCastle/BaseClass/DataManager.h"
#include "DefenceCastle/Predefine/PredefineMacro.h"
#include "DefenceCastle/BaseClass/DcPlayerController.h"
#include "DefenceCastle/System/QuestTarget.h"
#include "State/StateBase.h"

AUnitAIController::AUnitAIController(FObjectInitializer const& ObjInitializer)
{
	BehaviorTreeComponent = ObjInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorTreeComponent"));
	BlackboardPtr = ObjInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("DefaultBlackboard"));
}

void AUnitAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AUnitAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if(DefaultBTRef)
	{
		if(BlackboardPtr) 
		{
			BlackboardPtr->InitializeBlackboard(*DefaultBTRef->BlackboardAsset);
		}
		RunBehaviorTree(DefaultBTRef);
		BehaviorTreeComponent->StartTree(*DefaultBTRef);
	}

	if(AUnit* UnitPtr = Cast<AUnit>(InPawn))
	{
		ControlledUnit = UnitPtr;
	}
}

void AUnitAIController::OnUnitInitialized(const AUnit* InUnit, const FUnitSpawnParam& InUnitSpawnParams)
{
	OwnerIndex = InUnitSpawnParams.OwnerIndex;
	
	if(InUnitSpawnParams.OwnerIndex == PLAYER_INDEX)
	{
		bControlledByPlayer = true;
	}
	else
	{
		QuestTargets = InUnitSpawnParams.QuestTargets;
		ConvertedQuestLocation.Reset();
		for(const auto QuestTarget : QuestTargets)
		{
			ConvertedQuestLocation.Add(QuestTargets[0]->GetQuestLocation());
		}
		ProcessEngineEvent(EActionEvent::MoveToQuest);
	}

	GetWorld()->GetTimerManager().SetTimer(SearchEnemyTimerHandle, this, &AUnitAIController::SearchEnemyTimerEvent, 1, true);
}

UBlackboardComponent* AUnitAIController::GetBlackboard() const
{
	return BlackboardPtr;
}

void AUnitAIController::MoveToLeftMouseButtonPosition()
{
	DC_GET(ADcPlayerController, ControllerPtr,GetWorld());
	MoveToTargetLocation(ControllerPtr->LastRightMouseHitLocation);	
}

void AUnitAIController::MoveWithFormationToLeftMouseButtonPosition()
{
	DC_GET(ADcPlayerController, ControllerPtr,GetWorld());
	if(ControllerPtr->LastRightMouseHitLocationWithFormation.Contains(ControlledUnit))
	{
		MoveToTargetLocation(ControllerPtr->LastRightMouseHitLocationWithFormation[ControlledUnit]);
	}
}

void AUnitAIController::MoveToLastSelectedEnemy()
{
	DC_GET(ADcPlayerController, ControllerPtr,GetWorld());
	MoveToTargetActor(ControllerPtr->LastSelectedEnemyUnit.Get());
	AttackUnit(Cast<AUnit>(ControllerPtr->LastSelectedEnemyUnit));
}

void AUnitAIController::ResetAttackUnit()
{
	if(AttackTargetUnit.IsValid())
	{
		AttackTargetUnit->UnitDeadEvent.RemoveAll(this);
		AttackTargetUnit.Reset();
	}
	ControlledUnit->OnAttackUnitDisappeared(AttackTargetUnit.Get());
}

void AUnitAIController::MoveToTargetLocation(const FVector& TargetLocation)
{
	if(BlackboardPtr)
	{
		BlackboardPtr->SetValueAsVector(TEXT("TargetLocation"), TargetLocation);
	}
}

void AUnitAIController::MoveToTargetActor(UObject* TargetActor)
{
	if(BlackboardPtr)
	{
		BlackboardPtr->SetValueAsObject(TEXT("TargetActor"), TargetActor); 
	}
}

bool AUnitAIController::MoveToPriorityQuestLocation()
{
	if(ConvertedQuestLocation.Num() > 0)
	{
		MoveToTargetLocation(ConvertedQuestLocation[0]);
		return true;
	}
	return false;
}

void AUnitAIController::SetBtState(const int InState)
{
	if(BlackboardPtr)
	{
		StateManager.BTState = InState; 
		BlackboardPtr->SetValueAsInt(TEXT("UnitState"), InState);
	}
}

void AUnitAIController::Stop()
{
	SetBtState(STATE_NONE);
	if(BlackboardPtr)
	{
		BlackboardPtr->ClearValue("TargetLocation");
		BlackboardPtr->ClearValue("TargetActor");
	}
}

void AUnitAIController::AttackUnit(AActor* TargetActor)
{
	if(AttackTargetUnit.IsValid() && AttackTargetUnit != TargetActor)
	{
		AttackTargetUnit->UnitDeadEvent.RemoveAll(this);
	}
	
	AttackTargetUnit = Cast<AUnit>(TargetActor);
	AttackTargetUnit->UnitDeadEvent.AddUObject(this, &AUnitAIController::OnAttackUnitDisappeared);

	ControlledUnit->OnAttackUnit(AttackTargetUnit.Get());
}

void AUnitAIController::UpdateMovingType(const EUnitMovingType InUnitMovingType)
{
	StateManager.SetUnitMovingType(this, InUnitMovingType);
}

void AUnitAIController::OnAttackUnitDisappeared(AUnit* EnemyUnit)
{
	ResetAttackUnit();
}

void AUnitAIController::ProcessEngineEvent(const EActionEvent& EngineEvent)
{
	DC_GET(ADcPlayerController, ControllerPtr,GetWorld());

	if(bControlledByPlayer)
	{
		switch (EngineEvent)
		{
		case EActionEvent::SelectOne:
			{
				if(ControllerPtr->LastSelectedUnit.IsValid())
				{
					ControllerPtr->LastSelectedUnit->OnUnselect();
					if(const auto UnSelectedUnitController = Cast<AUnitAIController>(ControllerPtr->LastSelectedUnit->GetController()))
					{
						UnSelectedUnitController->bSelected = false;
					}
				}
				ControlledUnit->OnSelect();
				bSelected = true;
			}
			break;
		case EActionEvent::UnSelect:
			{
				ControlledUnit->OnUnselect();
				bSelected = false;
			}
			break;
		case EActionEvent::SelectMultiple:
			{
				ControlledUnit->OnSelect();
				bSelected = true;
				ControllerPtr->LastSelectedUnit = ControlledUnit;
			}
			break;
		default:;
		}
	}

	//상태머신에 전달.
	if(StateManager.GetStateObject())
	{
		StateManager.GetStateObject()->ProcessActionEvent(this, EngineEvent);
	}
}

AUnit* AUnitAIController::GetNearestUnit() const
{
	AUnit* NearestUnit = nullptr;
	if(ControlledUnit->OverlappingEnemies.Num() > 0)
	{
		//적 찾기
		float DistanceFromNearestActor = TNumericLimits<float>::Max();

		//TODO 일단 가장 가까운애.
		for (TWeakObjectPtr<AUnit> UnitToCheck : ControlledUnit->OverlappingEnemies)
		{
			if(!UnitToCheck.IsValid())
				continue;

			if(UnitToCheck->OwnerIndex == ControlledUnit->OwnerIndex)
				continue;
		
			const float DistanceFromActorToCheck = (ControlledUnit->GetActorLocation() - UnitToCheck->GetActorLocation()).SizeSquared();
			if (DistanceFromActorToCheck < DistanceFromNearestActor)
			{
				NearestUnit = UnitToCheck.Get();
				DistanceFromNearestActor = DistanceFromActorToCheck;
			}
		}
	}

	return NearestUnit;
}

void AUnitAIController::SearchEnemyTimerEvent()
{

	// if(ControlledUnit->OverlappingEnemies.Num() <= 0)//근처 적이 없을때
	// {
	// 	if(bControlledByPlayer)
	// 	{
	// 		//따로 처리 없음.
	// 	}
	// 	else
	// 	{
	// 		if(StateManager == STATE_ATTACKING && false == MoveToPriorityQuestLocation())
	// 		{
	// 			Stop();
	// 		}
	// 	}
	// }


	if(StateManager.GetStateObject())
	{
		StateManager.GetStateObject()->OnSearchEnemy( this);
	}
}
