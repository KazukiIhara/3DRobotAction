#include "MechCore.h"

#include "MAGIAssert/MAGIAssert.h"

MechCore::MechCore() {
	// ゲームオブジェクトを作成


	// ステートを作成


	// コンポーネントを作成


	// パーツを作成



}

void MechCore::Update() {
	if (auto cs = currentState_.lock()) {
		cs->Update(this);
	}
}

void MechCore::ChangeState(MechCoreState nextState) {

}

std::weak_ptr<MechCoreBaseState> MechCore::GetState(MechCoreState state) {
	auto it = states_.find(state);
	if (it != states_.end()) {
		return it->second;
	}

	MAGIAssert::Assert(false, "Not find MechCoreState!");
	return {};
}
