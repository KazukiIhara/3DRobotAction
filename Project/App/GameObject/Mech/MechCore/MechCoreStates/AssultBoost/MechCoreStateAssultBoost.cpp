#include "MechCoreStateAssultBoost.h"

#include "GameObject/Mech/MechCore/MechCore.h"

#include "Math/Utility/MathUtility.h"

using namespace MAGIMath;

void MechCoreStateAssultBoost::Enter([[maybe_unused]] MechCore* mechCore) {
	// アサルトブースト初期化
	mechCore->GetMovementComponent()->AssultBoostEnter(mechCore);
}

void MechCoreStateAssultBoost::Update([[maybe_unused]] MechCore* mechCore) {
	// コマンドを取得
	const InputCommand command = mechCore->GetInputCommand();

	// キャンセル行動
	if (command.quickBoost && Length(command.moveDirection)) { // クイックブーストでキャンセル
		mechCore->ChangeState(MechCoreState::QuickBoost);
		return;
	} else if (command.jump) { // ジャンプでキャンセル
		mechCore->ChangeState(MechCoreState::Idle);
		return;
	}

	// 更新
	mechCore->GetMovementComponent()->AssultBoostUpdate(mechCore);

}

void MechCoreStateAssultBoost::Exit([[maybe_unused]] MechCore* mechCore) {

}
