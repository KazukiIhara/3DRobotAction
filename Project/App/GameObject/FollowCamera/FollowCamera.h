#pragma once

#include "3D/Cameras3D/Camera3D/Camera3D.h"

/// <summary>
/// 追尾カメラ
/// </summary>
class FollowCamera: public Camera3D {
public:
	FollowCamera(const std::string& cameraName);
	~FollowCamera()override = default;

	// 更新
	void Update() override;

	// カメラ操作
	void MoveCamera();

	// ターゲット追尾
	void FollowTarget();

	// 追従対象セット
	void SetTarget(const Transform3D* target);

private:
	// 追従対象のトランスフォーム
	Transform3D* followTarget_ = nullptr;

	// 追従対象からカメラまでのオフセット
	const Vector3 kOffset = { 0.0f, 2.5f, -15.0f };
};