// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DefenceCastle/Data/UnitData.h"
#include "DefenceCastle/Predefine/PredefineMacro.h"
#include "GameFramework/Character.h"
#include "Unit.generated.h"

UCLASS()
class DEFENCECASTLE_API AUnit : public ACharacter
{
	GENERATED_BODY()

	friend class AUnitAIController;

	enum class  EUnitMontage
	{
		None,
		BasicAttack,
	};
public:
	// Sets default values for this character's properties
	AUnit();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Initialize(const FUnitSpawnParam& InUnitSpawnParams);
	void LoadByData();
	
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	UFUNCTION(BlueprintCallable)
	void ShowSelectionDecal(const bool bShow);

	
	virtual bool IsPlayerControlled() const override;//플레이어 컨트롤 여부

	const FString& GetUnitName() const { return UnitName; }
	int GetCurrentHp() const { return CurrentHp; }
	float GetCurrentHpPercent() const { return CurrentHp/static_cast<float>(MaxHp) * 100; }
	float GetMovementSpeed() const;

protected:
	//UnitAIController Event
	void OnSelect();
	void OnUnselect();
	void OnAttackUnit(AUnit* TargetEnemy);
	void OnAttackUnitDisappeared(const AUnit* EnemyUnit);
	
	UFUNCTION(BlueprintCallable)
	void BasicAttack(bool bPlay);

	//State Event
	void OnDead();
	
private:
	UFUNCTION()
	void OnBeginDetectRangeBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndDetectRangeBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void OnBeginMeleeAttackRangeBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndMeleeAttackRangeBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	//Timer Event
	void OnResetAttackCoolTime();
	void OnCheckingAnimation();
	//Util
	static void PlayMontage(const class USkeletalMeshComponent* InSkeletalMeshComponent, class UAnimMontage* MontageToPlay, const float PlayRate = 1.f, const float StartingPosition = 0.0f, const FName& StartingSection = TEXT(""));
	static void StopMontage(const class USkeletalMeshComponent* InSkeletalMeshComponent, class UAnimMontage* MontageToPlay);

public:
	TWeakObjectPtr<class AUnitAIController> CastedController;
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnUnitDeadEvent , AUnit* DeadUnit)
	FOnUnitDeadEvent UnitDeadEvent;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UDecalComponent* SelectedDecal = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* WoodMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* BagMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* QuiverMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* ShieldMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* WeaponRMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* WeaponLMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* ShoulderPadMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* LegMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* ArmMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* HeadMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* MeleeAttackRangeComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* ClickDetectionRangeComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* DetectRangeSphereComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Data")
	FName UnitDataKey;
	UPROPERTY(VisibleAnywhere, Category="Data")
	FName BasicSkillDataKey;
	UPROPERTY(VisibleAnywhere, Category="Data")
	FSkillData BasicSkillData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Property")
	int OwnerIndex = INVALID_INDEX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Property")
	bool bNotSpawnedUnit = false;

	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category="State")
	int CurrentHp = 0;
	UPROPERTY(Transient, VisibleAnywhere, Category="Property")
	FString UnitName;
	UPROPERTY(Transient, VisibleAnywhere, Category="Property")
	int MaxHp = 0;

private:
	bool bInitialized = false;
	//AI
	TSet<TWeakObjectPtr<AUnit>> OverlappingEnemies;
	TSet<TWeakObjectPtr<AUnit>> InAttackRangeEnemies;
	FTimerHandle AttackCoolTimeTimerHandle;
	//Animation
	FTimerHandle AnimationCheckTimerHandle;
	EUnitMontage CurrentPlayingMontage = EUnitMontage::None;
	//Resource
	UPROPERTY()
	TArray<UObject*> LoadedAssets;
};