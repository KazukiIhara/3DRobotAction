#pragma once

#include "Cameras3D/Camera3D/Camera3D.h"

#include "Transform3D/Transform3D.h"

class MechCore;

class PlayerCamera:public Camera3D {
public:
	PlayerCamera(const std::string& name);
	~PlayerCamera();

	void Update()override;

	void SetTargetTransform(Transform3D* target);

	void SetMechCore(std::weak_ptr<MechCore> mechCore);

private:
	void HardLockOnCamera(float dt);
	void FollowCamera(float dt);

private:
	// 追従対象のトランスフォーム
	Transform3D* followTargetTransform_ = nullptr;

	// パラメータ
	float radius_ = 5.5f;
	float yawSpeed_ = 5.0f;
	float pitchSpeed_ = 5.0f;

	float mouseYawSpeed_ = 1.0f;
	float mousePitchSpeed_ = 1.0f;

	float maxPitchDegrees_ = 10.0f;
	float minPitchDegrees_ = -60.0f;

	float followLagHorizontal_ = 0.2f;
	float followLagVertical_ = 0.05f;

	float rotLagYaw_ = 0.15f;   // 水平回転ラグ (秒)
	float rotLagPitch_ = 0.05f;   // 垂直回転ラグ (秒)

	float focusBias_ = 0.2f;

	float shoulderSign_ = 0.0f;
	Vector3 pivotOffset_ = { 0.0f, 1.8f, 0.0f };
	Vector3 smoothedPivot_{};

	// 機体を受け取る
	std::weak_ptr<MechCore> core_;

};