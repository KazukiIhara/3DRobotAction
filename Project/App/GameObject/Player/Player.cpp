#include "Player.h"

#include "MAGI.h"
#include "MAGIAssert/MAGIAssert.h"

#include "GameObject/PlayerCamera/PlayerCamera.h"

Player::Player(BulletManager* bulletManager) {
	// 機体の作成
	mech_ = std::make_shared<MechCore>(bulletManager);

	// 三人称視点カメラの作成
	std::shared_ptr<PlayerCamera> followCamera = std::make_shared<PlayerCamera>("MainCamera");
	followCamera->SetIsUnique(true);
	followCamera->SetTargetTransform(mech_->GetGameObject().lock()->GetTransform());
	followCamera->SetMechCore(mech_);
	followCamera->ApplyCurrent();

	// カメラを追加
	if (auto mechObj = mech_->GetGameObject().lock()) {
		mechObj->AddCamera3D(followCamera);
	}


	// プレイヤーUIクラス作成
	playerUI_ = std::make_unique<PlayerUI>();

}

void Player::Update() {
	InputCommand command{};
	Vector2 lStick{};
	Vector3 forward{};
	Vector3 right{};
	Vector2 moveDir{};

	//===========================
	// 入力を取得
	//===========================

	// パッド接続確認
	if (MAGISYSTEM::IsPadConnected(0)) {
		// スティックによる移動入力
		lStick = MAGISYSTEM::GetLeftStick(0);

		// スティックによるカメラ入力
		command.cameraRotDirection = MAGISYSTEM::GetRightStick(0);

		// ジャンプ入力
		command.jump = MAGISYSTEM::PushButton(0, ButtonL);

		// クイックブースト入力
		command.quickBoost = MAGISYSTEM::TriggerButton(0, ButtonR);

		// ロックオンモード切り替え
		command.switchHardLock = MAGISYSTEM::TriggerButton(0, ButtonRightStick);

		// 左手武器攻撃
		command.leftHandWeapon = MAGISYSTEM::GetLeftTrigger(0) >= 0.3f;

		// 右手武器攻撃
		command.rightHandWeapon = MAGISYSTEM::GetRightTrigger(0) >= 0.3f;


	} else { // パッドなしならキーボード入力解禁
		// 移動入力
		if (MAGISYSTEM::PushKey(DIK_W)) lStick.y += 1.0f;
		if (MAGISYSTEM::PushKey(DIK_A)) lStick.x -= 1.0f;
		if (MAGISYSTEM::PushKey(DIK_S)) lStick.y -= 1.0f;
		if (MAGISYSTEM::PushKey(DIK_D)) lStick.x += 1.0f;

		// ジャンプ入力
		command.jump = MAGISYSTEM::PushKey(DIK_SPACE);

		// クイックブースト入力
		command.quickBoost = MAGISYSTEM::TriggerKey(DIK_LSHIFT);

		// ロックオンモード切り替え
		command.switchHardLock = MAGISYSTEM::TriggerMouseButton(MouseButton::Middle);
	}

	// 移動入力があった場合
	if (Length(lStick)) {
		// カメラに対しての移動方向を計算
		if (auto cucam = MAGISYSTEM::GetCurrentCamera3D()) {
			forward = cucam->GetTarget() - cucam->GetEye();
			forward.y = 0.0f;
			forward = Normalize(forward);
			right = Normalize(Cross({ 0.0f,1.0f,0.0f }, forward));
			const Vector3 tempDir = Normalize(right * lStick.x + forward * lStick.y);
			moveDir = { tempDir.x, tempDir.z };
		} else {
			MAGIAssert::Assert(false, "Not found SceneCamera!");
		}
		// 移動方向のコマンドをセット
		command.moveDirection = Normalize(moveDir);
	}

	// コマンドセット
	mech_->SetInputCommand(command);

	// ロックオンコンポーネント用のカメラを作成、セット
	LockOnView lockOnView{};
	if (auto camera = dynamic_cast<PlayerCamera*>(mech_->GetGameObject().lock()->GetCamera3D("MainCamera").lock().get())) {
		lockOnView.eye = camera->GetEye();
		lockOnView.target = camera->GetTarget();
	}
	mech_->SetLockOnView(lockOnView);

	// 機体更新
	mech_->Update();


	// UI更新
	playerUI_->Update(mech_.get());


	//// 破壊時エフェクトテスト
	//if (ImGui::Button("PlayEffect")) {
	//	breakEffect_ = std::make_unique<BreakEffect>(mech_->GetGameObject().lock()->GetTransform()->GetWorldPosition());
	//}

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
	// UI描画
	playerUI_->Draw();

	if (breakEffect_) {
		breakEffect_->Draw();
	}

}

std::weak_ptr<MechCore> Player::GetMechCore() {
	return mech_;
}

