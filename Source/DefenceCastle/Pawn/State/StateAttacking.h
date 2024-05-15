#pragma once
#include "StateBase.h"

class FStateAttacking : public FStateBase
{
public:
	virtual void ProcessActionEvent(AUnitAIController* Controller, const EActionEvent& ActionEvent) override;
	virtual void OnSearchEnemy( AUnitAIController* Controller) override;
	virtual void OnChangeUnitMovingType(AUnitAIController* Controller, const EUnitMovingType InUnitMovingType) override;
public:
	static FStateAttacking Singleton;
};
