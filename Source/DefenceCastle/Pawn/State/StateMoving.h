#pragma once
#include "StateBase.h"

class FStateMoving final : public FStateBase
{
public:
	virtual void ProcessActionEvent(AUnitAIController* Controller, const EActionEvent& EngineEvent) override;
	virtual void OnSearchEnemy( AUnitAIController* Controller) override;
	virtual void OnChangeUnitMovingType(AUnitAIController* Controller, const EUnitMovingType InUnitMovingType) override;

public:
	static FStateMoving Singleton;
};
