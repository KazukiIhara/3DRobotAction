#include "Enemy.h"

#include "MAGI.h"
#include "MAGIAssert/MAGIAssert.h"

#include "GameObject/PlayerCamera/PlayerCamera.h"

Enemy::Enemy(BulletManager* bulletManager) {
	// 機体の作成
	mech_ = std::make_unique<MechCore>(FriendlyTag::EnemySide, bulletManager, true);

	// 三人称視点カメラの作成
	std::shared_ptr<PlayerCamera> followCamera = std::make_shared<PlayerCamera>("MainCamera");
	followCamera->SetIsUnique(true);
	followCamera->SetTargetTransform(mech_->GetGameObject().lock()->GetTransform());
	followCamera->SetMechCore(mech_);

	// カメラを追加
	if (auto mechObj = mech_->GetGameObject().lock()) {
		mechObj->AddCamera3D(followCamera);
	}

}

void Enemy::Update() {
	// コマンド
	InputCommand command{};

	// コマンドセット
	mech_->SetInputCommand(command);

	// ロックオンコンポーネント用のカメラを作成、セット
	LockOnView lockOnView{};
	if (auto camera = mech_->GetGameObject().lock()->GetCamera3D("MainCamera").lock()) {
		lockOnView.eye = camera->GetEye();
		lockOnView.target = camera->GetTarget();
	}
	mech_->SetLockOnView(lockOnView);

	// 機体更新
	mech_->Update();
}

std::weak_ptr<MechCore> Enemy::GetMechCore() {
	return mech_;
}