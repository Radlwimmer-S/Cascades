#pragma once
#include "KdPrimitive.h"

struct HitResult
{
	float Distance = INFINITY;
	KdPrimitive* Triangle;
};
