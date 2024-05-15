// Fill out your copyright notice in the Description page of Project Settings.


#include "DcPlayerController.h"

#include "DcPlayer.h"
#include "DefenceCastle/HelperClass/FormationHelper.h"
#include "DefenceCastle/Pawn/Unit.h"
#include "DefenceCastle/Pawn/UnitAIController.h"
#include "DefenceCastle/Pawn/UnitSpawner.h"
#include "DefenceCastle/Predefine/BaseEnumsAndInterface.h"
#include "DefenceCastle/Predefine/PredefineMacro.h"
#include "DefenceCastle/Predefine/PredefineUnitClass.h"
#include "DefenceCastle/System/WavePoint.h"
#include "DefenceCastle/UI/DcHud.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_DC_GET(ADcPlayerController)
{
	return Cast<ADcPlayerController>(UGameplayStatics::GetPlayerController(WorldContextObject, 0));
}

// Sets default values
ADcPlayerController::ADcPlayerController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bShowMouseCursor = true;
	UnitSpawner = CreateDefaultSubobject<UUnitSpawner>("UnitSpawner");
}

// Called when the game starts or when spawned
void ADcPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADcPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADcPlayerController::EdgeScrolling() const
{
	const auto PlayerRef = GetPawn<ADcPlayer>();
	if(!PlayerRef)
		return;
	
	int SizeX = 0, SizeY = 0;
	GetViewportSize(SizeX, SizeY);
	if(SizeX == 0 || SizeY == 0)
		return;
	float MouseX = 0, MouseY = 0;
	GetMousePosition(MouseX, MouseY);
	//UE_LOG(LogTemp, Warning, TEXT("mouseX %f mouseY %f"), MouseX, MouseY);

	if(MouseX < FLT_EPSILON || MouseY < FLT_EPSILON)
		return;
	
	const float XPercent = MouseX/static_cast<float>(SizeX);
	const float YPercent = MouseY/static_cast<float>(SizeY);
	
	if(XPercent > 0.975)
	{
		PlayerRef->FloatingPawnMovement->AddInputVector(FVector(0,XPercent,0));
	}
	else if (XPercent < 0.025)
	{
		PlayerRef->FloatingPawnMovement->AddInputVector(FVector(0,XPercent - 1,0));
	}

	if(YPercent > 0.975)
	{
		PlayerRef->FloatingPawnMovement->AddInputVector(FVector(YPercent * -1,0,0));
	}
	else if (YPercent < 0.025)
	{
		PlayerRef->FloatingPawnMovement->AddInputVector(FVector(1 - YPercent,0,0));
	}
}

void ADcPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if(InputComponent)
	{
		InputComponent->BindAxis("Zoom",this, &ADcPlayerController::Zoom);
		InputComponent->BindAxis("MoveForward",this, &ADcPlayerController::MoveVertical);
		InputComponent->BindAxis("MoveRight",this, &ADcPlayerController::MoveHorizontal);
		InputComponent->BindAction("LeftMouseButton",IE_Pressed, this, &ADcPlayerController::LeftMousePressed);
		InputComponent->BindAction("LeftMouseButton",IE_Released, this, &ADcPlayerController::LeftMouseReleased);
		InputComponent->BindAction("RightMouseButton",IE_Pressed, this, &ADcPlayerController::RightMousePressed);
		InputComponent->BindAction("MultiSelection",IE_Pressed, this, &ADcPlayerController::LeftMousePressedWithCtrl);
		InputComponent->BindAction("DragSelect",IE_Pressed, this, &ADcPlayerController::LeftMousePressedWithCtrlShift);
		InputComponent->BindAction("DragSelect",IE_Released, this, &ADcPlayerController::LeftMouseReleasedWithCtrlShift);
	}
}

void ADcPlayerController::CheckAndCleanupUnitParameter(AUnit* Unit)
{
	MultipleSelectedUnits.Remove(Unit);
	if(LastSelectedEnemyUnit == Unit)
	{
		LastSelectedEnemyUnit = nullptr;
	}

	if(LastSelectedUnit == Unit)
	{
		LastSelectedUnit = nullptr;
	}
}

void ADcPlayerController::OnUIEventUpdateUnitMovingType(const EUnitMovingType UnitMovingType)
{
	if(MultipleSelectedUnits.Num() > 1)
	{
		for (auto& Unit : MultipleSelectedUnits)
		{
			if(Unit.IsValid() && Unit->CastedController.IsValid())
			{
				Unit->CastedController->UpdateMovingType(UnitMovingType);
			}
		}
	}
	else
	{
		if(LastSelectedUnit.IsValid() && LastSelectedUnit->CastedController.IsValid())
		{
			LastSelectedUnit->CastedController->UpdateMovingType(UnitMovingType);
		}
	}
}

void ADcPlayerController::Zoom(const float Amount)
{
	const auto PlayerRef = GetPawn<ADcPlayer>();
	if(!PlayerRef)
		return;
	
	PlayerRef->Zoom(Amount);
}

void ADcPlayerController::MoveVertical(const float Amount)
{
	if(const auto PlayerRef = GetPawn<ADcPlayer>())
	{
		PlayerRef->MoveVertical(Amount);
	}
}

void ADcPlayerController::MoveHorizontal(const float Amount)
{
	if(const auto PlayerRef = GetPawn<ADcPlayer>())
	{
		PlayerRef->MoveHorizontal(Amount);
	}
}

void ADcPlayerController::LeftMousePressed()
{
	FHitResult HitResult; 
	
	GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(TRACE_TYPE_VISIBILITY),false, HitResult); 
	if(HitResult.IsValidBlockingHit())
	{
		if(nullptr == Cast<AUnit>(HitResult.GetActor()))//선택된 게 유닛이 아닐때만
		{
			ProcessUnSelectAll();	
			LastLeftMouseHitLocation = HitResult.Location;
		}
	}

	GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(TRACE_TYPE_MOUSE_SELECT),false, HitResult);
	if(HitResult.IsValidBlockingHit())
	{
		LastLeftMouseHitLocation = HitResult.Location;
		ProcessSelectSingle(Cast<AUnit>(HitResult.GetActor()));
	}
}

void ADcPlayerController::LeftMousePressedWithCtrl()
{
	if(MultipleSelectedUnits.Num() > 16)
		return;
	
	FHitResult HitResult;
	GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(TRACE_TYPE_MOUSE_SELECT),false, HitResult);
	if(HitResult.IsValidBlockingHit())
	{
		ProcessSelectMultiple(Cast<AUnit>(HitResult.GetActor()));
	}
}

void ADcPlayerController::LeftMousePressedWithCtrlShift()
{
	if(CastedHud.IsValid())
	{
		CastedHud->DragStart();
	}
}

void ADcPlayerController::LeftMouseReleasedWithCtrlShift()
{
	if(CastedHud.IsValid())
	{
		CastedHud->DragEnd();
	}
}

void ADcPlayerController::LeftMouseReleased()
{
	if(CastedHud.IsValid())
	{
		CastedHud->DragEnd();
	}
}

void ADcPlayerController::RightMousePressed()
{
	//이동을 위해 땅쪽 trace는 무조건 계산.
	FHitResult HitResult;
	
	if(false == LastSelectedUnit.IsValid() && MultipleSelectedUnits.IsEmpty())
		return;

	GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(TRACE_TYPE_MOUSE_SELECT),false, HitResult);
	if(HitResult.IsValidBlockingHit())
	{
		if(AUnit* PressedUnit = Cast<AUnit>(HitResult.GetActor()); PressedUnit && false == PressedUnit->IsPlayerControlled())
		{
			LastSelectedEnemyUnit = PressedUnit;
			if(MultipleSelectedUnits.Num() > 0)
			{
				for(const auto& Unit : MultipleSelectedUnits)
				{
					if(!Unit.IsValid() || !Unit->CastedController.IsValid())
						continue;
					
					Unit->CastedController->ProcessEngineEvent(EActionEvent::AttackTarget);
				}
			}
			else
			{
				if(LastSelectedUnit->CastedController.IsValid())
				{
					LastSelectedUnit->CastedController->ProcessEngineEvent(EActionEvent::AttackTarget);
				}
			}
			return;
		}
	}

	
	
	GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(TRACE_TYPE_VISIBILITY),false, HitResult);
	if(HitResult.IsValidBlockingHit())
	{
		LastRightMouseHitLocation = HitResult.Location;
		if(MultipleSelectedUnits.Num() > 1)
		{
			TArray<FVector> FormationLocations;
			FFormationHelper::GetFormationLocations(MultipleSelectedUnits.begin()->Get()->GetActorLocation(), LastRightMouseHitLocation, MultipleSelectedUnits.Num(), OUT FormationLocations);
			if(FormationLocations.Num() != MultipleSelectedUnits.Num())
				return;

			int Index = 0;
			for(const auto& Unit :  MultipleSelectedUnits)
			{
				if(!Unit.IsValid())
					return;
				
				LastRightMouseHitLocationWithFormation.Add(Unit.Get(), FormationLocations[Index]);
				if(Unit->CastedController.IsValid())
				{
					Unit->CastedController->ProcessEngineEvent(EActionEvent::MoveWithFormation);
				}
				Index += 1;
			}
		}
		else
		{
			if(LastSelectedUnit.IsValid() && LastSelectedUnit->CastedController.IsValid())
			{
				LastSelectedUnit->CastedController->ProcessEngineEvent(EActionEvent::MoveSingle);
			}
		}
	}
}

void ADcPlayerController::SelectUnitOnHud(const TArray<AActor*>& SelectActors)
{
	ProcessUnSelectAll();
	
	for(const auto ActorPtr : SelectActors)
	{
		ProcessSelectMultiple(Cast<AUnit>(ActorPtr));
	}
}

void ADcPlayerController::ProcessSelectMultiple(AUnit* SelectedUnit)
{
	if(!SelectedUnit || !SelectedUnit->IsPlayerControlled())
		return;
	
	if(SelectedUnit->CastedController.IsValid())
	{
		if(SelectedUnit->CastedController->bSelected)
		{
			SelectedUnit->CastedController->ProcessEngineEvent(EActionEvent::UnSelect);
			if(LastSelectedUnit == SelectedUnit)
			{
				LastSelectedUnit.Reset();
			}
			MultipleSelectedUnits.Remove(SelectedUnit);
		}
		else
		{
			MultipleSelectedUnits.Add(SelectedUnit);
			SelectedUnit->CastedController->ProcessEngineEvent(EActionEvent::SelectMultiple);
		}
	}
}

void ADcPlayerController::ProcessSelectSingle(AUnit* SelectedUnit)
{
	if(!SelectedUnit || !SelectedUnit->IsPlayerControlled())
		return;
	
	if(SelectedUnit->CastedController.IsValid())
	{
		if(SelectedUnit->CastedController->bSelected)
		{
			SelectedUnit->CastedController->ProcessEngineEvent(EActionEvent::UnSelect); 
			if(LastSelectedUnit == SelectedUnit)
			{
				LastSelectedUnit.Reset();
			}
			MultipleSelectedUnits.Remove(SelectedUnit);
		}
		else
		{
			MultipleSelectedUnits.Add(SelectedUnit);
			LastSelectedUnit = SelectedUnit;
			SelectedUnit->CastedController->ProcessEngineEvent(EActionEvent::SelectOne);

			SelectUnitInfoForUnitControlUIData.Name = SelectedUnit->GetUnitName();
			SelectUnitInfoForUnitControlUIData.MovementSpeed = FString::Printf(TEXT("%.1f"), SelectedUnit->GetMovementSpeed());
			SelectUnitInfoForUnitControlUIData.CurrentHp = FString::Printf(TEXT("%d(%.1f%%)"), SelectedUnit->GetCurrentHp(), SelectedUnit->GetCurrentHpPercent());
			OnUnitSelected.Broadcast(SelectUnitInfoForUnitControlUIData);
		}
	}
}

void ADcPlayerController::ProcessUnSelectAll()
{
	for(const auto& Unit : MultipleSelectedUnits)
	{
		if(Unit.IsValid() && Unit->CastedController.IsValid())
		{
			Unit->CastedController->ProcessEngineEvent(EActionEvent::UnSelect);
		}
	}
	MultipleSelectedUnits.Reset();
	LastSelectedUnit.Reset();
}

