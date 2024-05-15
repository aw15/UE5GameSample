#pragma once
#include "DefenceCastle/Predefine/PredefineMacro.h"
#include "DefenceCastle/Predefine/PredefineUnitClass.h"

class FStateManager
{
	friend class FStateMoving;
	friend class FStateAttacking;
public:
	explicit FStateManager();
	
	class FStateBase* GetStateObject() const {return StateObject;}
	void SetUnitMovingType(class AUnitAIController* Controller, const EUnitMovingType InUnitMovingType);
	void SetStateObject(FStateBase* InStateObject);
	
public:
	int BTState = STATE_NONE;
private:
	EUnitMovingType UnitMovingType = EUnitMovingType::MoveFirst; //일단 플레이어 유닛만 사용.
	class FStateBase* StateObject;
};
