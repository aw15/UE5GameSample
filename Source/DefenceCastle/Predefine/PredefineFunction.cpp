#include "PredefineFunction.h"
#include "PredefineMacro.h"

bool CheckFunc::IsPlayerUnit(const int OwnerIndex)
{
	return OwnerIndex == PLAYER_INDEX;
}

FVector MathFunc::RandomPointInBoundingBox(const FVector& Center, const FVector& HalfSize)
{
	const FVector BoxMin = Center - HalfSize;
	const FVector BoxMax = Center + HalfSize;
	return FMath::RandPointInBox(FBox(BoxMin, BoxMax));
}
