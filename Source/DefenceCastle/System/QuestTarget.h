// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/DecalComponent.h"
#include "GameFramework/Actor.h"
#include "QuestTarget.generated.h"


UCLASS()
class DEFENCECASTLE_API AQuestTarget : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AQuestTarget();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	bool IsDestroyed() const  { return Hp <= 0; }
	FVector GetQuestLocation() const;

private:
	void CheckOccupying();
	//State
	void OnDestroyed();
	
public:
	DECLARE_DELEGATE_OneParam(FOnQuestComplete , const AQuestTarget*);
	FOnQuestComplete OnQuestComplete;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* SceneComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UDecalComponent* OccupyingAreaDecal;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USphereComponent* OccupyingAreaSphere;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UStaticMesh> DestroyMesh;
	UPROPERTY(VisibleAnywhere)
	int Hp = 1;
	UPROPERTY(VisibleAnywhere)
	float OccupyingCheckRate = 1.f;
	
private:
	UPROPERTY()
	TArray<UObject*> LoadedAsset;
	FTimerHandle OccupyingTimerHandle;
};
