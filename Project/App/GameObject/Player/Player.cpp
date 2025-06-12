#include "Player.h"

#include "Framework/MAGI.h"

Player::Player() {
	std::shared_ptr<ModelRenderer> playerModel = std::make_shared<ModelRenderer>("Player", "teapot");
	std::shared_ptr<GameObject3D> obj = std::make_shared<GameObject3D>("Player");
	obj->AddModelRenderer(std::move(playerModel));
	playerObject_ = MAGISYSTEM::AddGameObject3D(std::move(obj));
}

void Player::Update() {
	// 破壊時エフェクトテスト
	if (ImGui::Button("PlayEffect")) {
		breakEffect_ = std::make_unique<BreakEffect>(Vector3(0.0f, 0.0f, 0.0f));
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

}
