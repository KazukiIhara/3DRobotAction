#pragma once

#include "Cameras3D/Camera3D/Camera3D.h"
#include "Transform3D/Transform3D.h"

class MechCore;

enum PlayerCameraState {
	Initialize,
	Root,
};

class MechCamera:public Camera3D {
public:
	MechCamera(const std::string& name, float yaw);
	~MechCamera() = default;

	void Update()override;

	void SetTargetTransform(Transform3D* target);

	void SetMechCore(std::weak_ptr<MechCore> mechCore);

	void SetCameraQuaternion(const Quaternion& q);

	const Quaternion& GetCameraQuaternion()const;

private:
	void ApplyInput(float dt);
	void HardLockCamera(float dt);
	void FollowCamera();
private:
	// プレイヤーカメラの状態
	PlayerCameraState state_;

	// 追従対象のトランスフォーム
	Transform3D* followTargetTransform_ = nullptr;

	// 累積用変数
	float pYaw_ = 0.0f;
	float pPitch_ = 0.0f;

	// カメラの回転
	Quaternion cameraRotation_;

	// ピボット
	Vector3 pivot_ = { 0.0f,0.0f,0.0f };

	// 目標ピボット
	Vector3 targetPivot_ = { 0.0f,0.0f,0.0f };
	// 目標目線座標
	Vector3 targetEye_ = { 0.0f,0.0f,0.0f };
	// 目標目標座標
	Vector3	targetTarget_ = { 0.0f,0.0f,1.0f };

	// パラメータ

	// オフセットの半径
	float radius_ = 3.0f;

	// カメラの感度
	float sensYaw_ = 8.0f;
	float sensPitch_ = 6.0f;

	// カメラ補間速度
	const float kPivotLag_ = 0.1f;

	// ターゲットにカメラを向ける速度
	const float kHardLockTargetLag_ = 0.25f;
	const float kHardLockRotLag_ = 0.25f;

	// ピッチの上限角度
	const float kPitchLim_ = 75.0f * std::numbers::pi_v<float> / 180.0f;

	// ピボットのオフセット
	Vector3 pivotOffset_ = { 0.0f, 2.2f, 0.0f };

	// 機体を受け取る
	std::weak_ptr<MechCore> core_;


	//
	// カメラシェイク用のステータス
	//
	

	
};