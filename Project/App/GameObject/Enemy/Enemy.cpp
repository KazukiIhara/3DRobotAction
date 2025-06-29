#include "Enemy.h"

Enemy::Enemy() {
	// 機体の作成
	mech_ = std::make_unique<MechCore>();

}

void Enemy::Update() {
	// コマンド
	InputCommand command{};

	// コマンドセット
	mech_->SetInputCommand(command);

	// 機体更新
	mech_->Update();
}

MechCore* Enemy::GetMechCore() {
	return mech_.get();
}