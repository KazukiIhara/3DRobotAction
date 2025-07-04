#pragma once

#include "Cameras3D/Camera3D/Camera3D.h"

#include "Transform3D/Transform3D.h"

class MechCore;

class PlayerCamera:public Camera3D {
public:
	PlayerCamera(const std::string& name);
	~PlayerCamera() = default;

	void Update()override;

	void SetTargetTransform(Transform3D* target);

	void SetMechCore(std::weak_ptr<MechCore> mechCore);

	Vector3	GetTargetEye()const;
private:
	void ApplyInput(float dt);
	void HardLockOnCamera(float dt);
	void FollowCamera(float dt);

private:
	// 追従対象のトランスフォーム
	Transform3D* followTargetTransform_ = nullptr;

	// 累積用変数
	float pYaw_ = 0.0f;
	float pPitch_ = 0.0f;

	// カメラの回転
	Quaternion cameraRotation_;

	// ピボット
	Vector3 pivot_ = { 0.0f,0.0f,0.0f };
	// カメラの前方ベクトル
	Vector3	forward_ = { 0.0f,0.0f,1.0f };

	// 目標目線座標
	Vector3 targetEye_ = { 0.0f,0.0f,0.0f };
	// 目標目標座標
	Vector3	targetTarget_ = { 0.0f,0.0f,1.0f };

	// パラメータ

	// オフセットの半径
	float radius_ = 6.0f;

	// カメラの感度
	float sensYaw_ = 8.0f;
	float sensPitch_ = 6.0f;

	// カメラ補間速度
	const float kHardLockLag_ = 0.15f;
	const float kFollowLag_ = 0.05f;

	const float kPitchLim_ = 75.0f * std::numbers::pi_v<float> / 180.0f;

	Vector3 pivotOffset_ = { 0.0f, 2.0f, 0.0f };

	// 機体を受け取る
	std::weak_ptr<MechCore> core_;

};