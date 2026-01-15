#include "BossMech.h"

#include "MAGI.h"

#include "MAGIAssert/MAGIAssert.h" 

#include "GameObject/Boss/Mech/State/Idle/BossMechStateIdle.h"

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

	// 最初のステートを設定
	ChangeState(BossMechState::Idle);

}

void BossMech::Update() {
	// 全パーツを更新
	for (auto part : parts_) {
		part->Update();
	}

	// 全武器を更新

}

void BossMech::Draw(bool isDebugDraw) {
	// 全パーツを更新
	for (auto part : parts_) {
		part->Draw();
		if (isDebugDraw) {
			part->DebugDraw();
		}
	}

	// 全武器を描画

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

BossMechBaseState* BossMech::GetState(BossMech::BossMechState state) {
	auto it = states_.find(state);
	if (it != states_.end()) {
		return it->second.get();
	}

	MAGIAssert::Assert(false, "Not find BossMechState!");
	return {};
}
