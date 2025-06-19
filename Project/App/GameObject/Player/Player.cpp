#include "Player.h"

#include "Framework/MAGI.h"

Player::Player() {
	mech_ = std::make_unique<MechCore>();
}

void Player::Update() {
	//
	// 入力からコマンドを生成
	//

	InputCommand command{};
	Vector2 stick{};
	Vector3 forward{};
	Vector3 right{};
	Vector2 moveDir{};

	// パッド接続確認
	if (MAGISYSTEM::IsPadConnected(0)) {
		// スティック入力を取得
		stick.x = MAGISYSTEM::GetLeftStickX(0);
		stick.y = MAGISYSTEM::GetLeftStickY(0);

		// カメラに対しての移動方向を計算
		if (auto cucam = MAGISYSTEM::GetCurrentCamera3D().lock()) {
			forward = cucam->GetTarget() - cucam->GetEye();
			forward.y = 0.0f;
			right = Normalize(Cross({ 0.0f,1.0f,0.0f }, forward));
			Vector3 tempDir = Normalize(right * stick.x + forward * stick.y);
			moveDir = { tempDir.x, tempDir.z };
		}

		// 
		// コマンドを生成してセット
		// 
		command.moveDirection = moveDir;
		mech_->SetInputCommand(command);
	}

	// 機体更新
	mech_->Update();


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

MechCore* Player::GetMechCore() {
	return mech_.get();
}
