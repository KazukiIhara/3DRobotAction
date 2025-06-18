#include "MechCore.h"

#include "MAGIAssert/MAGIAssert.h"

MechCore::MechCore() {
	// ゲームオブジェクトを作成


	// ステートを作成


	// コンポーネントを作成


	// パーツを作成



}

void MechCore::Update() {
	if (auto cs = currentState_.second.lock()) {
		cs->Update(this);
	}
}

void MechCore::ChangeState(MechCoreState nextState) {
	if (auto cs = currentState_.second.lock()) {
		cs->Exit(this);
	}
	currentState_ = std::make_pair(nextState, GetState(nextState));

	if (auto cs = currentState_.second.lock()) {
		cs->Enter(this);
	}
}

const MechCoreState& MechCore::GetCurrentState() const {
	return currentState_.first;
}

const Vector3& MechCore::GetVelocity()const {
	return velocity_;
}

void MechCore::SetVelocity(const Vector3& velocity) {
	velocity_ = velocity;
}

std::weak_ptr<MechCoreBaseState> MechCore::GetState(MechCoreState state) {
	auto it = states_.find(state);
	if (it != states_.end()) {
		return it->second;
	}

	MAGIAssert::Assert(false, "Not find MechCoreState!");
	return {};
}
