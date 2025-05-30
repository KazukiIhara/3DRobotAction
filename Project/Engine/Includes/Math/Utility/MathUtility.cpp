#include "MathUtility.h"

#include <cassert>
#include <stdexcept>
#include <string>

static const float EPSILON = 1.0e-6f;

Vector2 operator-(const Vector2& v) {
	return Vector2(-v.x, -v.y);
}

Vector2 operator+(const Vector2& v) {
	return v;
}

Vector2 operator+(const Vector2& v1, const Vector2& v2) {
	return Vector2(v1.x + v2.x, v1.y + v2.y);
}

Vector2 operator-(const Vector2& v1, const Vector2& v2) {
	return Vector2(v1.x - v2.x, v1.y - v2.y);
}

Vector2 operator*(const Vector2& v1, const Vector2& v2) {
	return Vector2(v1.x * v2.x, v1.y * v2.y);
}

Vector2 operator/(const Vector2& v1, const Vector2& v2) {
	return Vector2(v1.x / v2.x, v1.y / v2.y);
}

Vector2 operator/(const Vector2& v, float s) {
	return Vector2(v.x / s, v.y / s);
}

Vector2 operator*(float s, const Vector2& v) {
	return Vector2(v.x * s, v.y * s);
}

Vector2 operator*(const Vector2& v, float s) {
	return Vector2(v.x * s, v.y * s);
}

///
/// Vector3
///

Vector3 operator-(const Vector3& v) {
	return Vector3(-v.x, -v.y, -v.z);
}

Vector3 operator+(const Vector3& v) {
	return v;
}

Vector3 operator+(const Vector3& v1, const Vector3& v2) {
	return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

Vector3 operator-(const Vector3& v1, const Vector3& v2) {
	return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

Vector3 operator*(const Vector3& v1, const Vector3& v2) {
	return Vector3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

Vector3 operator/(const Vector3& v1, const Vector3& v2) {
	return Vector3(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
}

bool operator!=(const Vector3& v1, const Vector3& v2) {
	return (v1.x != v2.x) || (v1.y != v2.y) || (v1.z != v2.z);
}

bool operator==(const Vector3& v1, const Vector3& v2) {
	return (v1.x == v2.x) && (v1.y == v2.y) && (v1.z == v2.z);
}

Vector3 operator*(float s, const Vector3& v) {
	return Vector3(v.x * s, v.y * s, v.z * s);
}

Vector3 operator*(const Vector3& v, float s) {
	return Vector3(v.x * s, v.y * s, v.z * s);
}

Vector3 operator/(const Vector3& v, float s) {
	return Vector3(v.x / s, v.y / s, v.z / s);
}

///
/// Vector4
///

Vector4 operator-(const Vector4& v) {
	return Vector4(-v.x, -v.y, -v.z, -v.w);
}

Vector4 operator+(const Vector4& v) {
	return v;
}

Vector4 operator+(const Vector4& v1, const Vector4& v2) {
	return Vector4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w);
}

Vector4 operator-(const Vector4& v1, const Vector4& v2) {
	return Vector4(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w);
}

Vector4 operator*(const Vector4& v1, const Vector4& v2) {
	return Vector4(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w);
}

Vector4 operator/(const Vector4& v1, const Vector4& v2) {
	return Vector4(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w / v2.w);
}

Vector4 operator*(float s, const Vector4& v) {
	return Vector4(v.x * s, v.y * s, v.z * s, v.w * s);
}

Vector4 operator*(const Vector4& v, float s) {
	return Vector4(v.x * s, v.y * s, v.z * s, v.w * s);
}

Vector4 operator/(const Vector4& v, float s) {
	return Vector4(v.x / s, v.y / s, v.z / s, v.w / s);
}

///
/// Matrix4x4
///

Matrix4x4 operator+(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = m1.m[i][j] + m2.m[i][j];
		}
	}
	return result;
}

Matrix4x4 operator-(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = m1.m[i][j] - m2.m[i][j];
		}
	}
	return result;
}

Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result{};
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = 0;
			for (int k = 0; k < 4; ++k) {
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}
	return result;
}

Quaternion operator+(const Quaternion& q1, const Quaternion& q2) {
	return Quaternion(q1.x + q2.x, q1.y + q2.y, q1.z + q2.z, q1.w + q2.w);
}

Quaternion operator-(const Quaternion& q1, const Quaternion& q2) {
	return Quaternion(q1.x - q2.x, q1.y - q2.y, q1.z - q2.z, q1.w - q2.w);
}

Quaternion operator*(const Quaternion& q1, const Quaternion& q2) {
	return Quaternion(
		q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y,
		q1.w * q2.y + q1.y * q2.w + q1.z * q2.x - q1.x * q2.z,
		q1.w * q2.z + q1.z * q2.w + q1.x * q2.y - q1.y * q2.x,
		q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z
	);
}

bool operator!=(const Quaternion& q1, const Quaternion& q2) {
	return (q1.x != q2.x) || (q1.y != q2.y) || (q1.z != q2.z) || (q1.w != q2.w);
}

bool operator==(const Quaternion& q1, const Quaternion& q2) {
	return (q1.x == q2.x) || (q1.y == q2.y) || (q1.z == q2.z) || (q1.w == q2.w);
}

Vector3 MAGIMath::MakeZeroVector3() {
	Vector3 result{};
	result.x = 0.0f;
	result.y = 0.0f;
	result.z = 0.0f;
	return result;
}

Vector3 MAGIMath::MakeRightVector3() {
	Vector3 result{};
	result.x = 1.0f;
	result.y = 0.0f;
	result.z = 0.0f;
	return result;
}

Vector3 MAGIMath::MakeUpVector3() {
	Vector3 result{};
	result.x = 0.0f;
	result.y = 1.0f;
	result.z = 0.0f;
	return result;
}

Vector3 MAGIMath::MakeForwardVector3() {
	Vector3 result{};
	result.x = 0.0f;
	result.y = 0.0f;
	result.z = 1.0f;
	return result;
}

float MAGIMath::Length(const Vector3& a) {
	return std::sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
}

float MAGIMath::Dot(const Vector3& a, const Vector3& b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

float MAGIMath::LengthSquared(const Vector3& v) {
	return v.x * v.x + v.y * v.y + v.z * v.z;
}

float MAGIMath::DegreeToRadian(float degree) {
	return degree * (std::numbers::pi_v<float> / 180.0f);
}

float MAGIMath::Lerp(float a, float b, float t) {
	return a + (b - a) * t;
}

Vector2 MAGIMath::Lerp(const Vector2& v1, const Vector2& v2, float t) {
	return v1 + (v2 - v1) * t;
}

Vector3 MAGIMath::Normalize(const Vector3& a) {
	float length = Length(a);
	Vector3 normalizedVector{};

	if (length != 0) {
		normalizedVector.x = a.x / length;
		normalizedVector.y = a.y / length;
		normalizedVector.z = a.z / length;
	} else {
		normalizedVector.x = 0;
		normalizedVector.y = 0;
		normalizedVector.z = 0;
	}

	return normalizedVector;
}

Vector3 MAGIMath::Forward(const Vector3& rotate) {
	// 基準の前方向ベクトル (ワールド座標系の Z 軸正方向)
	Vector3 baseForward = MakeForwardVector3();

	// XYZ軸の回転行列を作成
	Matrix4x4 rotationMatrix = MakeRotateXYZMatrix(rotate);

	// 回転行列で基準ベクトルを変換
	Vector3 forward{};
	forward.x = rotationMatrix.m[0][0] * baseForward.x +
		rotationMatrix.m[1][0] * baseForward.y +
		rotationMatrix.m[2][0] * baseForward.z;

	forward.y = rotationMatrix.m[0][1] * baseForward.x +
		rotationMatrix.m[1][1] * baseForward.y +
		rotationMatrix.m[2][1] * baseForward.z;

	forward.z = rotationMatrix.m[0][2] * baseForward.x +
		rotationMatrix.m[1][2] * baseForward.y +
		rotationMatrix.m[2][2] * baseForward.z;

	// 単位ベクトル化
	return Normalize(forward);
}

Vector3 MAGIMath::Forward(const Quaternion& q) {
	return Normalize(Vector3{
		2.0f * (q.x * q.z + q.w * q.y),
		2.0f * (q.y * q.z - q.w * q.x),
		1.0f - 2.0f * (q.x * q.x + q.y * q.y)
		});
}

Vector3 MAGIMath::Right(const Quaternion& q) {
	return Normalize(Vector3{
		1.0f - 2.0f * (q.y * q.y + q.z * q.z),
		2.0f * (q.x * q.y + q.w * q.z),
		2.0f * (q.x * q.z - q.w * q.y)
		});
}

Vector3 MAGIMath::DirectionToEuler(const Vector3& dir) {
	Vector3 forward = Normalize(dir);

	// yaw: Y軸回転（XZ平面での角度）
	float yaw = std::atan2(forward.x, -forward.z);

	// pitch: X軸回転（Y方向への傾き）
	float pitch = std::asin(forward.y);

	// roll: Z軸回転 → ライトやカメラでは通常不要（=0）
	float roll = 0.0f;

	return Vector3(pitch, yaw, roll); // X=Pitch, Y=Yaw, Z=Roll
}

Vector3 MAGIMath::Lerp(const Vector3& v1, const Vector3& v2, float t) {
	return v1 + (v2 - v1) * t;
}

Vector3 MAGIMath::CatmullRomSpline(const std::vector<Vector3>& controlPoints, float t) {
	int numPoints = static_cast<int>(controlPoints.size());
	if (numPoints < 4) {
		// 4つ未満のポイントではCatmull-Romスプラインが機能しないため、直接返す
		return controlPoints[0];
	}

	// ttの計算
	float tt = t * (numPoints - 1);
	int p1 = static_cast<int>(tt);
	p1 = std::clamp(p1, 1, numPoints - 3); // p1を範囲内に制限
	int p0 = p1 - 1;
	int p2 = p1 + 1;
	int p3 = p2 + 1;

	tt = tt - p1; // 局所的なtを取得

	// Catmull-Romスプラインの計算式に基づく補間
	float tt2 = tt * tt;
	float tt3 = tt2 * tt;

	Vector3 a = controlPoints[p0] * (-0.5f) + controlPoints[p1] * (1.5f) - controlPoints[p2] * (1.5f) + controlPoints[p3] * (0.5f);
	Vector3 b = controlPoints[p0] - controlPoints[p1] * (2.5f) + controlPoints[p2] * (2.0f) - controlPoints[p3] * (0.5f);
	Vector3 c = controlPoints[p2] * 0.5f - controlPoints[p0] * 0.5f;
	Vector3 d = controlPoints[p1];

	return a * tt3 + b * tt2 + c * tt + d;
}

Vector3 MAGIMath::Cross(const Vector3& a, const Vector3& b) {
	return {
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	};
}

Vector3 MAGIMath::Transform(const Vector3& vector, const Quaternion& rotation) {
	// クォータニオンの逆（共役）を計算
	Quaternion conjugateRotation = Conjugate(rotation);

	// ベクトルをクォータニオン形式に変換（w = 0）
	Quaternion vectorQuat = { vector.x, vector.y, vector.z, 0.0f };

	// 回転の適用: q * v * q^-1
	Quaternion rotatedQuat = rotation * vectorQuat * conjugateRotation;

	// 結果をベクトル形式に戻す
	return { rotatedQuat.x, rotatedQuat.y, rotatedQuat.z };
}

Vector4 MAGIMath::Transform(const Vector4& v, const Matrix4x4& m) {
	Vector4 result;
	result.x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + v.w * m.m[3][0];
	result.y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + v.w * m.m[3][1];
	result.z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + v.w * m.m[3][2];
	result.w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + v.w * m.m[3][3];
	return result;
}

Vector3 MAGIMath::ExtractionWorldPos(const Matrix4x4& m) {
	Vector3 result{};
	result.x = m.m[3][0];
	result.y = m.m[3][1];
	result.z = m.m[3][2];
	return result;
}

Matrix4x4 MAGIMath::MakeRotateAxisAngle(const Vector3& axis, float angle) {

	Vector3 u = axis;
	float x = u.x;
	float y = u.y;
	float z = u.z;

	float c = std::cos(-angle);
	float s = std::sin(-angle);
	float sub = 1.0f - c;

	Matrix4x4 rotation;

	rotation.m[0][0] = c + x * x * sub;
	rotation.m[0][1] = x * y * sub - z * s;
	rotation.m[0][2] = x * z * sub + y * s;
	rotation.m[0][3] = 0.0f;

	rotation.m[1][0] = y * x * sub + z * s;
	rotation.m[1][1] = c + y * y * sub;
	rotation.m[1][2] = y * z * sub - x * s;
	rotation.m[1][3] = 0.0f;

	rotation.m[2][0] = z * x * sub - y * s;
	rotation.m[2][1] = z * y * sub + x * s;
	rotation.m[2][2] = c + z * z * sub;
	rotation.m[2][3] = 0.0f;

	rotation.m[3][0] = 0.0f;
	rotation.m[3][1] = 0.0f;
	rotation.m[3][2] = 0.0f;
	rotation.m[3][3] = 1.0f;

	return rotation;
}

Matrix4x4 MAGIMath::DirectionToDirection(const Vector3& from, const Vector3& to) {

	Vector3 fromTmp = from;
	Vector3 toTmp = to;

	// 正規化
	Vector3 f = Normalize(fromTmp);
	Vector3 t = Normalize(toTmp);

	// f と t の内積 (角度計算用) を求める
	float dotFT = Dot(f, t);
	// -1.0f～1.0f の範囲にクリップして acos のドメインエラーを回避
	dotFT = std::max(-1.0f, std::min(dotFT, 1.0f));

	// なす角 θ
	float angle = std::acos(dotFT);

	// クロス積で回転軸を求める (f -> t の回転軸)
	Vector3 axis = Cross(f, t);
	float axisLen = Length(axis);

	// 回転軸の長さがほぼ0なら、f と t は平行か反平行
	if (axisLen < EPSILON) {
		// ほぼ同方向 (dotFT ~ 1.0f) の場合は回転不要→単位行列を返す
		if (dotFT > 0.9999f) {
			return Matrix4x4(); // 単位行列
		} else {
			// ほぼ反対方向 (dotFT ~ -1.0f) の場合
			// 180度回転させる軸が必要（f に直交する任意の軸でOK）
			// 例えば f が (0,0,1) に近ければ x軸やy軸などに対して 180度回転
			// 一例として、f に対して最も小さい成分の軸を選ぶなど
			Vector3 orth(0.0f, 0.0f, 0.0f);
			if (std::fabs(f.x) < std::fabs(f.y) && std::fabs(f.x) < std::fabs(f.z))
				orth = Vector3(1.0f, 0.0f, 0.0f);
			else if (std::fabs(f.y) < std::fabs(f.x) && std::fabs(f.y) < std::fabs(f.z))
				orth = Vector3(0.0f, 1.0f, 0.0f);
			else
				orth = Vector3(0.0f, 0.0f, 1.0f);

			// f と直交するベクトルを作る
			axis = Cross(f, orth);
			axis = Normalize(axis);
			angle = static_cast<float>(std::numbers::pi_v<float>); // 180度回転

			return MakeRotateAxisAngle(axis, angle);
		}
	} else {
		// 軸を正規化して Rodrigues の回転公式により行列を作る
		axis = Vector3(axis.x / axisLen, axis.y / axisLen, axis.z / axisLen);
		return MakeRotateAxisAngle(axis, angle);
	}
}

Matrix4x4 MAGIMath::MakeIdentityMatrix4x4() {
	Matrix4x4 result{};
	result.m[0][0] = 1.0f; result.m[0][1] = 0.0f; result.m[0][2] = 0.0f; result.m[0][3] = 0.0f;
	result.m[1][0] = 0.0f; result.m[1][1] = 1.0f; result.m[1][2] = 0.0f; result.m[1][3] = 0.0f;
	result.m[2][0] = 0.0f; result.m[2][1] = 0.0f; result.m[2][2] = 1.0f; result.m[2][3] = 0.0f;
	result.m[3][0] = 0.0f; result.m[3][1] = 0.0f; result.m[3][2] = 0.0f; result.m[3][3] = 1.0f;
	return result;
}

Matrix4x4 MAGIMath::Inverse(const Matrix4x4& a) {
	Matrix4x4 A = a;
	Matrix4x4 B = MakeIdentityMatrix4x4();

	for (int i = 0; i < 4; ++i) {
		// ピボット選択（ゼロ近傍チェック）
		const float epsilon = std::numeric_limits<float>::epsilon() * std::max(1.0f, std::abs(A.m[i][i]));
		if (std::abs(A.m[i][i]) < epsilon) {
			bool swapped = false;

			// ピボット列でスワップ行を探す
			for (int row = i + 1; row < 4; ++row) {
				if (std::abs(A.m[row][i]) > epsilon) {
					std::swap(A.m[i], A.m[row]);
					std::swap(B.m[i], B.m[row]);
					swapped = true;
					break;
				}
			}

			// スワップ失敗 → 特異行列
			if (!swapped) {
				throw std::runtime_error("Matrix is singular at row " + std::to_string(i) +
					" with pivot value " + std::to_string(A.m[i][i]));
			}
		}

		// ピボット行のスケーリング
		float scale = 1.0f / A.m[i][i];
		for (int j = 0; j < 4; ++j) {
			A.m[i][j] *= scale;
			B.m[i][j] *= scale;
		}

		// 他の行のピボット列をゼロにする
		for (int k = 0; k < 4; ++k) {
			if (k != i) {
				float factor = A.m[k][i];
				for (int j = 0; j < 4; ++j) {
					A.m[k][j] -= factor * A.m[i][j];
					B.m[k][j] -= factor * B.m[i][j];
				}
			}
		}
	}

	return B;
}

Matrix4x4 MAGIMath::Transpose(const Matrix4x4& a) {
	Matrix4x4 result{};
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = a.m[j][i];
		}
	}
	return result;
}

Matrix4x4 MAGIMath::MakeInverseTransposeMatrix(const Matrix4x4& a) {
	Matrix4x4 result = Inverse(a);
	result = Transpose(result);
	return result;
}

Matrix4x4 MAGIMath::RemoveScaling(const Matrix4x4& mat) {
	Matrix4x4 result = mat;

	// X軸方向のベクトルを正規化
	Vector3 xAxis = { result.m[0][0], result.m[1][0], result.m[2][0] };
	xAxis = Normalize(xAxis);
	result.m[0][0] = xAxis.x;
	result.m[1][0] = xAxis.y;
	result.m[2][0] = xAxis.z;

	// Y軸方向のベクトルを正規化
	Vector3 yAxis = { result.m[0][1], result.m[1][1], result.m[2][1] };
	yAxis = Normalize(yAxis);
	result.m[0][1] = yAxis.x;
	result.m[1][1] = yAxis.y;
	result.m[2][1] = yAxis.z;

	// Z軸方向のベクトルを正規化
	Vector3	zAxis = { result.m[0][2], result.m[1][2], result.m[2][2] };
	zAxis = Normalize(zAxis);
	result.m[0][2] = zAxis.x;
	result.m[1][2] = zAxis.y;
	result.m[2][2] = zAxis.z;

	return result;
}

Matrix4x4 MAGIMath::MakeLookAtMatrix(const Vector3& eye, const Vector3& target, const Vector3& up) {
	// 1) 前方向ベクトル（左手系: target - eye）
	Vector3 z = Normalize(target - eye);

	// 2) 右方向ベクトル
	Vector3 x = Normalize(Cross(up, z));

	// 3) 真上ベクトル
	Vector3 y = Cross(z, x);

	// 4) ビューマトリクスを組み立て
	Matrix4x4 result = {
		x.x,  y.x,  z.x,  0.0f,   // 第一行
		x.y,  y.y,  z.y,  0.0f,   // 第二行
		x.z,  y.z,  z.z,  0.0f,   // 第三行
	   -Dot(x, eye), -Dot(y, eye), -Dot(z, eye), 1.0f  // 第四行
	};

	return result;
}

Matrix4x4 MAGIMath::MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 result = {
		scale.x, 0.0f, 0.0f, 0.0f,
		0.0f, scale.y, 0.0f, 0.0f,
		0.0f, 0.0f, scale.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};
	return result;
}

Matrix4x4 MAGIMath::MakeRotateXMatrix(float radian) {
	Matrix4x4 rotateX{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, std::cos(radian), std::sin(radian), 0.0f,
		0.0f, -std::sin(radian), std::cos(radian), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};
	return rotateX;
}

Matrix4x4 MAGIMath::MakeRotateYMatrix(float radian) {
	Matrix4x4 rotateY{
		std::cos(radian), 0.0f, -std::sin(radian), 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		std::sin(radian), 0.0f, std::cos(radian), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};
	return rotateY;
}

Matrix4x4 MAGIMath::MakeRotateZMatrix(float radian) {
	Matrix4x4 rotateZ{
		std::cos(radian), std::sin(radian), 0.0f, 0.0f,
		-std::sin(radian), std::cos(radian), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};
	return rotateZ;
}

Matrix4x4 MAGIMath::MakeRotateXYZMatrix(const Vector3& rotate) {
	Matrix4x4 rotateX = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateY = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZ = MakeRotateZMatrix(rotate.z);

	Matrix4x4 result = rotateX * rotateY * rotateZ;
	return result;
}

Matrix4x4 MAGIMath::MakeRotateZXYMatrix(const Vector3& rotate) {
	Matrix4x4 rotateX = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateY = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZ = MakeRotateZMatrix(rotate.z);

	Matrix4x4 result = rotateZ * rotateX * rotateY;
	return result;
}

Matrix4x4 MAGIMath::MakeRotateYXZMatrix(const Vector3& rotate) {
	Matrix4x4 rotateX = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateY = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZ = MakeRotateZMatrix(rotate.z);

	Matrix4x4 result = rotateY * rotateX * rotateZ;
	return result;
}

Matrix4x4 MAGIMath::MakeRotateXZYMatrix(const Vector3& rotate) {
	Matrix4x4 rotateX = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateY = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZ = MakeRotateZMatrix(rotate.z);

	Matrix4x4 result = rotateX * rotateZ * rotateY;
	return result;
}

Matrix4x4 MAGIMath::MakeRotateMatrix(const Quaternion& q) {
	Matrix4x4 mat{};

	float xx = q.x * q.x;
	float yy = q.y * q.y;
	float zz = q.z * q.z;
	float xy = q.x * q.y;
	float yz = q.y * q.z;
	float zx = q.z * q.x;
	float wx = q.w * q.x;
	float wy = q.w * q.y;
	float wz = q.w * q.z;

	// 行 (i), 列 (j) の順で代入
	// row 0
	mat.m[0][0] = 1.0f - 2.0f * (yy + zz);
	mat.m[0][1] = 2.0f * (xy + wz);
	mat.m[0][2] = 2.0f * (zx - wy);
	mat.m[0][3] = 0.0f;

	// row 1
	mat.m[1][0] = 2.0f * (xy - wz);
	mat.m[1][1] = 1.0f - 2.0f * (xx + zz);
	mat.m[1][2] = 2.0f * (yz + wx);
	mat.m[1][3] = 0.0f;

	// row 2
	mat.m[2][0] = 2.0f * (zx + wy);
	mat.m[2][1] = 2.0f * (yz - wx);
	mat.m[2][2] = 1.0f - 2.0f * (xx + yy);
	mat.m[2][3] = 0.0f;

	// row 3
	mat.m[3][0] = 0.0f;
	mat.m[3][1] = 0.0f;
	mat.m[3][2] = 0.0f;
	mat.m[3][3] = 1.0f;

	return mat;
}

Matrix4x4 MAGIMath::MakeTranslateMatrix(const Vector3& translate) {
	Matrix4x4 result = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		translate.x, translate.y, translate.z, 1.0f
	};
	return result;
}

Matrix4x4 MAGIMath::MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4 result = MakeScaleMatrix(scale) * MakeRotateYXZMatrix(rotate) * MakeTranslateMatrix(translate);
	return result;
}

Matrix4x4 MAGIMath::MakeAffineMatrix(const Vector3& scale, const Quaternion& rotate, const Vector3& translate) {
	Matrix4x4 result = MakeScaleMatrix(scale) * MakeRotateMatrix(rotate) * MakeTranslateMatrix(translate);
	return result;
}

Matrix4x4 MAGIMath::MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth) {
	Matrix4x4 result =
	{
			width / 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, -height / 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, maxDepth - minDepth, 0.0f, left + width / 2.0f, top + height / 2.0f, minDepth, 1.0f,
	};
	return result;
}

Matrix4x4 MAGIMath::MakePerspectiveFovMatrix(float fovY, float aspectRaito, float nearClip, float farClip) {
	Matrix4x4 result =
	{
		(1.0f / aspectRaito) * (1.0f / std::tan(fovY / 2.0f)), 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f / std::tan(fovY / 2.0f), 0.0f, 0.0f,
		0.0f, 0.0f, farClip / (farClip - nearClip), 1.0f,
		0.0f, 0.0f, (-nearClip * farClip) / (farClip - nearClip), 0.0f,
	};
	return result;
}

Matrix4x4 MAGIMath::MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip) {
	Matrix4x4 result =
	{
		2.0f / (right - left), 0.0f, 0.0f, 0.0f,
		0.0f, 2.0f / (top - bottom), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f / (farClip - nearClip), 0.0f,
		(left + right) / (left - right), (top + bottom) / (bottom - top), nearClip / (nearClip - farClip), 1.0f,
	};
	return result;
}

Matrix4x4 MAGIMath::MakeOrthographicMatrix(float width, float height, float nearClip, float farClip) {
	const float l = -width * 0.5f;
	const float r = width * 0.5f;
	const float b = -height * 0.5f;
	const float t = height * 0.5f;

	Matrix4x4 result =
	{
		// row0
		2.0f / (r - l), 0.0f,           0.0f,                       0.0f,
		// row1
		0.0f,           2.0f / (t - b), 0.0f,                       0.0f,
		// row2
		0.0f,           0.0f,           1.0f / (farClip - nearClip), 0.0f,
		// row3
		-(r + l) / (r - l),
		-(t + b) / (t - b),
		-nearClip / (farClip - nearClip),
		1.0f,
	};
	return result;
}

Matrix4x4 MAGIMath::MakeUVMatrix(const Vector2& scale, const float& rotateZ, const Vector2& translate) {
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotateZ);
	Matrix4x4 scaleMatrix = MakeScaleMatrix(Vector3(scale.x, scale.y, 1.0f));
	Matrix4x4 translateMatrix = MakeTranslateMatrix(Vector3(translate.x, translate.y, 0.0f));
	return scaleMatrix * rotateZMatrix * translateMatrix;
}

Quaternion MAGIMath::MakeIdentityQuaternion() {
	Quaternion result{};
	result.x = 0.0f;
	result.y = 0.0f;
	result.z = 0.0f;
	result.w = 1.0f;
	return result;
}

Quaternion MAGIMath::Conjugate(const Quaternion& quaternion) {
	Quaternion result{};
	result.x = -quaternion.x;
	result.y = -quaternion.y;
	result.z = -quaternion.z;
	result.w = quaternion.w;
	return result;
}

float MAGIMath::Dot(const Quaternion& q1, const Quaternion& q2) {
	return q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
}

float MAGIMath::Norm(const Quaternion& quaternion) {
	return std::sqrt(
		quaternion.x * quaternion.x +
		quaternion.y * quaternion.y +
		quaternion.z * quaternion.z +
		quaternion.w * quaternion.w
	);
}

Quaternion MAGIMath::EulerToQuaternionYXZ(const Vector3& euler) {

	// 各軸回転クォータニオンを生成（X→Y→Z 軸）
	Quaternion qx = MakeRotateAxisAngleQuaternion({ 1, 0, 0 }, euler.x);
	Quaternion qy = MakeRotateAxisAngleQuaternion({ 0, 1, 0 }, euler.y);
	Quaternion qz = MakeRotateAxisAngleQuaternion({ 0, 0, 1 }, euler.z);

	Quaternion q = qy * qx * qz;

	return Normalize(q);
}

Vector3 MAGIMath::QuaternionToEuler(const Quaternion& qIn) {
	// ① 浮動小数誤差で伸びていることがあるので念のため正規化
	Quaternion q = Normalize(qIn);

	Vector3 euler; // [x]Pitch, [y]Yaw, [z]Roll

	// ── Roll(X) ─────────────────────────
	float sinr_cosp = 2.0f * (q.w * q.x + q.y * q.z);
	float cosr_cosp = 1.0f - 2.0f * (q.x * q.x + q.y * q.y);
	euler.x = std::atan2(sinr_cosp, cosr_cosp);

	// ── Pitch(Y) ────────────────────────
	float sinp = 2.0f * (q.w * q.y - q.z * q.x);
	if (std::abs(sinp) >= 1.0f) {
		// ±90° 近傍：ジンバルロック補正（符号維持）
		euler.y = std::copysign(std::numbers::pi_v<float> *0.5f, sinp);
	} else {
		euler.y = std::asin(sinp);
	}

	// ── Yaw(Z) ─────────────────────────
	float siny_cosp = 2.0f * (q.w * q.z + q.x * q.y);
	float cosy_cosp = 1.0f - 2.0f * (q.y * q.y + q.z * q.z);
	euler.z = std::atan2(siny_cosp, cosy_cosp);

	return euler; // 返り値は XYZ 順ラジアン
}

Quaternion MAGIMath::DirectionToQuaternion(const Vector3& direction) {
	Vector3 forward = Normalize(direction);
	Vector3 baseForward = Vector3(0.0f, 0.0f, -1.0f); // Z-方向を前方とする

	float dot = Dot(baseForward, forward);
	dot = std::clamp(dot, -1.0f, 1.0f); // 安全のためクランプ

	if (dot > 0.9999f) {
		// 同じ方向 → 単位クォータニオン
		return Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
	}

	if (dot < -0.9999f) {
		// 反対方向 → 任意の直交ベクトルを軸に180度回転
		Vector3 orth;
		if (std::fabs(baseForward.x) < std::fabs(baseForward.y) && std::fabs(baseForward.x) < std::fabs(baseForward.z))
			orth = Vector3(1.0f, 0.0f, 0.0f);
		else if (std::fabs(baseForward.y) < std::fabs(baseForward.z))
			orth = Vector3(0.0f, 1.0f, 0.0f);
		else
			orth = Vector3(0.0f, 0.0f, 1.0f);

		Vector3 axis = Normalize(Cross(baseForward, orth));
		return MakeRotateAxisAngleQuaternion(axis, static_cast<float>(std::numbers::pi_v<float>));
	}

	Vector3 axis = Normalize(Cross(baseForward, forward));
	float angle = std::acos(dot);

	return MakeRotateAxisAngleQuaternion(axis, angle);
}

Quaternion MAGIMath::Normalize(const Quaternion& quaternion) {
	// ノルムを求める
	float length = Norm(quaternion);

	// ノルムが 0 の場合は正規化できない
	if (length == 0.0f) {
		throw std::runtime_error("Cannot normalize a zero-norm quaternion.");
	}

	// 逆数を計算
	float inv_length = 1.0f / length;

	// 正規化したクオータニオンを返す
	Quaternion result = {
		.x = quaternion.x * inv_length,
		.y = quaternion.y * inv_length,
		.z = quaternion.z * inv_length,
		.w = quaternion.w * inv_length
	};
	return result;
}

Quaternion MAGIMath::Inverse(const Quaternion& quaternion) {
	float norm_sq =
		quaternion.x * quaternion.x +
		quaternion.y * quaternion.y +
		quaternion.z * quaternion.z +
		quaternion.w * quaternion.w;

	if (norm_sq == 0.0f) {
		// ノルムが0の場合は逆が定義できないため例外を投げる
		throw std::runtime_error("Cannot invert a zero-norm quaternion.");
	}

	// 協約
	Quaternion conj = Conjugate(quaternion);

	// 1 / ノルム^2
	float inv_norm = 1.0f / norm_sq;

	// 結果を格納
	Quaternion result;
	result.x = conj.x * inv_norm;
	result.y = conj.y * inv_norm;
	result.z = conj.z * inv_norm;
	result.w = conj.w * inv_norm;

	return result;
}

Quaternion MAGIMath::MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle) {
	// 軸ベクトルの長さを求める
	float axis_len = std::sqrt(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);

	// 軸ベクトルがゼロ長なら回転を定義できないので、適宜例外を投げる
	if (axis_len == 0.0f) {
		throw std::runtime_error("Axis vector has zero length. Cannot create rotation quaternion.");
	}

	// 正規化した軸ベクトル
	float inv_len = 1.0f / axis_len;
	float nx = axis.x * inv_len;
	float ny = axis.y * inv_len;
	float nz = axis.z * inv_len;

	// 回転角を2で割った値
	float half_angle = angle * 0.5f;
	float s = std::sin(half_angle);
	float c = std::cos(half_angle);

	// クオータニオン (x, y, z, w) の順で格納
	Quaternion q{};
	q.x = nx * s;
	q.y = ny * s;
	q.z = nz * s;
	q.w = c;

	return q;
}

Quaternion MAGIMath::QuaternionFromYawPitchRoll(float yaw, float pitch, float roll) {
	const float halfYaw = yaw * 0.5f;
	const float halfPitch = pitch * 0.5f;
	const float halfRoll = roll * 0.5f;

	const float cy = std::cos(halfYaw);
	const float sy = std::sin(halfYaw);
	const float cp = std::cos(halfPitch);
	const float sp = std::sin(halfPitch);
	const float cr = std::cos(halfRoll);
	const float sr = std::sin(halfRoll);

	Quaternion q;
	q.w = cr * cp * cy + sr * sp * sy;  // = cos(R/2)cos(P/2)cos(Y/2) + sin(R/2)sin(P/2)sin(Y/2)
	q.x = sr * cp * cy - cr * sp * sy;
	q.y = cr * sp * cy + sr * cp * sy;
	q.z = cr * cp * sy - sr * sp * cy;

	return Normalize(q);                // MAGIMath::Normalize で長さ1に
}

Quaternion MAGIMath::Slerp(Quaternion q1, Quaternion q2, float t) {
	// クォータニオンの内積を計算
	float dot = q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;

	// ドット積が負の場合、逆の方向に補間するために q1 を反転
	if (dot < 0.0f) {
		q1.x = -q1.x;
		q1.y = -q1.y;
		q1.z = -q1.z;
		q1.w = -q1.w;
		dot = -dot;
	}

	if (dot >= 1.0f - EPSILON) {
		Quaternion result{};
		result.x = (1.0f - t) * q1.x + t * q2.x;
		result.y = (1.0f - t) * q1.y + t * q2.y;
		result.z = (1.0f - t) * q1.z + t * q2.z;
		result.w = (1.0f - t) * q1.w + t * q2.w;

		return Normalize(result);
	}

	// 角度の計算
	float theta_0 = std::acos(dot);        // θ0 = q1 と q2 間の角度
	float theta = theta_0 * t;             // θ = t に対応する角度

	// 係数の計算
	float sin_theta = std::sin(theta);
	float sin_theta_0 = std::sin(theta_0);

	float s1 = std::cos(theta) - dot * sin_theta / sin_theta_0;
	float s2 = sin_theta / sin_theta_0;

	// 補間結果の計算
	Quaternion result = {
		s1 * q1.x + s2 * q2.x,
		s1 * q1.y + s2 * q2.y,
		s1 * q1.z + s2 * q2.z,
		s1 * q1.w + s2 * q2.w
	};
	return Normalize(result);
}
