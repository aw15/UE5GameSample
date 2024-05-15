#include "StateAttacking.h"

#include "StateMoving.h"
#include "DefenceCastle/Pawn/Unit.h"
#include "DefenceCastle/Pawn/UnitAIController.h"

FStateAttacking FStateAttacking::Singleton;
void FStateAttacking::ProcessActionEvent(AUnitAIController* Controller, const EActionEvent& ActionEvent)
{
	if(!Controller)
		return;
	
	switch (ActionEvent)
	{
	case EActionEvent::MoveSingle:
		{
			if(Controller->StateManager.UnitMovingType == EUnitMovingType::EnemyFirst)
			{
				Controller->SetBtState(STATE_MOVE_TO_LOCATION);
				Controller->MoveToLeftMouseButtonPosition();
			}
			else
			{
				Controller->StateManager.SetStateObject(&FStateMoving::Singleton);
				Controller->StateManager.StateObject->ProcessActionEvent(Controller, ActionEvent);
			}
		}
		break;
	case EActionEvent::MoveWithFormation:
		{
			if(Controller->StateManager.UnitMovingType == EUnitMovingType::EnemyFirst)
			{
				Controller->SetBtState(STATE_MOVE_TO_LOCATION);
				Controller->MoveWithFormationToLeftMouseButtonPosition();
			}
			else
			{
				Controller->StateManager.SetStateObject(&FStateMoving::Singleton);
				Controller->StateManager.StateObject->ProcessActionEvent(Controller, ActionEvent);
			}
		}
		break;
	case EActionEvent::AttackTarget:
		{
			Controller->SetBtState(STATE_ATTACKING);
			Controller->MoveToLastSelectedEnemy();
		}
		break;
	case EActionEvent::MoveToQuest:
		{
			Controller->StateManager.SetStateObject(&FStateMoving::Singleton);
			Controller->StateManager.StateObject->ProcessActionEvent(Controller, ActionEvent);
		}
		break;
	default:;
	}
}

void FStateAttacking::OnSearchEnemy(AUnitAIController* Controller)
{
	if(!Controller || !Controller->ControlledUnit)
		return;

	AUnit* NearestUnit = Controller->GetNearestUnit();
	if(NearestUnit)
	{
		Controller->SetBtState(STATE_ATTACKING);
		Controller->MoveToTargetActor(NearestUnit);
		Controller->AttackUnit(NearestUnit);
	}
	else
	{
		if(false  == Controller->bControlledByPlayer)
		{
			ProcessActionEvent(Controller, EActionEvent::MoveToQuest);
		}
	}
}

void FStateAttacking::OnChangeUnitMovingType(AUnitAIController* Controller, const EUnitMovingType InUnitMovingType)
{
	if(!Controller)
		return;
	
	switch(InUnitMovingType)
	{
	case EUnitMovingType::MoveFirst:
		{
			Controller->StateManager.SetStateObject(&FStateMoving::Singleton);
			Controller->StateManager.StateObject->OnChangeUnitMovingType(Controller, InUnitMovingType);
		}
		break;
	default:;
	}
}
