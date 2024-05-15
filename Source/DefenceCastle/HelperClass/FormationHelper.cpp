#include "FormationHelper.h"

#include "Kismet/KismetMathLibrary.h"

void FFormationHelper::GetFormationLocations(const FVector& FirstUnitLocation, const FVector& TargetLocation, const int UnitCount, TArray<FVector>& OutResult, const float RowInterval, const float ColumnInterval)
{
	OutResult.Reset();
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(FirstUnitLocation, TargetLocation);
	LookAtRotation.Pitch = 0;
	LookAtRotation.Roll = 0;

	const FVector RowVector = UKismetMathLibrary::GetRightVector(LookAtRotation) * RowInterval;
	const FVector ColumnVector = UKismetMathLibrary::GetForwardVector(LookAtRotation) * ColumnInterval;
	
	if(UnitCount <= 4)
	{
		for(int Index = 0; Index < UnitCount; ++Index)
		{
			OutResult.Add(GetFourUnitSquareFormationLocation(Index, TargetLocation, RowVector, ColumnVector));
		}
	}
	else if(UnitCount <= 9)
	{
		for(int Index = 0; Index < UnitCount; ++Index)
		{
			OutResult.Add(GetNineUnitSquareFormationLocation(Index, TargetLocation, RowVector, ColumnVector));
		}
	}
	else if(UnitCount <= 16)
	{
		for(int Index = 0; Index < UnitCount; ++Index)
		{
			OutResult.Add(GetSixteenUnitSquareFormationLocation(Index, TargetLocation, RowVector, ColumnVector));
		}
	}
}

FVector FFormationHelper::GetFourUnitSquareFormationLocation(const int Index, const FVector& TargetLocation,
	const FVector& RowIntervalVector, const FVector& ColumnIntervalVector)
{
	FVector CalculatedLocation;
	switch (Index)
	{
	case 0:
		{
			CalculatedLocation = TargetLocation - RowIntervalVector + ColumnIntervalVector;
		}
		break;
	case 1:
		{
			CalculatedLocation = TargetLocation + RowIntervalVector + ColumnIntervalVector;
		}
		break;
	case 2:
		{
			CalculatedLocation = TargetLocation - RowIntervalVector - ColumnIntervalVector;
		}
		break;
	case 3:
		{
			CalculatedLocation = TargetLocation + RowIntervalVector - ColumnIntervalVector;
		}
		break;
	default:;
	}
	return CalculatedLocation;
}

FVector FFormationHelper::GetNineUnitSquareFormationLocation(const int Index, const FVector& TargetLocation,
                                                          const FVector& RowIntervalVector, const FVector& ColumnIntervalVector)
{
	FVector CalculatedLocation;
	switch (Index)
	{
	case 0:
		{
			CalculatedLocation = TargetLocation + (2 * RowIntervalVector) + ColumnIntervalVector;
		}
		break;
	case 1:
		{
			CalculatedLocation = TargetLocation + ColumnIntervalVector;
		}
		break;
	case 2:
		{
			CalculatedLocation = TargetLocation - (2 * RowIntervalVector) + ColumnIntervalVector;
		}
		break;
	case 3:
		{
			CalculatedLocation = TargetLocation + (2 * RowIntervalVector);
		}
		break;
	case 4:
		{
			CalculatedLocation = TargetLocation;
		}
		break;
	case 5:
		{
			CalculatedLocation = TargetLocation - (2 * RowIntervalVector);
		}
		break;
	case 6:
		{
			CalculatedLocation = TargetLocation + (2 * RowIntervalVector) - ColumnIntervalVector;
		}
		break;
	case 7:
		{
			CalculatedLocation = TargetLocation - ColumnIntervalVector;
		}
		break;
	case 8:
		{
			CalculatedLocation = TargetLocation - (2 * RowIntervalVector) - ColumnIntervalVector;
		}
		break;
	default:;
	}
	return CalculatedLocation;
}
	
FVector FFormationHelper::GetSixteenUnitSquareFormationLocation(const int Index, const FVector& TargetLocation,
	const FVector& RowIntervalVector, const FVector& ColumnIntervalVector)
{
	if(Index < 4)
	{
		return GetFourUnitSquareFormationLocation(Index, TargetLocation, RowIntervalVector, ColumnIntervalVector);
	}

	if(Index < 8)
	{
		return GetFourUnitSquareFormationLocation(Index % 4, TargetLocation, RowIntervalVector * 3, ColumnIntervalVector);
	}

	if(Index < 12)
	{
		return GetFourUnitSquareFormationLocation(Index % 4, TargetLocation, RowIntervalVector, ColumnIntervalVector * 3);
	}

	return GetFourUnitSquareFormationLocation(Index % 4, TargetLocation, RowIntervalVector * 3, ColumnIntervalVector * 3); 
}
