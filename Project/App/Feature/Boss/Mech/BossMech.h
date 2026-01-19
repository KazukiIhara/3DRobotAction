#pragma once

// C++
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>

// 機体基底クラス
#include "Feature/Mech/Base/BaseMech.h"

// 武器クラス
#include "Feature/Boss/Mech/Weapon/LaserGun/BossMechWeaponLaserGun.h"

// ステート基底クラス
#include "Feature/Boss/Mech/State/BossMechBaseState.h"

// アニメーションクラス
#include "MechAnimation/Animator/MechAnimator.h"

// コライダークラス
#include "Feature/Mech/Collider/MechCollider.h"

// 前方宣言
class DamageObjectManager;
class GameEffectManager;
class MechAnimationContainer;
class MechCore;

/// <summary>
/// ボス機体クラス
/// </summary>
class BossMech:public BaseMech {
public:
	// ステート
	enum class BossMechState {
		Idle,
		LaserShot
	};
public:
	BossMech(const InitParam& param, const RefContext& ref, MechCore* playerMech);
	~BossMech() = default;

	void Update([[maybe_unused]] bool isShowDebugUI, const BossMech::InitParam& param);
	void Draw();

	void ChangeState(BossMech::BossMechState nextState);

	Transform3D* GetTransform();

	// 関節トランスフォーム取得
	Transform3D* GetPartsTransform(MechAnimation::TransType type);

	// アニメーション再生クラスへのアクセッサ
	MechAnimator* GetAnimator();

	// プレイヤー機体へのアクセッサ
	MechCore* GetPlayerMech();

	// 参照ポインタへのアクセッサ
	DamageObjectManager* GetDamageObjectManager();
	GameEffectManager* GetGameEffectManager();

	// デバッグ用描画
	void DebugDraw();

	// 初期化パラメータを受け取る
	void SetInitParam(const BossMech::InitParam& initParam);

private:
	// 対応するステートを取得
	BossMechBaseState* GetState(BossMech::BossMechState state);
	// ステートを文字列に変換
	const std::string StateToString(BossMech::BossMechState state);



	// パーツタイプを文字列に変換
	const std::string TransTypeToString(MechAnimation::TransType partsType);

	// デバッグウィンドウ描画
	void ShowDebugWidow();

	// デバッグフラグ切り替え
	void SwitchShowPartsTransform();
	void SwitchEditPartsTransform();
	void SwitchStopUpdate();

private:
	// ステートテーブル
	std::unordered_map<BossMech::BossMechState, std::unique_ptr<BossMechBaseState>> states_;
	// 現在のステート
	std::pair<BossMech::BossMechState, BossMechBaseState*> currentState_;

	MechCore* playerMech_ = nullptr;

};