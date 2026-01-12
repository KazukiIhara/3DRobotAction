#define NOMINMAX

#include "MechCoreStateQuickBoost.h"

#include "GameObject/Mech/MechCore/MechCore.h"

#include "MAGI.h"

using namespace MAGIMath;
using namespace Magi;

MechCoreStateQuickBoost::MechCoreStateQuickBoost() {

}

void MechCoreStateQuickBoost::Enter(MechCore* mechCore) {
	// クイックブースト初期化
	mechCore->GetMovementComponent()->QuickBoostEnter(mechCore);
	// エネルギー消費
	mechCore->GetStatusComponent()->UseQuickBoostEnergy();

	// ジャスト回避判定タイマー初期化
	justDodgeTimer_ = MAGISYSTEM::GetParameterValue<float>({ "MechCommonParam","JustDodge","CanJustDodgeTime" });
}

void MechCoreStateQuickBoost::Update(MechCore* mechCore) {
	// コマンド取得
	const InputCommand command = mechCore->GetInputCommand();

	// ジャスト回避判定かどうかの更新
	mechCore->GetStatusComponent()->SetCanJustDodge(CanJustDodge());

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
		// クイックブーストでキャンセル
		if (command.quickBoost &&
			Length(command.moveDirection) &&
			!mechCore->GetStatusComponent()->GetIsOverheat()) {

			mechCore->ChangeState(MechCoreState::QuickBoost);
			return;
		} else if (command.jump) { // ジャンプでキャンセル
			mechCore->ChangeState(MechCoreState::Idle);
			return;
		}
	}

	// 更新
	mechCore->GetMovementComponent()->QuickBoostUpdate(mechCore);
}

void MechCoreStateQuickBoost::Exit([[maybe_unused]] MechCore* mechCore) {

}

bool MechCoreStateQuickBoost::CanJustDodge() {
	bool justDodge{};
	// ジャスト回避タイマー更新
	justDodgeTimer_ -= MAGISYSTEM::GetDeltaTime();
	justDodgeTimer_ = std::max(0.0f, justDodgeTimer_);

	// ジャスト回避フラグ設定
	if (justDodgeTimer_) {
		justDodge = true;
	} else {
		justDodge = false;
	}
	return justDodge;
}
