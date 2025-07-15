#include "Enemy.h"

#include "MAGI.h"
#include "MAGIAssert/MAGIAssert.h"

Enemy::Enemy(BulletManager* bulletManager) {
	// 機体の作成
	mech_ = std::make_unique<MechCore>(bulletManager);

}

void Enemy::Update() {
	// コマンド
	InputCommand command{};

	// コマンドセット
	mech_->SetInputCommand(command);

	// 機体更新
	mech_->Update();
}

std::weak_ptr<MechCore> Enemy::GetMechCore() {
	return mech_;
}