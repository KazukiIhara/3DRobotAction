#include "Player.h"

#include "Framework/MAGI.h"

Player::Player() {
	std::shared_ptr<ModelRenderer> playerModel = std::make_shared<ModelRenderer>("Player", "teapot");
	playerModel->GetTransform()->SetTranslate(Vector3(0.0f, 1.0f, 0.0f));

	std::shared_ptr<GameObject3D> gameObject = std::make_shared<GameObject3D>("Player");
	gameObject->AddModelRenderer(std::move(playerModel));

	gameObject_ = MAGISYSTEM::AddGameObject3D(std::move(gameObject));
}

void Player::Update() {
	Vector2 stick{};
	Vector3 forward{};
	Vector3 right{};
	Vector3 moveDir{};

	if (MAGISYSTEM::IsPadConnected(0)) {
		stick.x = MAGISYSTEM::GetLeftStickX(0);
		stick.y = MAGISYSTEM::GetLeftStickY(0);
	}

	if (auto cucam = MAGISYSTEM::GetCurrentCamera3D().lock()) {
		forward = cucam->GetTarget() - cucam->GetEye();
		forward.y = 0.0f;
		right = Normalize(Cross({ 0.0f,1.0f,0.0f }, forward));
		moveDir = Normalize(right * stick.x + forward * stick.y);
		velocity_ = moveDir * speed_ * MAGISYSTEM::GetDeltaTime();
	}

	if (auto obj = gameObject_.lock()) {
		obj->GetTransform()->AddTranslate(velocity_);
	}

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

std::weak_ptr<GameObject3D> Player::GetGameObject() {
	return gameObject_;
}
