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
#include "../Mech/Parts/Leg/BossMechLeg.h"

// 武器クラス
#include "GameObject/Boss/Mech/Weapon/LaserGun/BossMechWeaponLaserGun.h"

// ステート基底クラス
#include "GameObject/Boss/Mech/State/BossMechBaseState.h"

// アニメーションクラス
#include "MechAnimation/Animator/MechAnimator.h"

// コライダークラス
#include "GameObject/Boss/Mech/Collider/BossMechCollider.h"

// 前方宣言
class DamageObjectManager;
class GameEffectManager;
class MechAnimationContainer;
class MechCore;

/// <summary>
/// ボス機体クラス
/// </summary>
class BossMech {
public:
	// 識別タグ
	enum class Tag {
		Player,
		Enemy
	};
	// 初期化パラメータ
	struct InitParam {
		// 初期座標
		Vector3 position{};
		// 各パーツのデータ
		BossMechHead::InitParam head;
		BossMechBody::InitParam body;
		BossMechBaseArm::InitParam armR;
		BossMechBaseArm::InitParam armL;
		BossMechLeg::InitParam leg;
	};
	// デバッグUIフラグ
	struct DebugFlag {
		bool isDebugDraw = false;
		bool editPartsTransform = false;
		bool stopUpdate = false;
	};
	// ステート
	enum class BossMechState {
		Idle,
		LaserShot
	};
public:
	BossMech(
		const BossMech::InitParam& initParam,
		DamageObjectManager* damageObjectManager,
		GameEffectManager* gameEffectManager,
		MechAnimationContainer* animationContainer,
		MechCore* playerMech
	);
	~BossMech() = default;

	void Update([[maybe_unused]] bool isShowDebugUI, const BossMech::InitParam& param);
	void Draw();

	void ChangeState(BossMech::BossMechState nextState);

	Transform3D* GetTransform();

	// パーツへのアクセッサ
	BossMechHead* GetHead();
	BossMechBody* GetBody();
	BossMechRightArm* GetRightArm();
	BossMechLeftArm* GetLeftArm();
	BossMechLeg* GetLeg();

	// 関節トランスフォーム取得
	Transform3D* GetPartsTransform(MechAnimation::TransType type);

	// 武器へのアクセッサ
	BossMechBaseWeapon* GetWeapon(const std::string& name);

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
	// トランスフォームの配列を作成
	void CreatePartsTransformArray();
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
	// トランスフォーム
	Transform3D* transform_;

	// タグ
	Tag tag_ = Tag::Enemy;

	// パーツ
	std::unique_ptr<BossMechHead> head_;
	std::unique_ptr<BossMechBody> body_;
	std::unique_ptr<BossMechRightArm> armR_;
	std::unique_ptr<BossMechLeftArm> armL_;
	std::unique_ptr<BossMechLeg> leg_;

	// パーツリスト
	std::vector<IBossMechParts*> parts_;

	// トランスフォームリスト
	std::array<Transform3D*, static_cast<size_t>(MechAnimation::TransType::Count)> partsTrans_{};

	// 武器マップ
	std::unordered_map<std::string, std::unique_ptr<BossMechBaseWeapon>> weapons_;

	// ステートテーブル
	std::unordered_map<BossMech::BossMechState, std::unique_ptr<BossMechBaseState>> states_;
	// 現在のステート
	std::pair<BossMech::BossMechState, BossMechBaseState*> currentState_;

	// アニメーションクラス
	std::unique_ptr<MechAnimator> animator_;

	// コライダー
	std::unique_ptr<BossMechCollider> collider_;

	// デバッグフラグ構造体
	DebugFlag debugFlag_{};

	// 参照ポインタ
	DamageObjectManager* damageObjectManager_ = nullptr;
	GameEffectManager* gameEffectManager_ = nullptr;
	MechAnimationContainer* mechAnimationContainer_ = nullptr;
	MechCore* playerMech_ = nullptr;

};