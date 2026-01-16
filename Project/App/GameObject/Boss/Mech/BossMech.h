#pragma once

// C++
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>

// 機体パーツ
#include "../Mech/Parts/Head/BossMechHead.h"
#include "../Mech/Parts/Body/BossMechBody.h"
#include "../Mech/Parts/Arm/Right/BossMechRightArm.h"
#include "../Mech/Parts/Arm/Left/BossMechLeftArm.h"
#include "../Mech/Parts/Leg/Right/BossMechRightLeg.h"
#include "../Mech/Parts/Leg/Left/BossMechLeftLeg.h"

// 武器クラス
#include "GameObject/Boss/Mech/Weapon/Base/BossMechBaseWeapon.h"

// ステート基底クラス
#include "GameObject/Boss/Mech/State/BossMechBaseState.h"

// 前方宣言
class AttackObjectManager;
class GameEffectManager;

/// <summary>
/// ボス機体クラス
/// </summary>
class BossMech {
public:
	// デバッグUIフラグ
	struct DebugFlag {
		bool showPartsTransform = false;
	};

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
		Idle,
		LaserShot
	};
public:
	BossMech(const BossMech::InitParam& initParam);
	~BossMech() = default;

	void Update();
	void Draw(bool isDebugDraw);

	void ChangeState(BossMech::BossMechState nextState);

	// 各パーツへのアクセッサ
	BossMechHead* GetHead();
	BossMechBody* GetBody();
	BossMechRightArm* GetRightArm();
	BossMechLeftArm* GetLeftArm();
	BossMechRightLeg* GetRightLeg();
	BossMechLeftLeg* GetLeftLeg();

	// 参照ポインタへのアクセッサ
	AttackObjectManager* GetAttackObjectManager();
	GameEffectManager* GetGameEffectManager();

	// デバッグ用描画
	void DebugDraw();

private:
	// 対応するステートを取得
	BossMechBaseState* GetState(BossMech::BossMechState state);
	// ステートを文字列に変換
	const std::string StateToString(BossMech::BossMechState state);

	// デバッグウィンドウ描画
	void ShowDebugWidow();

	// デバッグフラグ切り替え
	void SwitchShowPartsTransform();

private:
	// トランスフォーム
	Transform3D* transform_;

	// 各パーツ
	std::unique_ptr<BossMechHead> head_;
	std::unique_ptr<BossMechBody> body_;
	std::unique_ptr<BossMechRightArm> rightArm_;
	std::unique_ptr<BossMechLeftArm> leftArm_;
	std::unique_ptr<BossMechRightLeg> rightLeg_;
	std::unique_ptr<BossMechLeftLeg> leftLeg_;

	// 更新用のパーツリスト
	std::vector<IBossMechParts*> parts_;

	// 更新用の武器リスト
	std::vector<BossMechBaseWeapon*> weapons_;

	// ステートテーブル
	std::unordered_map<BossMech::BossMechState, std::unique_ptr<BossMechBaseState>> states_;
	// 現在のステート
	std::pair<BossMech::BossMechState, BossMechBaseState*> currentState_;

	// デバッグフラグ構造体
	DebugFlag debugFlag_{};


	// 参照ポインタ
	AttackObjectManager* attackObjectManager_ = nullptr;
	GameEffectManager* gameEffectManager_ = nullptr;

};