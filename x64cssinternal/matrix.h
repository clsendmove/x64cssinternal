#pragma once
#include "vector.h"

typedef float matrix3x4[3][4];
typedef float matrix4x4[4][4];

class VMatrix
{
private:
	vec3 m[4][4]{};

public:
	inline const matrix3x4& As3x4() const
	{
		return *((const matrix3x4*)this);
	}
};