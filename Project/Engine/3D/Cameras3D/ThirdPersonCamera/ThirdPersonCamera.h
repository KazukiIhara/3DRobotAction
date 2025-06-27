#pragma once

#include "Cameras3D/Camera3D/Camera3D.h"

#include "Transform3D/Transform3D.h"

class ThirdPersonCamera :public Camera3D {
public:
	ThirdPersonCamera(const std::string& name);
	~ThirdPersonCamera()override;

	void Update()override;

	void SetTargetTransform(Transform3D* target);

private:
	Transform3D* followTargetTransform_ = nullptr;

	// パラメータ
	float radius_ = 5.0f;
	float yawSpeed_ = 5.0f;
	float pitchSpeed_ = 5.0f;

	float mouseYawSpeed_ = 1.0f;
	float mousePitchSpeed_ = 1.0f;

	float maxPitchDegrees_ = 10.0f;
	float minPitchDegrees_ = -60.0f;

	float followLagHorizontal_ = 0.2f;
	float followLagVertical_ = 0.05f;

	float shoulderSign_ = 0.0f;
	Vector3 pivotOffset_ = { 0.0f, 1.5f, 0.0f };
	Vector3 smoothedPivot_{};
};