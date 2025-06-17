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
	float radius_ = 5.0f;       // 基本距離
	float minRadius_ = 1.5f;
	float maxRadius_ = 7.0f;
	float yawSpeed_ = 3.0f;       // 入力感度
	float pitchSpeed_ = 3.0f;
	float followLag_ = 10.0f;      // 0=追随即時、値↑でふわっと
	float shoulderSign_ = 1.0f;       // 1:右肩 -1:左肩
	Vector3 pivotOffset_ = { 0.0f, 1.6f, 0.0f }; // プレイヤー胸元くらい

};