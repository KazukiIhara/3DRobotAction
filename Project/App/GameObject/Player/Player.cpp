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

	// 破壊時エフェクトテスト
	if (MAGISYSTEM::PushKey(DIK_SPACE)) {
		breakEffect_ = std::make_unique<BreakEffect>(MAGIMath::ExtractionWorldPos(transform_->GetWorldMatrix()));
	}

	if (breakEffect_) {
		// 終わってたら抜ける
		if (breakEffect_->IsFinished()) {
			breakEffect_.reset();
		} else {
			breakEffect_->Update();
		}
	}
}

void Player::Draw() {
	if (breakEffect_) {
		breakEffect_->Draw();
	}
	MAGISYSTEM::DrawBox3D(renderTransform_->GetWorldMatrix(), BoxData3D{}, PrimitiveMaterialData3D{});
}
