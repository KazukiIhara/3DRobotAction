#pragma once

/// <summary>
/// 三次元ベクトル
/// </summary>
struct Vector3 {
	float x;
	float y;
	float z;

	Vector3& operator+=(const Vector3& v) {
		x += v.x; y += v.y; z += v.z;
		return *this;
	}
	Vector3& operator-=(const Vector3& v) {
		x -= v.x; y -= v.y; z -= v.z;
		return *this;
	}
	Vector3& operator*=(const Vector3& v) {
		x *= v.x; y *= v.y; z *= v.z;
		return *this;
	}
	Vector3& operator/=(const Vector3& v) {
		x -= v.x; y -= v.y; z -= v.z;
		return *this;
	}

	Vector3& operator*=(float s) {
		x *= s; y *= s; z *= s;
		return *this;
	}
	Vector3& operator/=(float s) {
		x /= s; y /= s; z /= s;
		return *this;
	}
};