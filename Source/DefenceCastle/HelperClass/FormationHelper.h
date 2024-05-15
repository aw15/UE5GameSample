#pragma once

class FFormationHelper
{
public:
	static void GetFormationLocations(const FVector& FirstUnitLocation, const FVector& TargetLocation, const int UnitCount, TArray<FVector>& OutResult, const float RowInterval = 200, const float ColumnInterval = 200);
	static FVector GetFourUnitSquareFormationLocation(const int Index, const FVector& TargetLocation, const FVector& RowIntervalVector, const FVector& ColumnIntervalVector);
	static FVector GetNineUnitSquareFormationLocation(const int Index, const FVector& TargetLocation, const FVector& RowIntervalVector, const FVector& ColumnIntervalVector);
	static FVector GetSixteenUnitSquareFormationLocation(const int Index, const FVector& TargetLocation, const FVector& RowIntervalVector, const FVector& ColumnIntervalVector);
};
