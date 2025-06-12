#include "Player.h"

#include "Framework/MAGI.h"

Player::Player(const std::string& name) :GameObject3D(name) {
	std::shared_ptr<ModelRenderer> playerModel = std::make_shared<ModelRenderer>("Player", "teapot");
	playerModel->GetTransform()->SetTranslate(Vector3(0.0f, 1.0f, 0.0f));
	AddModelRenderer(std::move(playerModel));
}

void Player::Update() {

	Vector2 stick{};
	if (MAGISYSTEM::IsPadConnected(0)) {
		stick.x = MAGISYSTEM::GetLeftStickX(0);
		stick.y = MAGISYSTEM::GetLeftStickY(0);
	}

	velocity_ = Normalize(Vector3(stick.x, 0.0f, stick.y));
	velocity_ *= speed_ * MAGISYSTEM::GetDeltaTime();

	GetTransform()->AddTranslate(velocity_);

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
