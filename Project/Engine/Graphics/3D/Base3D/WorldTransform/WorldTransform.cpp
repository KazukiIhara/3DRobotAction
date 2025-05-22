#include "WorldTransform.h"

using namespace MAGIMath;

void WorldTransform::Initialize() {
	scale_ = { 1.0f,1.0f,1.0f };
	rotate_ = { 0.0f,0.0f,0.0f };
	translate_ = { 0.0f,0.0f,0.0f };

	rotateQuaternion_ = MakeIdentityQuaternion();

	worldMatrix_ = MakeIdentityMatrix4x4();
}

void WorldTransform::Update() {
	// 変更チェック
	bool isChanged = (scale_ != preScale_ || rotate_ != preRotate_ || translate_ != preTranslate_);
	if (!isChanged && !parent_) {
		return; // 変更がなければスキップ
	}

	// クオータニオンorオイラー
	UpdateEuler();

	// 親がいる場合の計算
	if (parent_) {
		worldMatrix_ = worldMatrix_ * parent_->worldMatrix_;
	}

	// 前フレームの値を保存
	preScale_ = scale_;
	preRotate_ = rotate_;
	preTranslate_ = translate_;
}

void WorldTransform::UpdateEuler() {
	// アフィン行列作成
	worldMatrix_ = MakeAffineMatrix(scale_, rotate_, translate_);
}

void WorldTransform::UpdateQuaternion() {
	// 回転処理
	if (rotate_.x != preRotate_.x || rotate_.y != preRotate_.y || rotate_.z != preRotate_.z) {
		RotateQuaternion();
	}
	// アフィン行列作成
	worldMatrix_ = MakeAffineMatrix(scale_, rotateQuaternion_, translate_);
	// 回転量計算用変数に挿入
	preRotate_ = rotate_;
}

void WorldTransform::RotateQuaternion() {
	if (rotate_.x == 0.0f && rotate_.y == 0.0f && rotate_.z == 0.0f) {
		rotateQuaternion_ = MakeIdentityQuaternion();
	} else {
		// 回転量を取得
		Vector3 deltaRotate = rotate_ - preRotate_;
		// 各軸の回転をクォータニオンに変換
		Quaternion rotationX = MakeRotateAxisAngleQuaternion(MakeRightVector3(), deltaRotate.x);
		Quaternion rotationY = MakeRotateAxisAngleQuaternion(MakeUpVector3(), deltaRotate.y);
		Quaternion rotationZ = MakeRotateAxisAngleQuaternion(MakeForwardVector3(), deltaRotate.z);

		// 回転の合成（順序は必要に応じて変更可能）
		Quaternion deltaRotation = rotationX * rotationY * rotationZ;

		// 現在の回転に適用
		rotateQuaternion_ = deltaRotation * rotateQuaternion_;

		// クォータニオンを正規化
		rotateQuaternion_ = Normalize(rotateQuaternion_);
	}
}