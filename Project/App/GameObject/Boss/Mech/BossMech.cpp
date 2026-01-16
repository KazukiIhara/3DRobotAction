#include "BossMech.h"

#include "MAGI.h"
#include "MAGIAssert/MAGIAssert.h" 

// ステートクラス
#include "GameObject/Boss/Mech/State/Idle/BossMechStateIdle.h"
#include "GameObject/Boss/Mech/State/LaserShot/BossMechStateLaserShot.h"

using namespace Magi;

BossMech::BossMech(const BossMech::InitParam& initParam) {
	// トランスフォーム作成
	std::unique_ptr<Transform3D> trans = std::make_unique<Transform3D>(initParam.position);
	transform_ = MAGISYSTEM::AddTransform3D(std::move(trans));

	// 各パーツ作成
	head_ = std::make_unique<BossMechHead>(initParam.head, this);
	body_ = std::make_unique<BossMechBody>(initParam.body, this);
	rightArm_ = std::make_unique<BossMechRightArm>(initParam.arm, this);
	leftArm_ = std::make_unique<BossMechLeftArm>(initParam.arm, this);
	rightLeg_ = std::make_unique<BossMechRightLeg>(initParam.leg, this);
	leftLeg_ = std::make_unique<BossMechLeftLeg>(initParam.leg, this);

	// 更新用コンテナに挿入
	parts_.push_back(head_.get());
	parts_.push_back(body_.get());      // Body 登録
	parts_.push_back(rightArm_.get());  // RightArm 登録
	parts_.push_back(leftArm_.get());   // LeftArm 登録
	parts_.push_back(rightLeg_.get());  // RightLeg 登録
	parts_.push_back(leftLeg_.get());

	// 武器を追加


	// 実装メモ
	/*
		胴体のみこのBossMech(コア)と親子付け
		そのほかのパーツはBodyのトランスフォームを親とする
		各パーツ内のさらに細かいパーツはパーツ作成時に親子付けする
	*/

	// 各パーツ結合
	body_->GetTransform()->SetParent(transform_);




	// ステートテーブル作成
	states_[BossMech::BossMechState::Idle] = std::make_unique<BossMechStateIdle>();
	states_[BossMech::BossMechState::LaserShot] = std::make_unique<BossMechStateLaserShot>();

	// 最初のステートを設定
	ChangeState(BossMechState::Idle);

	// パラメータの作成
	MAGISYSTEM::AddParameterGroup("BossMechState");

}

void BossMech::Update() {
	// ステート更新
	if (auto& state = currentState_.second) {
		state->Update(this);
	}

	// 全パーツを更新
	for (auto& part : parts_) {
		part->Update();
	}

	// 全武器を更新
	for (auto& weapon : weapons_) {
		weapon->Update();
	}

}

void BossMech::Draw([[maybe_unused]] bool isDebugDraw) {
#if defined (DEBUG) | (DEVELOP)
	// デバッグUIフラグ
	const bool debug = isDebugDraw;
	// デバッグUIを表示
	if (debug) {
		DebugDraw();
	}
#endif

	// 全パーツを描画
	for (auto part : parts_) {
		part->Draw();
	}

	// 全武器を描画
	for (auto& weapon : weapons_) {
		weapon->Draw();
	}

}

void BossMech::ChangeState(BossMech::BossMechState nextState) {
	// 旧ステートの終了処理
	if (auto cs = currentState_.second) {
		cs->Exit(this);
	}

	// 変更後ステートの開始処理
	currentState_ = std::make_pair(nextState, GetState(nextState));
	if (auto cs = currentState_.second) {
		cs->Enter(this);
	}
}

BossMechHead* BossMech::GetHead() {
	return head_.get();
}

BossMechBody* BossMech::GetBody() {
	return body_.get();
}

BossMechRightArm* BossMech::GetRightArm() {
	return rightArm_.get();
}

BossMechLeftArm* BossMech::GetLeftArm() {
	return leftArm_.get();
}

BossMechRightLeg* BossMech::GetRightLeg() {
	return rightLeg_.get();
}

BossMechLeftLeg* BossMech::GetLeftLeg() {
	return leftLeg_.get();
}

AttackObjectManager* BossMech::GetAttackObjectManager() {
	return attackObjectManager_;
}

GameEffectManager* BossMech::GetGameEffectManager() {
	return gameEffectManager_;
}

void BossMech::DebugDraw() {
	// デバッグウィンドウ描画処理
	ShowDebugWidow();

	// 各パーツのデバッグ描画
	for (auto part : parts_) {
		// パーツデバッグ描画
		if (debugFlag_.showPartsTransform) {
			part->DebugDraw();
		}
	}
}

BossMechBaseState* BossMech::GetState(BossMech::BossMechState state) {
	auto it = states_.find(state);
	if (it != states_.end()) {
		return it->second.get();
	}

	MAGIAssert::Assert(false, "Not find BossMechState!");
	return {};
}

const std::string BossMech::StateToString(BossMech::BossMechState state) {
	// ステートを文字列に変換
	std::string str = "";
	switch (state) {
		case BossMech::BossMechState::Idle:
			str = "Idle";
			break;
		case BossMech::BossMechState::LaserShot:
			str = "LaserShot";
			break;

		default:
			break;
	}
	return str;
}

void BossMech::ShowDebugWidow() {
	// デバッグ操作ウィンドウ
	ImGui::Begin("BossMech");

	ImGui::SeparatorText("Parameter");
	{
		ImGui::Text("CurrentState :");
		ImGui::SameLine();
		const std::string state = StateToString(currentState_.first);
		ImGui::Text(state.c_str());
	}

	ImGui::SeparatorText("SwitchState");
	{
		// 箱の高さ
		const float boxHeight = 100.0f;

		// スクロールできる箱（Child）
		ImGui::BeginChild("SwitchStateBox", ImVec2(0.0f, boxHeight), true);

		int index = 0;
		for (const auto& [stateEnum, statePtr] : states_) {
			(void)statePtr;

			ImGui::PushID(index++); // ID衝突対策

			// ボタンラベル
			const std::string stateName = StateToString(stateEnum);

			// ステート切り替えボタン
			if (ImGui::Button(stateName.c_str(), ImVec2(-1.0f, 0.0f))) {
				ChangeState(stateEnum);
			}

			ImGui::PopID();
		}

		ImGui::EndChild();
	}

	ImGui::SeparatorText("DebugFlag");
	if (ImGui::Button("SwitchPartsDebug")) {
		SwitchShowPartsTransform();
	}


	ImGui::End();
}

void BossMech::SwitchShowPartsTransform() {
	debugFlag_.showPartsTransform = !debugFlag_.showPartsTransform;
}
