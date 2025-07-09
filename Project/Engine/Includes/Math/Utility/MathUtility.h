#pragma once

// C++
#include <cmath>
#include <algorithm>
#include <numbers>
#include <vector>

// MyHedder
#include "Math/Types/AllMathTypes.h"

///
/// Vector2
///

Vector2 operator-(const Vector2& v);
Vector2 operator+(const Vector2& v);

///
/// Vector2 & Vector2
///

Vector2 operator+(const Vector2& v1, const Vector2& v2);
Vector2 operator-(const Vector2& v1, const Vector2& v2);
Vector2 operator*(const Vector2& v1, const Vector2& v2);
Vector2 operator/(const Vector2& v1, const Vector2& v2);

///
/// Vector2 & Scaler
///

Vector2 operator/(const Vector2& v, float s);
Vector2 operator*(float s, const Vector2& v);
Vector2 operator*(const Vector2& v, float s);

///
/// Vector3
///

Vector3 operator-(const Vector3& v);
Vector3 operator+(const Vector3& v);

///
/// Vector3 & Vector3
///

Vector3 operator+(const Vector3& v1, const Vector3& v2);
Vector3 operator-(const Vector3& v1, const Vector3& v2);
Vector3 operator*(const Vector3& v1, const Vector3& v2);
Vector3 operator/(const Vector3& v1, const Vector3& v2);

bool operator!=(const Vector3& v1, const Vector3& v2);
bool operator==(const Vector3& v1, const Vector3& v2);

///
/// Vector3 & Scaler
///

Vector3 operator*(float s, const Vector3& v);
Vector3 operator*(const Vector3& v, float s);
Vector3 operator/(const Vector3& v, float s);

///
/// Vector4
///

Vector4 operator-(const Vector4& v);
Vector4 operator+(const Vector4& v);

///
/// Vector4 & Vector4
///

Vector4 operator+(const Vector4& v1, const Vector4& v2);
Vector4 operator-(const Vector4& v1, const Vector4& v2);
Vector4 operator*(const Vector4& v1, const Vector4& v2);
Vector4 operator/(const Vector4& v1, const Vector4& v2);

///
/// Vector4 & Scaler
///

Vector4 operator*(float s, const Vector4& v);
Vector4 operator*(const Vector4& v, float s);
Vector4 operator/(const Vector4& v, float s);

///
/// Matrix4x4
///

///
/// Matrix4x4 & Matrix4x4
///

Matrix4x4 operator+(const Matrix4x4& m1, const Matrix4x4& m2);
Matrix4x4 operator-(const Matrix4x4& m1, const Matrix4x4& m2);
Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2);

///
/// Quaternion
///

///
/// Quaternion & Quaternion
///

Quaternion operator+(const Quaternion& q1, const Quaternion& q2);
Quaternion operator-(const Quaternion& q1, const Quaternion& q2);
Quaternion operator*(const Quaternion& q1, const Quaternion& q2);

bool operator!=(const Quaternion& q1, const Quaternion& q2);
bool operator==(const Quaternion& q1, const Quaternion& q2);

/// <summary>
/// 数学関数群　(左手座標系、行ベクトル)
/// </summary>
namespace MAGIMath {

	// ゼロベクトルを返す
	Vector3 MakeZeroVector3();
	// 右方向のベクトル
	Vector3 MakeRightVector3();
	// 上方向のベクトル
	Vector3 MakeUpVector3();
	// 前方のベクトル
	Vector3 MakeForwardVector3();
	// 後方ベクトル
	Vector3	MakeBackVector3();

	// ベクトルの長さ
	float Length(const Vector2& a);
	float Length(const Vector3& a);

	// 内積
	float Dot(const Vector2& a, const Vector2& b);
	float Dot(const Vector3& a, const Vector3& b);

	// ベクトルの長さとの二乗値
	float LengthSquared(const Vector3& v);
	float LengthSquared(const Vector2& v);

	// 度数法からラジアンに変換
	float DegreeToRadian(float degree);

	/// [-π, +π] に折り返す
	float WrapPi(float angle);

	// 角度線形補間(最短経路で補間する)
	float LerpAngle(float a, float b, float t);

	// 線形補完
	float Lerp(float a, float b, float t);
	// 線形補完
	Vector2 Lerp(const Vector2& v1, const Vector2& v2, float t);
	// 線形補完
	Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);
	// 線形補完
	Vector4 Lerp(const Vector4& v1, const Vector4& v2, float t);

	// 正規化
	Vector2 Normalize(const Vector2& v);

	// 正規化
	Vector3 Normalize(const Vector3& a);

	// ベクトルの向きを取得
	Vector3 Forward(const Vector3& rotate);

	// クオータニオンから正面を取得
	Vector3 Forward(const Quaternion& q);

	// クオータニオンから右向きを取得
	Vector3 Right(const Quaternion& q);

	// ヨーとピッチから方向を求める
	Vector3 DirectionFromYawPitch(float yaw, float pitch);

	// 向きから回転を取得
	Vector3 DirectionToEuler(const Vector3& dir);

	// キャットロムスプライン
	Vector3 CatmullRomSpline(const std::vector<Vector3>& controlPoints, float t);

	// クロス積
	Vector3 Cross(const Vector3& a, const Vector3& b);

	// クォータニオンを使用してベクトルを回転させる
	Vector3 Transform(const Vector3& vector, const Quaternion& rotation);

	// w除算付きのTransform
	Vector4 Transform(const Vector4& v, const Matrix4x4& m);

	// ワールド行列からワールド座標取得
	Vector3 ExtractionWorldPos(const Matrix4x4& m);

	// 回転の向きと量から回転行列を作る
	Matrix4x4 MakeRotateAxisAngle(const Vector3& axis, float angle);

	// 指定した回転から指定した回転にするための回転行列
	Matrix4x4 DirectionToDirection(const Vector3& from, const Vector3& to);

	// 4x4単位行列を返す
	Matrix4x4 MakeIdentityMatrix4x4();

	// 逆行列作成関数
	Matrix4x4 Inverse(const Matrix4x4& a);

	// 転置行列
	Matrix4x4 Transpose(const Matrix4x4& a);

	// 逆転置行列
	Matrix4x4 MakeInverseTransposeMatrix(const Matrix4x4& a);

	// スケール成分削除関数
	Matrix4x4 RemoveScaling(const Matrix4x4& mat);

	// 視点行列を作成
	Matrix4x4 MakeLookAtMatrix(const Vector3& eye, const Vector3& target, const Vector3& up);

	// 拡縮行列作成関数
	Matrix4x4 MakeScaleMatrix(const Vector3& scale);

	// 回転行列作成関数
	Matrix4x4 MakeRotateXMatrix(float radian); // X回転
	Matrix4x4 MakeRotateYMatrix(float radian); // Y回転
	Matrix4x4 MakeRotateZMatrix(float radian); // Z回転

	Matrix4x4 MakeRotateXYZMatrix(const Vector3& rotate); // XYZ回転
	Matrix4x4 MakeRotateZXYMatrix(const Vector3& rotate); // ZXY回転
	Matrix4x4 MakeRotateYXZMatrix(const Vector3& rotate); // YXZ回転
	Matrix4x4 MakeRotateXZYMatrix(const Vector3& rotate); // XZY回転

	Matrix4x4 MakeRotateMatrix(const Quaternion& q);// XYZクオータニオン

	// 平行移動行列作成関数
	Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

	// アフィン変換(オイラー)
	Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
	// アフィン変換(クオータニオン)
	Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Quaternion& rotate, const Vector3& translate);
	// アフィン行列分解
	void DecomposeAffineMatrix(const Matrix4x4& matrix, Vector3& outScale, Quaternion& outRotation, Vector3& outTranslate);

	// 回転行列からクオータニオンを作成
	Quaternion MatrixToQuaternion(const Matrix3x3& m);

	// ビューポート行列作成
	Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

	// 透視投影行列作成
	Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRaito, float nearClip, float farClip);

	// 正射影行列作成
	Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);
	Matrix4x4 MakeOrthographicMatrix(float width, float height, float nearClip, float farClip);

	// UV行列を作成
	Matrix4x4 MakeUVMatrix(const Vector2& scale, const float& rotateZ, const Vector2& translate);

	// 単位クオータニオンを返す
	Quaternion MakeIdentityQuaternion();

	// 協約クオータニオンを返す
	Quaternion Conjugate(const Quaternion& quaternion);

	// クオータニオン同士の内積
	float Dot(const Quaternion& q1, const Quaternion& q2);

	// クオータニオンのnormを返す
	float Norm(const Quaternion& quaternion);

	// オイラー角をクオータニオンに変換
	Quaternion EulerToQuaternionYXZ(const Vector3& euler);

	Vector3 QuaternionToEuler(const Quaternion& qIn);

	// 向きからクオータニオン角を取得
	Quaternion DirectionToQuaternion(const Vector3& direction);

	// 正規化したクオータニオンを返す
	Quaternion Normalize(const Quaternion& quaternion);
	// 逆クオータニオンを返す
	Quaternion Inverse(const Quaternion& quaternion);
	// 任意回転軸を表すクオータニオンの生成
	Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle);

	// ヨーピッチロールからクオータニオンを求める
	Quaternion QuaternionFromYawPitchRoll(float yaw, float pitch, float roll);

	// 球面線形補完
	Quaternion Slerp(Quaternion q1, Quaternion q2, float t);
}