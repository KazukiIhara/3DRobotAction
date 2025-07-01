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
		if (!Length(command.moveDirection)) {	// 移動入力なし　→　待機状態に遷移
			mechCore->ChangeState(MechCoreState::Idle);
			return;
		} else {	// 移動入力あり　→　移動状態に遷移
			mechCore->ChangeState(MechCoreState::Move);
			return;
		}
	}

	// キャンセル行動
	if (mechCore->GetMovementComponent()->QuickBoostEnableCancel()) {
		if (command.quickBoost) { // クイックブーストでキャンセル
			mechCore->ChangeState(MechCoreState::QuickBoost);
			return;
		} else if (command.jump) { // ジャンプでキャンセル
			mechCore->ChangeState(MechCoreState::Idle);
			return;
		}
	}

	// 更新
	mechCore->GetMovementComponent()->QuickBoostUpdate();

	// 重力による移動量計算
	mechCore->GetMovementComponent()->CulGravityVelocity(mechCore);
}

void MechCoreStateQuickBoost::Exit(MechCore* mechCore) {
	// 重力による移動量計算
	mechCore->GetMovementComponent()->CulGravityVelocity(mechCore);
}
