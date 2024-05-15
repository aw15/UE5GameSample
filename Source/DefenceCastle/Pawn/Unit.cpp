// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit.h"

#include "UnitAIController.h"
#include "UnitResource.h"
#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"
#include "DefenceCastle/BaseClass/DataManager.h"
#include "DefenceCastle/BaseClass/DcPlayerController.h"
#include "DefenceCastle/Predefine/PredefineMacro.h"
#include "DefenceCastle/BaseClass/DcGameInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AUnit::AUnit()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SelectedDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("SelectedDecal"));
	SelectedDecal->SetupAttachment(RootComponent);

	WoodMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WoodMesh"));
	BagMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BagMesh"));
	QuiverMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("QuiverMesh"));
	ShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShieldMesh"));
	WeaponRMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponRMesh"));
	WeaponLMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponLMesh"));

	WoodMesh->SetupAttachment(GetMesh(), "WoodSocket");
	BagMesh->SetupAttachment(GetMesh(), "BagSocket");
	QuiverMesh->SetupAttachment(GetMesh(), "QuiverSocket");
	ShieldMesh->SetupAttachment(GetMesh() , "ShieldSocket");
	WeaponRMesh->SetupAttachment(GetMesh(), "WeaponSocket_R");
	WeaponLMesh->SetupAttachment(GetMesh(), "WeaponSocket_L");

	ShoulderPadMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ShoulderPadMesh"));
	LegMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LegMesh"));
	ArmMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmMesh"));
	HeadMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HeadMesh"));
	ShoulderPadMesh->SetupAttachment(GetMesh());
	LegMesh->SetupAttachment(GetMesh());
	ArmMesh->SetupAttachment(GetMesh());
	HeadMesh->SetupAttachment(GetMesh());

	MeleeAttackRangeComponent = CreateDefaultSubobject<USphereComponent>(TEXT("MeleeAttackRangeComponent"));
	MeleeAttackRangeComponent->SetupAttachment(RootComponent);
	ClickDetectionRangeComponent = CreateDefaultSubobject<USphereComponent>(TEXT("ClickDetectionRangeComponent"));
	ClickDetectionRangeComponent->SetupAttachment(RootComponent);
	DetectRangeSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("DetectRangeSphere"));
	DetectRangeSphereComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AUnit::BeginPlay()
{
	Super::BeginPlay();
	ShowSelectionDecal(false);
	DC_GET(ADcPlayerController, ControllerPtr, GetWorld());
	ControllerPtr->OnUnselectUnit.AddUObject(this, &AUnit::OnUnselect);

	CastedController = Cast<AUnitAIController>(GetController());
	
	DetectRangeSphereComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &AUnit::OnBeginDetectRangeBoxOverlap);
	DetectRangeSphereComponent->OnComponentEndOverlap.AddUniqueDynamic(this, &AUnit::OnEndDetectRangeBoxOverlap);

	MeleeAttackRangeComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &AUnit::OnBeginMeleeAttackRangeBoxOverlap);
	MeleeAttackRangeComponent->OnComponentEndOverlap.AddUniqueDynamic(this, &AUnit::OnEndMeleeAttackRangeBoxOverlap);

	if(bNotSpawnedUnit) //Spawn으로 생성된 유닛이 아닌 경우 별도로 Initialize 호출.
	{
		FUnitSpawnParam UnitSpawnParam;
		UnitSpawnParam.OwnerIndex = OwnerIndex;
		UnitSpawnParam.UnitDataKey = UnitDataKey;
		Initialize(UnitSpawnParam);
	}
}
// Called every frame
void AUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AUnit::Initialize(const FUnitSpawnParam& InUnitSpawnParams)
{
	OwnerIndex = InUnitSpawnParams.OwnerIndex;
	UnitDataKey = InUnitSpawnParams.UnitDataKey;


	if(const auto ControllerPtr = Cast<AUnitAIController>(GetController()))
	{
		ControllerPtr->OnUnitInitialized(this, InUnitSpawnParams);
	}
	
	LoadByData();

	GetWorld()->GetTimerManager().SetTimer(AnimationCheckTimerHandle, this, &AUnit::OnCheckingAnimation, 0.5f, true);

	bInitialized = true;
}

void AUnit::LoadByData()
{
	DC_GET(UDataManager, DataManager, GetWorld())
	if(const auto UnitDataPtr = DataManager->FindData<FUnitData>(EDataType::Unit,UnitDataKey))
	{
		if(GetMesh()) GetMesh()->SetSkeletalMesh(UnitDataPtr->BodyMesh);
		if(HeadMesh) HeadMesh->SetSkeletalMesh(UnitDataPtr->HeadMesh);
		if(LegMesh) LegMesh->SetSkeletalMesh(UnitDataPtr->LegMesh);
		if(ArmMesh) ArmMesh->SetSkeletalMesh(UnitDataPtr->ArmMesh);
		if(WeaponLMesh) WeaponLMesh->SetStaticMesh(UnitDataPtr->WeaponLMesh);
		if(WeaponRMesh) WeaponRMesh->SetStaticMesh(UnitDataPtr->WeaponRMesh);
		if(ShieldMesh) ShieldMesh->SetStaticMesh(UnitDataPtr->ShieldMesh);
		if(QuiverMesh) QuiverMesh->SetStaticMesh(UnitDataPtr->QuiverMesh);
		if(BagMesh) BagMesh->SetStaticMesh(UnitDataPtr->BagMesh);
		if(WoodMesh) BagMesh->SetStaticMesh(UnitDataPtr->WoodMesh);
		if(GetCharacterMovement()) GetCharacterMovement()->MaxWalkSpeed = UnitDataPtr->MovementSpeed;
		CurrentHp = UnitDataPtr->Hp;
		MaxHp = UnitDataPtr->Hp;
		UnitName = UnitDataPtr->Name.ToString();

		if(const auto BasicSkillDataPtr = DataManager->FindData<FSkillData>(EDataType::Skill, UnitDataPtr->BasicSKillDataKey))
		{
			BasicSkillDataKey = UnitDataPtr->BasicSKillDataKey;
			BasicSkillData = *BasicSkillDataPtr;
			
			//공격 쿨타임 타이머 생성.
			GetWorldTimerManager().SetTimer(AttackCoolTimeTimerHandle, this, &AUnit::OnResetAttackCoolTime, BasicSkillDataPtr->AttackCoolTime, true);
		}
	}
}

float AUnit::GetMovementSpeed() const
{
	if(GetCharacterMovement())
	{
		return GetCharacterMovement()->GetMaxSpeed();
	}
	return 0.f;
}

void AUnit::OnSelect()
{
	ShowSelectionDecal(true);
}

void AUnit::OnUnselect()
{
	ShowSelectionDecal(false);
}

void AUnit::OnAttackUnit(AUnit* TargetEnemy)
{
	
}

void AUnit::OnAttackUnitDisappeared(const AUnit* EnemyUnit)
{
	//TODO 목표 유닛 죽은뒤에 처리
	BasicAttack(false);
}

float AUnit::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	const float Damage =  Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	CurrentHp -= Damage;
	PrintScreenMacro(FString::Printf(TEXT("%s Hp : %d"), *GetName(), CurrentHp))
	if(CurrentHp <= 0)
	{
		OnDead();
	}
	
	return 0;
}

void AUnit::OnDead()
{
	UnitDeadEvent.Broadcast(this);
	DC_GET(ADcPlayerController, PlayerController, GetWorld())
	PlayerController->CheckAndCleanupUnitParameter(this);
	Destroy();
}

void AUnit::ShowSelectionDecal(const bool bShow)
{
	if (SelectedDecal)
	{
		SelectedDecal->SetVisibility(bShow);
	}
}

bool AUnit::IsPlayerControlled() const
{
	if(CastedController.IsValid())
	{
		return CastedController->bControlledByPlayer;
	}
	return false;
}

void AUnit::OnBeginDetectRangeBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(AUnit* Unit = Cast<AUnit>(OtherActor))
	{
		if(Unit->OwnerIndex == INVALID_INDEX || OwnerIndex == INVALID_INDEX)
			return;
		
		if(Unit->OwnerIndex != OwnerIndex)
		{
			OverlappingEnemies.Add(Unit);
		}
	}
}

void AUnit::OnEndDetectRangeBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(AUnit* Unit = Cast<AUnit>(OtherActor))
	{
		if(Unit->OwnerIndex == INVALID_INDEX || OwnerIndex == INVALID_INDEX)
			return;
		
		if(Unit->OwnerIndex != OwnerIndex)
		{
			OverlappingEnemies.Remove(Unit);
		}
	}
}

void AUnit::OnBeginMeleeAttackRangeBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(AUnit* Unit = Cast<AUnit>(OtherActor))
	{
		if(Unit != this)
		{
			InAttackRangeEnemies.Add(Unit);
			//BasicAttack(true);
		}
	}
}

void AUnit::OnEndMeleeAttackRangeBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(!CastedController.IsValid())
		return;
	
	if(AUnit* Unit = Cast<AUnit>(OtherActor))
	{
		InAttackRangeEnemies.Remove(Unit);
		// if( Unit == CastedController->AttackTargetUnit) //현재 공격대상 유닛이 없다면 공격 애니메이션 스탑.
		// {
		// 	BasicAttack(false);
		// }
	}
}

void AUnit::OnResetAttackCoolTime()
{
	if(InAttackRangeEnemies.Num() <= 0 || !CastedController.IsValid() ||  !CastedController->AttackTargetUnit.IsValid())
		return;
	
	//TODO DamageType 개선 필요, Damage 개선 필요
	if(InAttackRangeEnemies.Contains(CastedController->AttackTargetUnit))
	{
		UGameplayStatics::ApplyDamage(CastedController->AttackTargetUnit.Get(), BasicSkillData.Damage, GetController(), this, UDamageType::StaticClass()); 
	}
}

void AUnit::OnCheckingAnimation()
{
	if(!CastedController.IsValid())
		return;
	//공격 애니메이션 체크
	if(CastedController->AttackTargetUnit.IsValid() && InAttackRangeEnemies.Contains(CastedController->AttackTargetUnit)) //현재 공격대상 유닛이 없다면 공격 애니메이션 스탑.
	{
		SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),CastedController->AttackTargetUnit->GetActorLocation()));
		BasicAttack(true);
	}
	else
	{
		BasicAttack(false);
	}
}

void AUnit::PlayMontage(const USkeletalMeshComponent* InSkeletalMeshComponent, UAnimMontage* MontageToPlay, const float PlayRate, const float StartingPosition, const FName& StartingSection)
{
	if (InSkeletalMeshComponent)
	{
		if (UAnimInstance* AnimInstance = InSkeletalMeshComponent->GetAnimInstance())
		{
			const float MontageLength = AnimInstance->Montage_Play(MontageToPlay, PlayRate, EMontagePlayReturnType::MontageLength, StartingPosition);
			const bool bPlayedSuccessfully = (MontageLength > 0.f);

			if (bPlayedSuccessfully)
			{
				if (StartingSection != NAME_None)
				{
					AnimInstance->Montage_JumpToSection(StartingSection, MontageToPlay);
				}
			}
		}
	}
}

void AUnit::StopMontage(const USkeletalMeshComponent* InSkeletalMeshComponent, UAnimMontage* MontageToPlay)
{
	if (InSkeletalMeshComponent)
	{
		if (UAnimInstance* AnimInstance = InSkeletalMeshComponent->GetAnimInstance())
		{
			AnimInstance->Montage_Stop(0, MontageToPlay);
		}
	}
}

void AUnit::BasicAttack(bool bPlay)
{
	if(bPlay)
	{
		if(CurrentPlayingMontage == EUnitMontage::BasicAttack)
			return;
		
		DC_GET(UUnitResource, UnitResource, GetWorld())
		const TSoftObjectPtr<UAnimMontage> BasicAttackAnimMontage = UnitResource->GetUnitBasicAttackAnimMontage(EUnitType::Dwarf_Infantry);
		if(false == BasicAttackAnimMontage.IsValid())
		{
			DC_GET_STREAMABLE_MANAGER(GetWorld())
			const auto Handle= StreamableManager.RequestAsyncLoad(BasicAttackAnimMontage.ToSoftObjectPath(), FStreamableDelegate::CreateWeakLambda(this, [this]()
			{
				DC_GET(UUnitResource, UnitResource, GetWorld())
				const TSoftObjectPtr<UAnimMontage> BasicAttackAnimMontage = UnitResource->GetUnitBasicAttackAnimMontage(EUnitType::Dwarf_Infantry);
				PlayMontage(GetMesh(),BasicAttackAnimMontage.Get());
				PlayMontage(HeadMesh, BasicAttackAnimMontage.Get());
				PlayMontage(ArmMesh, BasicAttackAnimMontage.Get());
				LoadedAssets.Add(BasicAttackAnimMontage.Get());
				CurrentPlayingMontage = EUnitMontage::BasicAttack;
			}));
		}
		else
		{
			PlayMontage(GetMesh(), BasicAttackAnimMontage.Get());
			PlayMontage(HeadMesh, BasicAttackAnimMontage.Get());
			PlayMontage(ArmMesh, BasicAttackAnimMontage.Get());
			CurrentPlayingMontage = EUnitMontage::BasicAttack;
		}
	}
	else
	{
		DC_GET(UUnitResource, UnitResource, GetWorld())
		const TSoftObjectPtr<UAnimMontage> BasicAttackAnimMontage = UnitResource->GetUnitBasicAttackAnimMontage(EUnitType::Dwarf_Infantry);
		if(BasicAttackAnimMontage.IsValid())
		{
			StopMontage(GetMesh(), BasicAttackAnimMontage.Get());
			StopMontage(HeadMesh, BasicAttackAnimMontage.Get());
			StopMontage(ArmMesh, BasicAttackAnimMontage.Get());
			CurrentPlayingMontage = EUnitMontage::None;
		}
	}
}
