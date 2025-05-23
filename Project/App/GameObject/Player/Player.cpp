#include "Player.h"

#include "Framework/MAGI.h"

Player::Player() {
	// トランスフォーム初期化
	transform_ = std::make_unique<Transform3D>();
	renderTransform_ = std::make_unique<Transform3D>(
		Vector3(1.0f, 1.0f, 1.0f),
		Vector3(0.0f, 0.0f, 0.0f),
		Vector3(0.0f, 1.0f, 0.0f)
	);

	// 描画用トランスフォームと親子付け
	renderTransform_->SetParent(transform_.get());
}

void Player::Update() {
	// 親だけ更新　TODO:TransformManagerを作る
	transform_->Update();

}

void Player::Draw() {
	MAGISYSTEM::DrawBox3D(renderTransform_->GetWorldMatrix(), BoxData3D{}, PrimitiveMaterialData3D{});
}
