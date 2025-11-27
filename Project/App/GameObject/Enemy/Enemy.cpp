#include "Enemy.h"

#include "MAGI.h"
#include "MAGIAssert/MAGIAssert.h"

#include "GameObject/PlayerCamera/PlayerCamera.h"

Enemy::Enemy(AttackObjectManager* bulletManager, std::weak_ptr<MechCore> playerMech) {
	// 機体の作成
	const Vector3 kEnemyPopPosition = { 0.0f,0.0f,30.0f };
	mech_ = std::make_unique<MechCore>(kEnemyPopPosition, FriendlyTag::EnemySide, bulletManager, true);

	// 三人称視点カメラの作成
	std::unique_ptr<MechCamera> followCamera = std::make_unique<MechCamera>("MainCamera", std::numbers::pi_v<float>, mech_.get());
	followCamera->SetIsUnique(true);
	followCamera->SetTargetTransform(mech_->GetGameObject().lock()->GetTransform());

	// カメラを追加
	if (auto mechObj = mech_->GetGameObject().lock()) {
		mechObj->AddCamera3D(std::move(followCamera));
	}

	// AIを作成
	ai_ = std::make_unique<EnemyAI>(mech_, playerMech, bulletManager);

}

void Enemy::Update() {
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

void Enemy::Draw() {
	// エフェクトなど描画
	mech_->Draw();

}

void Enemy::SetIsAIActive(bool isActive) {
	isAIActive_ = isActive;
}

std::weak_ptr<MechCore> Enemy::GetMechCore() {
	return mech_;
}

bool Enemy::GetIsAIActive() const {
	return isAIActive_;
}
