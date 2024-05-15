// Fill out your copyright notice in the Description page of Project Settings.


#include "DrawPoint.h"

#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"
#include "DefenceCastle/Predefine/PredefineMacro.h"
#include "../Pawn/UnitSpawner.h"
#include "Components/ArrowComponent.h"
#include "DefenceCastle/BaseClass/DataManager.h"
#include "DefenceCastle/BaseClass/DcGameInstance.h"
#include "DefenceCastle/Data/UnitData.h"


// Sets default values
ADrawPoint::ADrawPoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;
	DrawAreaDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("OccupyingAreaDecal"));
	DrawAreaDecal->SetupAttachment(SceneComponent);
	DrawAreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OccupyingAreaSphere"));
	DrawAreaSphere->SetupAttachment(DrawAreaDecal);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(SceneComponent);
	SpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnPoint"));
	SpawnPoint->SetupAttachment(SceneComponent);

	DrawAreaSphere->OnComponentBeginOverlap.AddUniqueDynamic(this, &ADrawPoint::OnBeginDrawAreaOverlap);
	DrawAreaSphere->OnComponentEndOverlap.AddUniqueDynamic(this, &ADrawPoint::OnEndDrawAreaOverlap);
}

// Called when the game starts or when spawned
void ADrawPoint::BeginPlay()
{
	Super::BeginPlay();
	constexpr std::hash<std::wstring> StringHash;
	const std::wstring HashString = *FString::Printf(TEXT("%lld%s"), FDateTime::Now().ToUnixTimestamp(), *GetName());
	const int64 Hash = StringHash(HashString.c_str());
	PrintScreenMacro_Int(TEXT(""), Hash)
	RandomStream.Initialize(Hash);

	DC_GET(UDataManager, DataManager, GetWorld())
	if(const auto DrawDataPtr = DataManager->FindData<FDrawData>(EDataType::DrawUnit, DrawDataKey))
	{
		UnitDrawData = *DrawDataPtr;
	}
}

void ADrawPoint::OnBeginDrawAreaOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	SpawnUnit();
}

void ADrawPoint::OnEndDrawAreaOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}

void ADrawPoint::SpawnUnit()
{
	DC_GET(UUnitSpawner, UnitSpawner, GetWorld())
	FUnitSpawnParam UnitSpawnParam;
	UnitSpawnParam.OwnerIndex = PLAYER_INDEX;
	DrawUnit(OUT UnitSpawnParam.UnitDataKey);
	if(false == UnitSpawnParam.UnitDataKey.IsNone())
	{
		UnitSpawner->SpawnUnit(SpawnUnitClass, SpawnPoint->GetComponentLocation(), SpawnPoint->GetComponentRotation(), UnitSpawnParam);
	}
}

void ADrawPoint::DrawUnit(FName& OutResultUnitKey)
{
	const float RandomValue = RandomStream.FRandRange(0.f, 100.f);
	for(const auto& [DataKey, Min, Max] : UnitDrawData.Probabilities)
	{
		if(RandomValue >= Min && RandomValue < Max)
		{
			OutResultUnitKey = DataKey;
			return;
		}
	}
}

// Called every frame
void ADrawPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

