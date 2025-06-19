#include "MechCoreStateIdle.h"

#include "GameObject/Mech/MechParts/MechCore/MechCore.h"

#include "Math/Utility/MathUtility.h"

using namespace MAGIMath;

void MechCoreStateIdle::Enter([[maybe_unused]] MechCore* mechCore) {

}

void MechCoreStateIdle::Update(MechCore* mechCore) {
	// コマンドを取得
	InputCommand command = mechCore->GetInputCommand();

	// 移動あり　→　移動状態に遷移
	if (Length(command.moveDirection)) {
		mechCore->ChangeState(MechCoreState::Move);
	}
}

void MechCoreStateIdle::Exit([[maybe_unused]] MechCore* mechCore) {

}
