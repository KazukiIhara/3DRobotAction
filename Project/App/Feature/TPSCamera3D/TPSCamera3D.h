#pragma once

#include "3D/Cameras3D/Camera3D/Camera3D.h"

// Forward
class Transform3D;

/// <summary>
/// TPS用 Camera3D 派生
/// eye/target を自前で計算して Camera3D の UpdateData に流す
/// </summary>
class TPSCamera3D : public Camera3D {
public:
	struct Param {
		float distance = 6.0f;
		float height = 2.0f;
		float shoulder = 0.6f;

		float yawSpeed = 2.5f;
		float pitchSpeed = 2.0f;

		float pitchMin = -1.2f;
		float pitchMax = 0.6f;

		float posSmooth = 14.0f;
		float targetSmooth = 18.0f;
	};

public:
	TPSCamera3D(const std::string& name);
	~TPSCamera3D() override = default;

	void Update() override;

	// パラメータ読み込み
	void LoadParam();

	// 追従
	void SetFollowTransform(Transform3D* follow);
	void SetLockOnTarget(Transform3D* lockOn);

	// 入力
	void AddYaw(float delta);
	void AddPitch(float delta);

	Param& GetParam();
	const Param& GetParam() const;

private:
	Vector3 CalcDesiredTarget() const;
	Vector3 CalcDesiredEye(const Vector3& desiredTarget) const;

private:
	Transform3D* follow_ = nullptr;
	Transform3D* lockOn_ = nullptr;

	Param param_{};

	Vector3 smoothedEye_{ 0.0f, 3.0f, -5.0f };
	Vector3 smoothedTarget_{ 0.0f, 0.0f, 0.0f };
};
