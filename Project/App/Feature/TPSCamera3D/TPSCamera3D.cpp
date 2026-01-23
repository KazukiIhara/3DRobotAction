#include "TPSCamera3D.h"

// C++
#include <algorithm>
#include <cmath>

#include "Framework/MAGI.h"
#include "3D/Transform3D/Transform3D.h"
#include "Feature/ILockOnTarget/ILockOnTarget.h"

// Math
#include "Math/Utility/MathUtility.h"

using namespace MAGIMath;

namespace {
	float Clamp(float v, float a, float b) {
		return std::clamp(v, a, b);
	}

	Vector3 ExpSmoothing(const Vector3& current, const Vector3& target, float sharpness, float dt) {
		const float t = 1.0f - std::exp(-sharpness * dt);
		return Lerp(current, target, t);
	}

	// [-pi, pi] に畳む
	float WrapPi(float a) {
		const float twoPi = 2.0f * std::numbers::pi_v<float>;
		a = std::fmod(a + std::numbers::pi_v<float>, twoPi);
		if (a < 0.0f) {
			a += twoPi;
		}
		return a - std::numbers::pi_v<float>;
	}

	// 角度の指数補間
	float ExpSmoothingAngle(float current, float target, float sharpness, float dt) {
		const float t = 1.0f - std::exp(-sharpness * dt);

		// 最短差分で補間
		const float delta = WrapPi(target - current);
		return current + delta * t;
	}
}

TPSCamera3D::TPSCamera3D(const std::string& name)
	: Camera3D(name, false) {
	// yaw/pitch で target を生成しない
	SetIsUseYawPitch(false);

	// 初期値
	smoothedEye_ = GetEye();
	smoothedTarget_ = GetTarget();

	// パラメータロード
	LoadParam();
}

void TPSCamera3D::Update() {
	const float dt = MAGISYSTEM::GetDeltaTime();

	if (!follow_) {
		UpdateData();
		return;
	}

	// パラメータロード
	LoadParam();

	// 追従点
	const Vector3 pivot = CalcPivot();

	// ロックオン角度更新
	if (lockOn_) {
		UpdateLockOnAngles(pivot, dt);
	} else {
		// ロックオン解除時はオフセットを戻す
		if (wasLockOn_) {
			ResetLockOffset();
		}
		wasLockOn_ = false;
	}

	// 注視点
	const Vector3 desiredLookAt = CalcLookAt(pivot);

	// カメラ位置
	const Vector3 desiredEye = CalcDesiredEye(pivot);

	// 平滑化
	smoothedTarget_ = ExpSmoothing(smoothedTarget_, desiredLookAt, param_.targetSmooth, dt);
	smoothedEye_ = ExpSmoothing(smoothedEye_, desiredEye, param_.posSmooth, dt);

	// 反映
	SetTarget(smoothedTarget_);
	SetEye(smoothedEye_);

	UpdateData();
}

void TPSCamera3D::LoadParam() {
	// 距離/オフセット
	param_.distance = MAGISYSTEM::GetParameterValue<float>({ "PilotCameraParam", "Distance" });
	param_.height = MAGISYSTEM::GetParameterValue<float>({ "PilotCameraParam", "Height" });
	param_.shoulder = MAGISYSTEM::GetParameterValue<float>({ "PilotCameraParam", "Shoulder" });

	// 回転速度
	param_.yawSpeed = MAGISYSTEM::GetParameterValue<float>({ "PilotCameraParam", "YawSpeed" });
	param_.pitchSpeed = MAGISYSTEM::GetParameterValue<float>({ "PilotCameraParam", "PitchSpeed" });

	// ピッチ制限
	param_.pitchMin = MAGISYSTEM::GetParameterValue<float>({ "PilotCameraParam", "PitchMin" });
	param_.pitchMax = MAGISYSTEM::GetParameterValue<float>({ "PilotCameraParam", "PitchMax" });

	// 追従スムージング
	param_.posSmooth = MAGISYSTEM::GetParameterValue<float>({ "PilotCameraParam", "PosSmooth" });
	param_.targetSmooth = MAGISYSTEM::GetParameterValue<float>({ "PilotCameraParam", "TargetSmooth" });
}

void TPSCamera3D::SetFollowTransform(Transform3D* follow) {
	follow_ = follow;

	if (!follow_) {
		return;
	}

	// 追従点
	const Vector3 pivot = CalcPivot();

	// 初期注視点
	const Vector3 desiredLookAt = CalcLookAt(pivot);

	// 初期カメラ位置
	const Vector3 desiredEye = CalcDesiredEye(pivot);

	// 初期化
	smoothedTarget_ = desiredLookAt;
	smoothedEye_ = desiredEye;

	SetTarget(smoothedTarget_);
	SetEye(smoothedEye_);
}

void TPSCamera3D::SetLockOnTarget(ILockOnTarget* lockOn) {
	lockOn_ = lockOn;

	// オフセットをリセット
	ResetLockOffset();

	// 状態を更新
	wasLockOn_ = (lockOn_ != nullptr);
}

void TPSCamera3D::ResetLockOffset() {
	// 入力オフセットを戻す
	lockOffsetYaw_ = 0.0f;
	lockOffsetPitch_ = 0.0f;
}

void TPSCamera3D::AddYaw(float delta) {
	// ロックオン中はオフセットとして加算
	if (lockOn_) {
		lockOffsetYaw_ += delta * param_.yawSpeed;
		return;
	}

	SetYaw(GetYaw() + delta * param_.yawSpeed);
}

void TPSCamera3D::AddPitch(float delta) {
	// ロックオン中はオフセットとして加算
	if (lockOn_) {
		lockOffsetPitch_ += delta * param_.pitchSpeed;
		return;
	}

	float p = GetPitch() + delta * param_.pitchSpeed;
	p = Clamp(p, param_.pitchMin, param_.pitchMax);
	SetPitch(p);
}

TPSCamera3D::Param& TPSCamera3D::GetParam() {
	return param_;
}
const TPSCamera3D::Param& TPSCamera3D::GetParam() const {
	return param_;
}

Vector3 TPSCamera3D::CalcPivot() const {
	// 基準点
	Vector3 pivot = follow_->GetWorldPosition();
	// 高さを足す
	pivot.y += param_.height;
	return pivot;
}

Vector3 TPSCamera3D::CalcLookAt(const Vector3& pivot) const {
	// 通常は追従点を見る
	Vector3 lookAt = pivot;

	// ロックオン時は中間点を見る
	if (lockOn_) {
		const Vector3 lockPos = lockOn_->GetCenterPos();
		lookAt = Lerp(pivot, lockPos, 0.5f);
	}

	return lookAt;
}

void TPSCamera3D::UpdateLockOnAngles(const Vector3& pivot, float dt) {
	// 敵方向
	Vector3 toEnemy = lockOn_->GetCenterPos() - pivot;

	// 退化ガード
	if (LengthSquared(toEnemy) < 1e-8f) {
		return;
	}

	// 正規化
	toEnemy = Normalize(toEnemy);

	// yaw を算出
	const float desiredYaw = std::atan2(toEnemy.x, toEnemy.z);

	// pitch を算出
	const float h = std::sqrt(toEnemy.x * toEnemy.x + toEnemy.z * toEnemy.z);
	float desiredPitch = std::atan2(toEnemy.y, h);

	// オフセット加算
	const float yawWithOffset = desiredYaw + lockOffsetYaw_;
	const float pitchWithOffset = Clamp(desiredPitch + lockOffsetPitch_, param_.pitchMin, param_.pitchMax);

	// 角度追従の強さ
	const float kAngleSharpness = 12.0f;

	// 角度を平滑化
	SetYaw(ExpSmoothingAngle(GetYaw(), yawWithOffset, kAngleSharpness, dt));
	SetPitch(ExpSmoothingAngle(GetPitch(), pitchWithOffset, kAngleSharpness, dt));
}

Vector3 TPSCamera3D::CalcDesiredEye(const Vector3& pivot) const {
	// yaw/pitch から forward を作る
	const Vector3 fwd = DirectionFromYawPitch(GetYaw(), GetPitch());

	// ワールドUp固定
	const Vector3 up = Vector3{ 0.0f, 1.0f, 0.0f };

	// right を作る
	Vector3 right = Cross(up, fwd);
	if (LengthSquared(right) < 1e-8f) {
		right = Vector3{ 1.0f, 0.0f, 0.0f };
	}
	right = Normalize(right);

	// 肩越しオフセット
	const Vector3 shoulder = right * param_.shoulder;

	// 後方オフセット
	const Vector3 back = -Normalize(fwd) * param_.distance;

	// 目線の高さ補正
	const Vector3 eyeUp = up * (param_.height * 0.15f);

	return pivot + back + shoulder + eyeUp;
}
