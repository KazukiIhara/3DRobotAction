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

	for (size_t i = 0; i < 4; i++) {
		ringRotate_[i] = { 0.0f,0.0f,0.0f };
	}

}

void Player::Update() {

	ImGui::Begin("BreakEffectSetting");
	ImGui::DragFloat3("RingRotate0", &ringRotate_[0].x, 0.01f);
	ImGui::DragFloat3("RingRotate1", &ringRotate_[1].x, 0.01f);
	ImGui::DragFloat3("RingRotate2", &ringRotate_[2].x, 0.01f);
	ImGui::DragFloat3("RingRotate3", &ringRotate_[3].x, 0.01f);

	ImGui::ColorEdit4("RingColor", &color_.x);
	ImGui::End();

	// 親だけ更新　TODO:TransformManagerを作る
	transform_->Update();

	// 破壊時エフェクトテスト
	if (MAGISYSTEM::PushKey(DIK_SPACE)) {
		breakEffect_ = std::make_unique<BreakEffect>(MAGIMath::ExtractionWorldPos(renderTransform_->GetWorldMatrix()));
	}

	if (breakEffect_) {
		// 終わってたら抜ける
		if (breakEffect_->IsFinished()) {
			breakEffect_.reset();
		} else {
			//breakEffect_->SetRingRotates(ringRotate_);
			// breakEffect_->SetRingColor(color_);

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
