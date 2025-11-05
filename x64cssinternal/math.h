#pragma once
#include "vector.h"
#include "matrix.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>
#include <vector>
#pragma warning(disable : 4305) 


#define DEG2RAD( x ) ( ( float )( x ) * ( float )( ( float )( PI ) / 180.0f ) )
#define RAD2DEG( x ) ( ( float )( x ) * ( float )( 180.0f / ( float )( PI ) ) )
#define square( x ) ( x * x )
#define RADPI 57.295779513082f
#define SQUARE( a ) a*a
#define PI			3.14159265358979323846
#define CHECK_VALID( _v ) 0
#define Assert( _exp ) ((void)0)

namespace math {
	inline void vector_transform(const vec3& vSome, const matrix3x4& vMatrix, vec3& vOut)
	{
		for (auto i = 0; i < 3; i++)
			vOut[i] = vSome.dot((vec3&)vMatrix[i]) + vMatrix[i][3];
	}
	inline void sin_cos(float radians, float* sine, float* cosine)
	{
		*sine = std::sin(radians);
		*cosine = std::cos(radians);
	}

	inline void angle_vectors(const vec3& angles, vec3* forward, vec3* right = nullptr, vec3* up = nullptr)
	{
		float sinRoll, sinPitch, sinYaw, cosRoll, cosPitch, cosYaw;
		sin_cos(DEG2RAD(angles.x), &sinPitch, &cosPitch);
		sin_cos(DEG2RAD(angles.y), &sinYaw, &cosYaw);
		sin_cos(DEG2RAD(angles.z), &sinRoll, &cosRoll);

		if (forward)
		{
			forward->x = cosPitch * cosYaw;
			forward->y = cosPitch * sinYaw;
			forward->z = -sinPitch;
		}

		if (right)
		{
			right->x = (-1 * sinRoll * sinPitch * cosYaw + -1 * cosRoll * -sinYaw);
			right->y = (-1 * sinRoll * sinPitch * sinYaw + -1 * cosRoll * cosYaw);
			right->z = -1 * sinRoll * cosPitch;
		}

		if (up)
		{
			up->x = (cosRoll * sinPitch * cosYaw + -sinRoll * -sinYaw);
			up->y = (cosRoll * sinPitch * sinYaw + -sinRoll * cosYaw);
			up->z = cosRoll * cosPitch;
		}
	}
	inline void vector_angles(vec3 forward, vec3& angles)
	{
		float tmp, yaw, pitch;

		if (forward.y == 0 && forward.x == 0)
		{
			yaw = 0;

			if (forward.z > 0)
				pitch = 270;
			else
				pitch = 90;
		}
		else
		{
			yaw = RAD2DEG(atan2f(forward.y, forward.x));

			if (yaw < 0)
				yaw += 360;

			tmp = forward.length2D();
			pitch = RAD2DEG(atan2f(-forward.z, tmp));

			if (pitch < 0)
				pitch += 360;
		}

		angles[0] = pitch;
		angles[1] = yaw;
		angles[2] = 0;
	}

	inline double fast_sqrt(double n)
	{
		return std::sqrt(n);
	}
	inline vec3 direction_to_angles(const vec3& direction) {
		auto magnitude = [&](const vec3& v) -> float {
			return fast_sqrt(v.dot(v));
			};

		float yaw, pitch;

		if (!direction.y && !direction.x) {
			yaw = 0.0f;

			if (direction.z > 0.0f)
				pitch = 270.0f;

			else pitch = 90.0f;
		}

		else {
			yaw = RAD2DEG(atan2f(direction.y, direction.x));
			pitch = RAD2DEG(atan2f(-direction.z, magnitude(vec3(direction))));

			if (yaw < 0.0f)
				yaw += 360.0f;

			if (pitch < 0.0f)
				pitch += 360.0f;
		}

		return { pitch, yaw, 0.0f };
	}
	inline float normalize_angle(float ang) noexcept
	{
		if (!std::isfinite(ang))
			return 0.0f;

		return std::remainder(ang, 360.0f);
	}
	inline float VectorNormalize(vec3& vector)
	{
		float length = vector.length();

		if (!length)
			vector.set();
		else
			vector /= length;

		return length;
	}
	inline vec3 calc_angle(const vec3& src, const vec3& dst)
	{
		vec3 angles = vec3(0.0f, 0.0f, 0.0f);
		vec3 delta = (src - dst);
		float fHyp = fast_sqrt((delta.x * delta.x) + (delta.y * delta.y));

		angles.x = (atanf(delta.z / fHyp) * RADPI);
		angles.y = (atanf(delta.y / delta.x) * RADPI);
		angles.z = 0.0f;

		if (delta.x >= 0.0f)
			angles.y += 180.0f;

		return angles;
	}
	inline vec3 velocity_to_angles(const vec3& direction)
	{
		auto Magnitude = [&](const vec3& v) -> float {
			return sqrtf(v.dot(v));
			};

		float yaw, pitch;

		if (direction.y == 0.0f && direction.x == 0.0f)
		{
			yaw = 0.0f;

			if (direction.z > 0.0f)
				pitch = 270.0f;

			else pitch = 90.0f;
		}

		else
		{
			yaw = RAD2DEG(atan2f(direction.y, direction.x));
			pitch = RAD2DEG(atan2f(-direction.z, Magnitude(vec3(direction))));

			if (yaw < 0.0f)
				yaw += 360.0f;

			if (pitch < 0.0f)
				pitch += 360.0f;
		}

		return { pitch, yaw, 0.0f };
	}
	inline float RemapValClamped(float val, float A, float B, float C, float D)
	{
		if (A == B)
			return val >= B ? D : C;
		float cVal = (val - A) / (B - A);
		cVal = std::clamp(cVal, 0.0f, 1.0f);

		return C + (D - C) * cVal;
	}
	static float normalizeRad(float a) noexcept
	{
		return std::isfinite(a) ? std::remainder(a, PI * 2) : 0.0f;
	}
	static float angleDiffRad(float a1, float a2) noexcept
	{
		float delta;

		delta = normalizeRad(a1 - a2);
		if (a1 > a2)
		{
			if (delta >= PI)
				delta -= PI * 2;
		}
		else
		{
			if (delta <= -PI)
				delta += PI * 2;
		}
		return delta;
	}
	inline float CalcFov(const vec3& src, const vec3& dst)
	{
		vec3 v_src = vec3();
		math::angle_vectors(src, &v_src);

		vec3 v_dst = vec3();
		math::angle_vectors(dst, &v_dst);

		float result = RAD2DEG(acos(v_dst.dot(v_src) / v_dst.lengthSqr()));

		if (!isfinite(result) || isinf(result) || isnan(result))
			result = 0.0f;

		return result;
	}
	inline vec2 RotateVec2(vec2& point, const vec2& origin, float radians)
	{
		float s = sin(radians);
		float c = cos(radians);

		//Translate point back to origin
		point.x -= origin.x;
		point.y -= origin.y;

		//Rotate point
		float xnew = point.x * c - point.y * s;
		float ynew = point.x * s + point.y * c;

		//Translate point back
		point.x = xnew + origin.x;
		point.y = ynew + origin.y;
		return point;
	}
	inline float remap(const float val, const float in_min, const float in_max, const float out_min, const float out_max)
	{
		if (in_min == in_max) {
			return val >= in_max ? out_max : out_min;
		}

		return out_min + (out_max - out_min) * std::clamp((val - in_min) / (in_max - in_min), 0.0f, 1.0f);
	}
	inline float get_distance_meter(const vec3& vec1, const vec3& vec2)
	{
		constexpr float HU_TO_METERS = 0.01905f; // Conversion factor
		return (vec1 - vec2).length() * HU_TO_METERS;
	}
}