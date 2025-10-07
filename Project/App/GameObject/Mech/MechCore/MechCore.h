#pragma once

// C++
#include <memory>
#include <unordered_map>

// MyHedder
#include "Math/Utility/MathUtility.h"

// アプリ用汎用ヘッダ
#include "GameCommon/GameCommon.h"

// 部位ごとのクラス
#include "GameObject/Mech/MechParts/MechHead/MechHead.h"
#include "GameObject/Mech/MechParts/MechBody/MechBody.h"
#include "GameObject/Mech/MechParts/MechArmRight/MechArmRight.h"
#include "GameObject/Mech/MechParts/MechArmLeft/MechArmLeft.h"
#include "GameObject/Mech/MechParts/MechLeg/MechLeg.h"

// 武器クラス
#include "GameObject/Mech/MechWeapons/MechWeaponAssultRifle/MechWeaponAssultRifle.h"
#include "GameObject/Mech/MechWeapons/MechShoulderWeaponDualMissileLauncher/MechShoulderWeaponDualMissileLauncher.h"

// コンポーネントクラス
#include "GameObject/Mech/MechComponents/Movement/MechMovementComponent.h"
#include "GameObject/Mech/MechComponents/LockOn/MechLockOnComponent.h"
#include "GameObject/Mech/MechComponents/Attack/MechAttackComponent.h"
#include "GameObject/Mech/MechComponents/Status/MechStatusComponent.h"

// コライダー
#include "GameObject/Mech/MechCollider/MechCollider.h"

// エフェクトクラス
#include "GameEffects/QuickBoost/QuickBoostParticle/QuickBoostParticle.h"
#include "GameEffects/BulletHit/BulletHitEffect/BulletHitEffect.h"

// 前方宣言
class GameObject3D;
class MechCoreBaseState;
class AttackObjectManager;

/// <summary>
/// 機体制御クラス
/// </summary>
class MechCore {
public:
	MechCore(const Vector3& position, FriendlyTag tag, AttackObjectManager* bulletManager, bool enableHardlockOn);
	~MechCore() = default;

	void Update();
	void ChangeState(MechCoreState nextState);

	// 機体そのものではなく、エフェクトなど付随するものの描画処理
	void Draw();

	//======================= 
	// ゲッター
	//======================= 

	// ゲームオブジェクトの取得
	std::weak_ptr<GameObject3D> GetGameObject()const;

	//
	// MechCoreが直接持っているパラメータの取得
	// 
	const MechCoreState& GetCurrentState()const;
	const InputCommand& GetInputCommand()const;
	const LockOnView& GetLockOnView()const;
	const FriendlyTag& GetFriendlyTag()const;
	bool GetIsInvaildFCS()const;

	// 
	// 各パーツの取得
	// 

	MechBody* GetMechBody();
	MechArmLeft* GetMechArmLeft();
	MechArmRight* GetMechArmRight();

	// 
	// 武器の取得
	// 
	BaseMechHandWeapon* GetLeftHandWeapon();
	BaseMechHandWeapon* GetRightHandWeapon();
	BaseMechShoulderWeapon* GetLeftShoulderWeapon();
	BaseMechShoulderWeapon* GetRightShoulderWeapon();

	// 
	// コンポーネントの取得
	// 
	MechMovementComponent* GetMovementComponent();
	MechLockOnComponent* GetLockOnComponent();
	MechAttackComponent* GetAttackComponent();
	MechStatusComponent* GetStatusComponent();

	// 
	// エフェクトクラスの取得
	// 
	QuickBoostParticle* GetQuickBoostParticle();
	BulletHitEffect* GetBulletHitEffect();


	// 
	// コライダーの取得(今後コンポーネント化する可能性あり)
	// 
	MechCollider* GetCollider();

	//======================= 
	// セッター
	//======================= 

	//
	// MechCoreが直接持つパラメータのセット
	// 
	void SetInputCommand(const InputCommand& command);
	void SetLockOnView(const LockOnView& lockOnView);

private:
	// 対応するステートを取得
	std::weak_ptr<MechCoreBaseState> GetState(MechCoreState state);

	// コライダーの更新
	void UpdateCollider();

	// コライダーの描画
	void DrawCollider();

private:
	// インプットコマンド
	InputCommand inputCommand_;
	// ロックオン用のカメラ情報
	LockOnView lockOnView_;
	// 友好タグ
	FriendlyTag tag_;
	// FCS無効状態
	bool isInvalidFCS_ = false;

	// オブジェクト
	std::weak_ptr<GameObject3D> core_;
	// ステートテーブル
	std::unordered_map<MechCoreState, std::shared_ptr<MechCoreBaseState>> states_;
	// 現在のステート
	std::pair<MechCoreState, std::weak_ptr<MechCoreBaseState>> currentState_;

	//======================= 
	// コンポーネント
	//=======================

	// 移動コンポーネント
	std::unique_ptr<MechMovementComponent> movementComponent_;
	// ロックオンコンポーネント
	std::unique_ptr<MechLockOnComponent> lockOnComponent_;
	// 攻撃コンポーネント
	std::unique_ptr<MechAttackComponent> attackComponent_;
	// ステータス値管理コンポーネント
	std::unique_ptr<MechStatusComponent> statusComponent_;

	//======================= 
	// 各パーツ
	//=======================

	// 頭
	std::unique_ptr<MechHead> head_ = nullptr;

	// 体
	std::unique_ptr<MechBody> body_ = nullptr;

	// 腕
	std::unique_ptr<MechArmRight> rightArm_ = nullptr;
	std::unique_ptr<MechArmLeft> leftArm_ = nullptr;

	// 足
	std::unique_ptr<MechLeg> leg_ = nullptr;

	// 左手武器
	std::unique_ptr<BaseMechHandWeapon> leftHandWeapon_ = nullptr;

	// 右手武器
	std::unique_ptr<BaseMechHandWeapon> rightHandWeapon_ = nullptr;

	// 左肩武器
	std::unique_ptr<BaseMechShoulderWeapon> leftShoulerWeapon_ = nullptr;

	// 右肩武器
	std::unique_ptr<BaseMechShoulderWeapon> rightShoulerWeapon_ = nullptr;

	//=======================
	// エフェクト
	//=======================

	// クイックブーストのエフェクト
	std::unique_ptr<QuickBoostParticle> quickBoostparticle_ = nullptr;
	// 弾衝突時のエフェクト
	std::unique_ptr<BulletHitEffect> bulletHitEffect_ = nullptr;
	

	//=======================
	// コライダー
	//=======================

	// ひとまず体
	std::unique_ptr<MechCollider> collider_;

	const Vector3 kColliderMin_ = { -0.5f,-1.0f,-0.5f };
	const Vector3 kColliderMax_ = { 0.5f,0.8f, 0.5f };

};