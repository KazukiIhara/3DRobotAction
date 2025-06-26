#include "MechCoreStateQuickBoost.h"

#include "GameObject/Mech/MechParts/MechCore/MechCore.h"

#include "MAGI.h"

using namespace MAGIMath;

void MechCoreStateQuickBoost::Enter(MechCore* mechCore) {
	// クイックブースト初期化
	mechCore->GetMovementComponent()->QuickBoostEnter(mechCore);

}

void MechCoreStateQuickBoost::Update(MechCore* mechCore) {
	// コマンド取得
	const InputCommand command = mechCore->GetInputCommand();

	// 終了通知があったら通常状態に移行
	if (mechCore->GetMovementComponent()->QuickBoostEndRequest()) {
		// 移動入力なし　→　待機状態に遷移
		if (!Length(command.moveDirection)) {
			mechCore->ChangeState(MechCoreState::Idle);
			return;
		} else {
			mechCore->ChangeState(MechCoreState::Move);
			return;
		}
	}

	// 更新
	mechCore->GetMovementComponent()->QuickBoostUpdate();
}

void MechCoreStateQuickBoost::Exit([[maybe_unused]] MechCore* mechCore) {

}
