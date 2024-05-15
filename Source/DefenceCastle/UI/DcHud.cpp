﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "DcHud.h"

#include "EngineUtils.h"
#include "Components/CapsuleComponent.h"
#include "DefenceCastle/Pawn/Unit.h"
#include "GameFramework/Character.h"


// Sets default values
ADcHud::ADcHud()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADcHud::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADcHud::GetActorsInSelectionRectangleForUnit(const FVector2D& FirstPoint,
	const FVector2D& SecondPoint, TArray<AActor*>& OutActors, bool bActorMustBeFullyEnclosed)
{
	// Because this is a HUD function it is likely to get called each tick,
	// so make sure any previous contents of the out actor array have been cleared!
	OutActors.Reset();

	//Create Selection Rectangle from Points
	FBox2D SelectionRectangle(ForceInit);

	//This method ensures that an appropriate rectangle is generated, 
	//		no matter what the coordinates of first and second point actually are.
	SelectionRectangle += FirstPoint;
	SelectionRectangle += SecondPoint;


	//The Actor Bounds Point Mapping
	const FVector BoundsPointMapping[8] =
	{
		FVector(1.f, 1.f, 1.f),
		FVector(1.f, 1.f, -1.f),
		FVector(1.f, -1.f, 1.f),
		FVector(1.f, -1.f, -1.f),
		FVector(-1.f, 1.f, 1.f),
		FVector(-1.f, 1.f, -1.f),
		FVector(-1.f, -1.f, 1.f),
		FVector(-1.f, -1.f, -1.f) };

	//~~~

	//For Each Actor of the Class Filter Type
	for (TActorIterator<AUnit> Itr(GetWorld(), AUnit::StaticClass()); Itr; ++Itr)
	{
		AUnit* EachActor = *Itr;

		//Get Actor Bounds				//casting to base class, checked by template in the .h
		const FBox EachActorBounds = EachActor->GetCapsuleComponent()->Bounds.GetBox(); //Only Top One

		//Center
		const FVector BoxCenter = EachActorBounds.GetCenter();

		//Extents
		const FVector BoxExtents = EachActorBounds.GetExtent();

		// Build 2D bounding box of actor in screen space
		FBox2D ActorBox2D(ForceInit);
		for (uint8 BoundsPointItr = 0; BoundsPointItr < 8; BoundsPointItr++)
		{
			// Project vert into screen space.
			const FVector ProjectedWorldLocation = Project(BoxCenter + (BoundsPointMapping[BoundsPointItr] * BoxExtents), true);
			// Add to 2D bounding box if point is on the front side of the camera
			if (ProjectedWorldLocation.Z > 0.f)
			{
				ActorBox2D += FVector2D(ProjectedWorldLocation.X, ProjectedWorldLocation.Y);
			}
		}
		// Only consider actor boxes that have valid points inside
		if (ActorBox2D.bIsValid)
		{
			//Selection Box must fully enclose the Projected Actor Bounds
			if (bActorMustBeFullyEnclosed && SelectionRectangle.IsInside(ActorBox2D))
			{
				OutActors.Add(EachActor);
			}
			//Partial Intersection with Projected Actor Bounds
			else if (SelectionRectangle.Intersect(ActorBox2D))
			{
				OutActors.Add(EachActor);
			}
		}
	}
}

// Called every frame
void ADcHud::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

