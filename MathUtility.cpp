#include "MathUtility.h"

float MathUtility::Clamp(float value, float max, float min)
{
	if (value >= max) return max;
	else if (value <= min) return min;
	else return value;
}