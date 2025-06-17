#pragma once

#include "Cameras3D/Camera3D/Camera3D.h"

#include "Transform3D/Transform3D.h"

class ThirdPersonCamera :public Camera3D {
public:
	ThirdPersonCamera();
	~ThirdPersonCamera()override;

	void Initialize()override;
	void Update()override;

	void SetTargetTransform(Transform3D* target);

private:
	Transform3D* targetTransform_;

	// パラメータ
	float radius_ = 5.0f;
	float yawSpeed_ = 2.0f;
	float pitchSpeed_ = 2.0f;
	float followLag_ = 3.0f;
	float shoulderSign_ = 1.0f; // 1:右肩 -1:左肩
	Vector3 pivotOffset_ = { 0.0f, 1.6f, 0.0f }; // プレイヤー胸元くらい

};