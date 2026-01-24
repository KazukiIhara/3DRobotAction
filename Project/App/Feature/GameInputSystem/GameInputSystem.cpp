#include "GameInputSystem.h"

#include "MAGI.h"

using namespace Magi;

GameInputSystem::GameInputSystem() {

}

void GameInputSystem::Update() {
	// プレイヤー操作コマンド更新
	UpdatePilotCommand();

}

std::pair<bool, GameInputSystem::PilotCommand> GameInputSystem::GetPilotCommand() const {
	return pilotCommand_;
}

void GameInputSystem::UpdatePilotCommand() {

	pilotCommand_.first = true;

	if (MAGISYSTEM::IsPadConnected(0)) {
		PilotCommand& pC = pilotCommand_.second;
		pC.common.StickL = MAGISYSTEM::GetLeftStick(0);
		pC.common.StickR = MAGISYSTEM::GetRightStick(0);

		pC.dodge = MAGISYSTEM::TriggerButton(0, ButtonR);

		pC.attackL = MAGISYSTEM::GetLeftTrigger(0);
		pC.attackR = MAGISYSTEM::GetRightTrigger(0);

		pC.switchLockOn = MAGISYSTEM::TriggerButton(0, ButtonRightStick);
	}

}
