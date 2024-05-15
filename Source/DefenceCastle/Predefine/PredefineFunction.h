#pragma once

namespace CheckFunc
{
	inline bool IsPlayerUnit(const int OwnerIndex);
}

namespace MathFunc
{
	FVector RandomPointInBoundingBox(const FVector& Center, const FVector& HalfSize);
}
