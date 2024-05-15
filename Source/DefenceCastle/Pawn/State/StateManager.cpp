#include "StateManager.h"
#include "StateMoving.h"


FStateManager::FStateManager() : StateObject(&FStateMoving::Singleton)
{
}

void FStateManager::SetUnitMovingType(class AUnitAIController* Controller, const EUnitMovingType InUnitMovingType)
{
	UnitMovingType = InUnitMovingType;
	if(StateObject)
	{
		StateObject->OnChangeUnitMovingType(Controller, UnitMovingType);
	}
}

void FStateManager::SetStateObject(FStateBase* InStateObject)
{
	StateObject = InStateObject;
}
