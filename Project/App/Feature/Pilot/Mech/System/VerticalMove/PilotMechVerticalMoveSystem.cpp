#include "PilotMechVerticalMoveSystem.h"

#include <algorithm>

#include "Feature/Pilot/Mech/PilotMech.h"
#include "Feature/Mech/System/Move/MechMoveSystem.h"
#include "Feature/Mech/System/Kinematic/MechKinematicSystem.h"
#include "Feature/GameInputSystem/GameInputSystem.h"

#include "MAGI.h"

using namespace Magi;

PilotMechVerticalMoveSystem::PilotMechVerticalMoveSystem(PilotMech* mech) {
	mech_ = mech;
}

void PilotMechVerticalMoveSystem::PreUpdate() {
	if (!mech_) {
		return;
	}

	auto* ms = mech_->GetMoveSystem();
	auto* ks = mech_->GetKinematicSystem();
	if (!ms || !ks) {
		return;
	}

	const float dt = MAGISYSTEM::GetDeltaTime();

	// 重力加速度
	const float gravity = MAGISYSTEM::GetParameterValue<float>({ "PilotMechParam","VerticalMoveSystem","Gravity" });
	// ジャンプ開始時に与える上向き初速
	const float jumpFirstSpeed = MAGISYSTEM::GetParameterValue<float>({ "PilotMechParam","VerticalMoveSystem","JumpFirstSpeed" });
	// 長押し中に加える上向き加速度
	const float boostAccel = MAGISYSTEM::GetParameterValue<float>({ "PilotMechParam","VerticalMoveSystem","BoostAccel" });
	// 上昇速度の上限
	const float maxUpSpeed = MAGISYSTEM::GetParameterValue<float>({ "PilotMechParam","VerticalMoveSystem","MaxUpSpeed" });
	// 落下速度の上限
	const float maxFallSpeed = MAGISYSTEM::GetParameterValue<float>({ "PilotMechParam","VerticalMoveSystem","MaxFallSpeed" });

	const bool grounded = ks->IsGrounded();

	auto commandPair = mech_->GetInputSys()->GetPilotCommand();
	if (commandPair.first) {
		const auto& command = commandPair.second;

		const bool jumpPressed = command.jump;
		const bool jumpHold = command.jumpHold;

		// 接地中に押した瞬間は初速を与える
		if (grounded && jumpPressed) {
			vy_ = jumpFirstSpeed;
		}

		// 押している間はいつでも上昇加速
		if (jumpHold) {
			vy_ += boostAccel * dt;
		}

	}

	// 重力を掛ける
	vy_ -= gravity * dt;

	// 上昇・落下の速度を制限
	vy_ = std::min(vy_, maxUpSpeed);
	vy_ = std::max(vy_, -maxFallSpeed);

	// MoveSystemへ縦速度を反映
	ms->SetVerticalSpeed(vy_);
}

void PilotMechVerticalMoveSystem::PostUpdate() {
	if (!mech_) return;

	auto* ms = mech_->GetMoveSystem();
	auto* ks = mech_->GetKinematicSystem();
	if (!ms || !ks) return;

	// 接地したら落下を止める
	if (ks->IsGrounded() && vy_ < 0.0f) {
		vy_ = 0.0f;
		ms->SetVerticalSpeed(0.0f);
	}
}
