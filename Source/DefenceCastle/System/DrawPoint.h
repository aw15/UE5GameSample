// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DefenceCastle/Data/DrawData.h"
#include "GameFramework/Actor.h"
#include "DrawPoint.generated.h"

UCLASS()
class DEFENCECASTLE_API ADrawPoint : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADrawPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnBeginDrawAreaOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndDrawAreaOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	void SpawnUnit();
	void DrawUnit(FName& OutResultUnitKey);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> SceneComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UDecalComponent> DrawAreaDecal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class USphereComponent> DrawAreaSphere;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Property")
	class UArrowComponent* SpawnPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Property")
	UClass* SpawnUnitClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Property")
	FName DrawDataKey;

	FDrawData UnitDrawData;
	FRandomStream RandomStream;
};
