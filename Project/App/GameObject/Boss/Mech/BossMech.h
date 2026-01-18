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

// 前方宣言
class DamageObjectManager;
class GameEffectManager;
class MechCore;

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
		BossMechBaseArm::InitParam armR;
		BossMechBaseArm::InitParam armL;
		BossMechLeg::InitParam leg;
	};
	// デバッグUIフラグ
	struct DebugFlag {
		bool showPartsTransform = false;
		bool editPartsTransform = false;
	};
	// パーツ列挙型
	enum class TransType {
		Head,           // 頭
		Body,           // 胴体

		UpperArmLeft,   // 上腕L
		LowerArmLeft,   // 下腕L
		HandLeft,       // 手L

		UpperArmRight,  // 上腕R
		LowerArmRight,  // 下腕R
		HandRight,      // 手R

		Waist,          // 腰
		UpperLegLeft,   // 上足L
		LowerLegLeft,   // 下足L
		FootLeft,       // 足L

		UpperLegRight,  // 上足R
		LowerLegRight,  // 下足R
		FootRight       // 足R
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

	// 武器へのアクセッサ
	BossMechBaseWeapon* GetWeapon(const std::string& name);

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
	const std::string TransTypeToString(BossMech::TransType partsType);

	// デバッグウィンドウ描画
	void ShowDebugWidow();

	// デバッグフラグ切り替え
	void SwitchShowPartsTransform();
	void SwitchEditPartsTransform();

private:
	// トランスフォーム
	Transform3D* transform_;

	// パーツ
	std::unique_ptr<BossMechHead> head_;
	std::unique_ptr<BossMechBody> body_;
	std::unique_ptr<BossMechRightArm> armR_;
	std::unique_ptr<BossMechLeftArm> armL_;
	std::unique_ptr<BossMechLeg> leg_;

	// パーツリスト
	std::vector<IBossMechParts*> parts_;

	// 武器マップ
	std::unordered_map<std::string, std::unique_ptr<BossMechBaseWeapon>> weapons_;

	// ステートテーブル
	std::unordered_map<BossMech::BossMechState, std::unique_ptr<BossMechBaseState>> states_;
	// 現在のステート
	std::pair<BossMech::BossMechState, BossMechBaseState*> currentState_;

	// デバッグフラグ構造体
	DebugFlag debugFlag_{};

	// 参照ポインタ
	DamageObjectManager* damageObjectManager_ = nullptr;
	GameEffectManager* gameEffectManager_ = nullptr;
	MechCore* playerMech_ = nullptr;

};