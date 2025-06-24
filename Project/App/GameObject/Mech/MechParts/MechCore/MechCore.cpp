#include "MechCore.h"

#include "MAGI.h"
#include "MAGIAssert/MAGIAssert.h" 

#include "MechCoreStates/MechCoreBaseState.h"
#include "MechCoreStates/Idle/MechCoreStateIdle.h"
#include "MechCoreStates/Move/MechCoreStateMove.h"

using namespace MAGIMath;

MechCore::MechCore() {
	// レンダラーとゲームオブジェクトを作成 TODO: 引数からオブジェクトを作るようにする
	std::shared_ptr<ModelRenderer> coreModel = std::make_shared<ModelRenderer>("Player", "teapot");
	coreModel->GetTransform()->SetTranslate(Vector3(0.0f, 1.0f, 0.0f));

	std::shared_ptr<GameObject3D> coreObject = std::make_shared<GameObject3D>("Player", Vector3(0.0f, 50.0f, 0.0f));
	coreObject->AddModelRenderer(std::move(coreModel));

	core_ = MAGISYSTEM::AddGameObject3D(std::move(coreObject));

	// パーツを作成



	// ステートを作成
	states_[MechCoreState::Idle] = std::make_shared<MechCoreStateIdle>();
	states_[MechCoreState::Move] = std::make_shared<MechCoreStateMove>();

	// 最初のステートを設定
	ChangeState(MechCoreState::Idle);

	// コンポーネントを作成
	movementComponent_ = std::make_unique<MechMovementComponent>();


}

void MechCore::Update() {
	// 状態を整理 (コマンドはMechCoreの更新前に外部からセットする)

	// 接地状態かどうかチェック
	movementComponent_->CheckOnGround(this);

	// ステートごとの更新
	if (auto cs = currentState_.second.lock()) {
		cs->Update(this);
	}

	// 各パーツを更新


	// コンポーネントを更新
	movementComponent_->Update(this);
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

const InputCommand& MechCore::GetInputCommand() const {
	return inputCommand_;
}

MechMovementComponent* MechCore::GetMovementComponent() {
	return movementComponent_.get();
}

void MechCore::SetInputCommand(const InputCommand& command) {
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
