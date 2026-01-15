#pragma once

// C++
#include <string>
#include <memory>
#include <unordered_map>

// 機体パーツ
#include "../Mech/Parts/Head/BossMechHead.h"
#include "../Mech/Parts/Body/BossMechBody.h"
#include "../Mech/Parts/Arm/Right/BossMechRightArm.h"
#include "../Mech/Parts/Arm/Left/BossMechLeftArm.h"
#include "../Mech/Parts/Leg/Right/BossMechRightLeg.h"
#include "../Mech/Parts/Leg/Left/BossMechLeftLeg.h"

// 前方宣言
class BossMechBaseState;
class AttackObjectManager;
class GameEffectManager;

/// <summary>
/// ボス機体クラス
/// </summary>
class BossMech {
public:
	// 初期化パラメータ
	struct InitParam {
		// 初期座標
		Vector3 position{};
		// 各パーツのデータ
		BossMechHead::InitParam head;
		BossMechBody::InitParam body;
		BossMechBaseArm::InitParam arm;
		BossMechBaseLeg::InitParam leg;
	};
	// ステート
	enum class BossMechState {
		Idle
	};
public:
	BossMech(const BossMech::InitParam& initParam);
	~BossMech() = default;

	void Update();
	void Draw();

	void ChangeState(BossMech::BossMechState nextState);

private:
	BossMechBaseState* GetState(BossMechState state);

private:
	// 各パーツ
	std::unique_ptr<BossMechHead> head_;
	std::unique_ptr<BossMechBody> body_;
	std::unique_ptr<BossMechRightArm> rightArm_;
	std::unique_ptr<BossMechLeftArm> leftArm_;
	std::unique_ptr<BossMechRightLeg> rightLeg_;
	std::unique_ptr<BossMechLeftLeg> leftLeg_;


	// ステートテーブル
	std::unordered_map<BossMechState, std::unique_ptr<BossMechBaseState>> states_;
	// 現在のステート
	std::pair<BossMechState, BossMechBaseState*> currentState_;


	// 参照ポインタ
	AttackObjectManager* attackObjectManager_ = nullptr;
	GameEffectManager* gameEffectManager_ = nullptr;

};