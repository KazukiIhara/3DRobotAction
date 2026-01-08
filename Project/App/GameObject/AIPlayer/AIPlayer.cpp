#include "AIPlayer.h"

#include "MAGI.h"

#include "MAGIAssert/MAGIAssert.h"

#include "GameObject/PlayerCamera/PlayerCamera.h"

AIPlayer::AIPlayer(AttackObjectManager* bulletManager) {
	// 機体の作成
	const Vector3 kPopPosition = { 0.0f,0.0f,-30.0f };
	mech_ = std::make_unique<MechCore>(kPopPosition, FriendlyTag::PlayerSide, "Default", bulletManager, true);

	// 三人称視点カメラの作成
	std::unique_ptr<MechCamera> followCamera = std::make_unique<MechCamera>("MainCamera", std::numbers::pi_v<float>, mech_.get());
	followCamera->SetIsUnique(true);
	followCamera->SetTargetTransform(mech_->GetGameObject().lock()->GetTransform());
	followCamera->ApplyCurrent();

	// カメラを追加
	if (auto mechObj = mech_->GetGameObject().lock()) {
		mechObj->AddCamera3D(std::move(followCamera));
	}
}

void AIPlayer::Initialize(AttackObjectManager* bulletManager, std::weak_ptr<MechCore> targetMech) {
	// AIを作成
	ai_ = std::make_unique<EnemyAI>(mech_, targetMech, bulletManager);
}

void AIPlayer::Update() {
	// ロックオンコンポーネント用のカメラを作成、セット
	LockOnView lockOnView{};
	if (auto camera = mech_->GetGameObject().lock()->GetCamera3D("MainCamera")) {
		lockOnView.eye = camera->GetEye();
		lockOnView.target = camera->GetTarget();
	}
	mech_->SetLockOnView(lockOnView);

	// コマンド
	InputCommand command{};
	if (isAIActive_) {
		command = ai_->Update();
	}

	// コマンドセット
	mech_->SetInputCommand(command);

	// 機体更新
	mech_->Update();
}

void AIPlayer::Draw() {
	// エフェクトなど描画
	mech_->Draw();

}

void AIPlayer::SetIsAIActive(bool isActive) {
	isAIActive_ = isActive;
}

std::weak_ptr<MechCore> AIPlayer::GetMechCore() {
	return mech_;
}

bool AIPlayer::GetIsAIActive() const {
	return isAIActive_;
}
