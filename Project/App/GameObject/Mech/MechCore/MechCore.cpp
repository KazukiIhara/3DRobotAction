#include "MechCore.h"

#include "MAGIAssert/MAGIAssert.h"

MechCore::MechCore() {
	// ゲームオブジェクトを作成


	// ステートを作成


	// コンポーネントを作成


	// パーツを作成


}

void MechCore::Update() {
	// ステートごとの更新
	if (auto cs = currentState_.second.lock()) {
		cs->Update(this);
	}

	// 各パーツを更新


	// 各パーツの更新後のCoreの更新


}

void MechCore::ChangeState(MechCoreState nextState) {
	// 旧ステートの終了処理
	if (auto cs = currentState_.second.lock()) {
		cs->Exit(this);
	}

	// 変更後ステートの開始処理
	currentState_ = std::make_pair(nextState, GetState(nextState));
	if (auto cs = currentState_.second.lock()) {
		cs->Enter(this);
	}
}

std::weak_ptr<GameObject3D> MechCore::GetGameObject() const {
	return core_;
}

const MechCoreState& MechCore::GetCurrentState() const {
	return currentState_.first;
}

const Vector3& MechCore::GetVelocity()const {
	return velocity_;
}

const InputCommand& MechCore::GetInputCommand() const {
	return inputCommand_;
}

void MechCore::SetVelocity(const Vector3& velocity) {
	velocity_ = velocity;
}

void MechCore::SetInputCommand(InputCommand command) {
	inputCommand_ = command;
}

std::weak_ptr<MechCoreBaseState> MechCore::GetState(MechCoreState state) {
	auto it = states_.find(state);
	if (it != states_.end()) {
		return it->second;
	}

	MAGIAssert::Assert(false, "Not find MechCoreState!");
	return {};
}
