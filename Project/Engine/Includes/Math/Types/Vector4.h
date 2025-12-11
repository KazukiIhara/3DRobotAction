#pragma once

/// <summary>
/// 四次元ベクトル
/// </summary>
struct Vector4 {
	float x;
	float y;
	float z;
	float w;

	Vector4& operator+=(const Vector4& v) {
		x += v.x; y += v.y; z += v.z; w += v.w;
		return *this;
	}
	Vector4& operator-=(const Vector4& v) {
		x -= v.x; y -= v.y; z -= v.z; w -= v.w;
		return *this;
	}
	Vector4& operator*=(const Vector4& v) {
		x *= v.x; y *= v.y; z *= v.z; w *= v.w;
		return *this;
	}
	Vector4& operator/=(const Vector4& v) {
		x /= v.x; y /= v.y; z /= v.z; w /= v.w;
		return *this;
	}

	Vector4& operator*=(float s) {
		x *= s; y *= s; z *= s; w *= s;
		return *this;
	}
	Vector4& operator/=(float s) {
		x /= s; y /= s; z /= s; w /= s;
		return *this;
	}
};