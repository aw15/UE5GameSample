// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnumsAndInterface.generated.h"


UENUM()
enum class EActionEvent
{
	SelectOne,
	SelectMultiple,
	UnSelect,
	MoveSingle,
	MoveWithFormation,
	AttackTarget,
	MoveToQuest
};


// This class does not need to be modified.
UINTERFACE(Blueprintable, MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UEngineEventListener : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DEFENCECASTLE_API IEngineEventListener
{
	GENERATED_BODY()
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
	public:
	virtual void ProcessEngineEvent(const enum class EActionEvent& EngineEvent) {}
};
