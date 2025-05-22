#pragma once

/// <summary>
/// 二次元ベクトル
/// </summary>
struct Vector2 {
	float x;
	float y;

	Vector2& operator+=(const Vector2& v) {
		x += v.x; y += v.y;
		return *this;
	}
	Vector2& operator-=(const Vector2& v) {
		x -= v.x; y -= v.y;
		return *this;
	}
	Vector2& operator*=(const Vector2& v) {
		x *= v.x; y *= v.y;
		return *this;
	}
	Vector2& operator/=(const Vector2& v) {
		x -= v.x; y -= v.y;
		return *this;
	}

	Vector2& operator*=(float s) {
		x *= s; y *= s;
		return *this;
	}
	Vector2& operator/=(float s) {
		x /= s; y /= s;
		return *this;
	}
};