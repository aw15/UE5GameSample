#pragma once
#include "DefenceCastle/Predefine/BaseEnumsAndInterface.h"
#include "DefenceCastle/Predefine/PredefineUnitClass.h"

class AUnitAIController;
class AUnit;
class FStateManager;

class FStateBase
{
public:
	virtual ~FStateBase() = default;
	virtual void OnSearchEnemy( AUnitAIController* Controller) {}
	virtual void ProcessActionEvent(AUnitAIController* Controller, const EActionEvent& EngineEvent) {}
	virtual void CheckAnimation(AUnitAIController* Controller) {}
	virtual void OnChangeUnitMovingType(AUnitAIController* Controller, const EUnitMovingType InUnitMovingType) {}
};
