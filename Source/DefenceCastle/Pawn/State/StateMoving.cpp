#include "StateMoving.h"

#include "StateAttacking.h"
#include "DefenceCastle/Pawn/UnitAIController.h"

FStateMoving FStateMoving::Singleton;
void FStateMoving::ProcessActionEvent(AUnitAIController* Controller, const EActionEvent& EngineEvent)
{
	if(!Controller)
		return;
	
	switch (EngineEvent)
	{
	case EActionEvent::MoveSingle:
		{
			Controller->SetBtState(STATE_MOVE_TO_LOCATION);
			Controller->MoveToLeftMouseButtonPosition();
		}
		break;
	case EActionEvent::MoveWithFormation:
		{
			Controller->SetBtState(STATE_MOVE_TO_LOCATION);
			Controller->MoveWithFormationToLeftMouseButtonPosition();
		}
		break;
	case EActionEvent::AttackTarget:
		{
			Controller->StateManager.SetStateObject(&FStateAttacking::Singleton);
			Controller->StateManager.StateObject->ProcessActionEvent(Controller, EngineEvent);
		}
		break;
	case EActionEvent::MoveToQuest:
		{
			Controller->SetBtState(STATE_MOVE_TO_LOCATION);
			Controller->MoveToPriorityQuestLocation();
		}
		break;
	default:;
	}
}

void FStateMoving::OnSearchEnemy(AUnitAIController* Controller)
{
	if(!Controller || !Controller->ControlledUnit)
		return;
	
	if(false == Controller->bControlledByPlayer)
	{
		if(const AUnit* NearestUnit = Controller->GetNearestUnit())
		{
			Controller->StateManager.SetStateObject(&FStateAttacking::Singleton);
			Controller->StateManager.StateObject->OnSearchEnemy(Controller);
		}
	}
}

void FStateMoving::OnChangeUnitMovingType(AUnitAIController* Controller, const EUnitMovingType InUnitMovingType)
{
	if(!Controller)
		return;

	Controller->ResetAttackUnit();
	
	switch (InUnitMovingType)
	{
	case EUnitMovingType::EnemyFirst:
		{
			Controller->StateManager.SetStateObject(&FStateAttacking::Singleton);
			Controller->StateManager.StateObject->OnChangeUnitMovingType(Controller, InUnitMovingType);
		}
		break;
	default: ;
	}
}
