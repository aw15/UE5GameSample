// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestTarget.h"

#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"
#include "DefenceCastle/BaseClass/DcGameInstance.h"
#include "DefenceCastle/Pawn/Unit.h"
#include "DefenceCastle/Predefine/PredefineFunction.h"
#include "DefenceCastle/Predefine/PredefineMacro.h"


// Sets default values
AQuestTarget::AQuestTarget()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;
	OccupyingAreaDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("OccupyingAreaDecal"));
	OccupyingAreaDecal->SetupAttachment(SceneComponent);
	OccupyingAreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OccupyingAreaSphere"));
	OccupyingAreaSphere->SetupAttachment(OccupyingAreaDecal);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(SceneComponent);
}

// Called when the game starts or when spawned
void AQuestTarget::BeginPlay()
{
	Super::BeginPlay();
	if(GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(OccupyingTimerHandle, this, &AQuestTarget::CheckOccupying, OccupyingCheckRate, true);
	}
}

// Called every frame
void AQuestTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FVector AQuestTarget::GetQuestLocation() const
{
	FVector Center = OccupyingAreaDecal->GetComponentLocation();
	const FVector2d Result = FMath::RandPointInCircle(OccupyingAreaSphere->GetUnscaledSphereRadius());
	return {Center.X + Result.X, Center.Y + Result.Y, Center.Z};
}

void AQuestTarget::CheckOccupying()
{
	if(!OccupyingAreaSphere)
		return;

	TArray<AActor*> OverlappingActors;
	OccupyingAreaSphere->GetOverlappingActors(OverlappingActors, AUnit::StaticClass());

	int PlayerUnitCount = 0;
	int EnemyUnitCount = 0;
	
	for(AActor* Actor : OverlappingActors)
	{
		if(const AUnit* Unit = Cast<AUnit>(Actor))
		{
			if(Unit->IsPlayerControlled()) //플레이어 유닛이 하나라도 있다면 넘김.
			{
				PlayerUnitCount += 1;
			}
			else
			{
				EnemyUnitCount += 1;
			}
		}
	}
	Hp = Hp + FMath::Min(PlayerUnitCount - EnemyUnitCount, 0);
	if(Hp <= 0)
	{
		OnDestroyed();
	}
}

void AQuestTarget::OnDestroyed()
{
	//파괴 처리
	if(GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(OccupyingTimerHandle);
	}

	if(DestroyMesh.IsNull()) //설정된 메시가 아예 없을 때
	{
		Destroy();
		return;
	}
	
	if(DestroyMesh.IsValid())
	{
		Mesh->SetStaticMesh(DestroyMesh.Get());
	}
	else
	{
		DC_GET(UDcGameInstance, GameInstance, GetWorld())
		GameInstance->StreamableManager.RequestAsyncLoad(DestroyMesh.ToSoftObjectPath(), FStreamableDelegate::CreateWeakLambda(this, [this]()
		{
			Mesh->SetStaticMesh(DestroyMesh.Get());
			LoadedAsset.Add(DestroyMesh.Get());
		}));
	}
}

