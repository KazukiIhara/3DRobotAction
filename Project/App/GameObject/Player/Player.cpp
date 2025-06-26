#include "Player.h"

#include "MAGI.h"
#include "MAGIAssert/MAGIAssert.h"

Player::Player() {
	// 機体の作成
	mech_ = std::make_unique<MechCore>();

	// 三人称視点カメラの作成
	std::shared_ptr<ThirdPersonCamera> mainCamera = std::make_shared<ThirdPersonCamera>("MainCamera");
	mainCamera->SetTargetTransform(mech_->GetGameObject().lock()->GetTransform());
	mainCamera->ApplyCurrent();

	if (auto mechObj = mech_->GetGameObject().lock()) {
		mechObj->AddCamera3D(mainCamera);
	}
}

void Player::Update() {

	InputCommand command{};
	Vector2 stick{};
	Vector3 forward{};
	Vector3 right{};
	Vector2 moveDir{};

	//===========================
	// 入力を取得
	//===========================

	// パッド接続確認
	if (MAGISYSTEM::IsPadConnected(0)) {
		// スティックによる移動入力
		stick.x = MAGISYSTEM::GetLeftStickX(0);
		stick.y = MAGISYSTEM::GetLeftStickY(0);

		// ジャンプ入力
		command.jump = MAGISYSTEM::PushButton(0, ButtonL);

		// クイックブースト入力
		command.quickBoost = MAGISYSTEM::TriggerButton(0, ButtonR);

	} else { // パッドなしならキーボード入力解禁
		// 移動入力
		if (MAGISYSTEM::PushKey(DIK_W)) stick.y += 1.0f;
		if (MAGISYSTEM::PushKey(DIK_A)) stick.x -= 1.0f;
		if (MAGISYSTEM::PushKey(DIK_S)) stick.y -= 1.0f;
		if (MAGISYSTEM::PushKey(DIK_D)) stick.x += 1.0f;

		// ジャンプ入力
		command.jump = MAGISYSTEM::PushKey(DIK_SPACE);

	}

	//===========================
	// コマンド作成
	//===========================

	// カメラに対しての移動方向を計算
	if (auto cucam = MAGISYSTEM::GetCurrentCamera3D()) {
		forward = cucam->GetTarget() - cucam->GetEye();
		forward.y = 0.0f;
		right = Normalize(Cross({ 0.0f,1.0f,0.0f }, forward));
		Vector3 tempDir = Normalize(right * stick.x + forward * stick.y);
		moveDir = { tempDir.x, tempDir.z };
	} else {
		MAGIAssert::Assert(false, "Not found SceneCamera!");
	}

	command.moveDirection = moveDir;

	// コマンドセット
	mech_->SetInputCommand(command);


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
