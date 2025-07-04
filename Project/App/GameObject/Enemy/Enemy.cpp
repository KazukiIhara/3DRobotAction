#include "Enemy.h"

#include "MAGI.h"
#include "MAGIAssert/MAGIAssert.h"

Enemy::Enemy() {
	// 機体の作成
	mech_ = std::make_unique<MechCore>();

}

void Enemy::Update() {
	// コマンド
	InputCommand command{};

	command.moveDirection = { 0.0f,1.0f };

	ImGui::Begin("EnemyBodyWPos");
	Vector3 mechBWpos = mech_->GetMechBody()->GetGameObject().lock()->GetTransform()->GetWorldPosition();
	ImGui::DragFloat3("", &mechBWpos.x);
	ImGui::End();
	ImGui::End();
	
	// コマンドセット
	mech_->SetInputCommand(command);

	// 機体更新
	mech_->Update();
}

std::weak_ptr<MechCore> Enemy::GetMechCore() {
	return mech_;
}