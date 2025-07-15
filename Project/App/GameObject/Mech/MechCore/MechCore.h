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

// コンポーネントクラス
#include "GameObject/Mech/MechComponents/Movement/MechMovementComponent.h"
#include "GameObject/Mech/MechComponents/LockOn/MechLockOnComponent.h"
#include "GameObject/Mech/MechComponents/Attack/MechAttackComponent.h"

// 前方宣言
class GameObject3D;
class MechCoreBaseState;
class BulletManager;


/// <summary>
/// 機体の状態
/// </summary>
enum class MechCoreState {
	Idle,
	Move,
	QuickBoost,
	AssultBoost,
	Melee,
};

/// <summary>
/// 機体制御クラス
/// </summary>
class MechCore {
public:
	MechCore(FriendlyTag tag, BulletManager* bulletManager);
	~MechCore() = default;

	void Update();
	void ChangeState(MechCoreState nextState);

	//======================= 
	// ゲッター
	//======================= 

	std::weak_ptr<GameObject3D> GetGameObject()const;
	const MechCoreState& GetCurrentState()const;
	const InputCommand& GetInputCommand()const;
	const LockOnView& GetLockOnView()const;

	const FriendlyTag& GetFriendlyTag()const;

	MechBody* GetMechBody();
	MechArmLeft* GetMechArmLeft();
	MechArmRight* GetMechArmRight();

	BaseMechWeapon* GetLeftHandWeapon();
	BaseMechWeapon* GetRightHandWeapon();

	MechMovementComponent* GetMovementComponent();
	MechLockOnComponent* GetLockOnComponent();
	MechAttackComponent* GetAttackComponent();

	//======================= 
	// セッター
	//======================= 

	void SetInputCommand(const InputCommand& command);
	void SetLockOnView(const LockOnView& lockOnView);

private:
	// 対応するステートを取得
	std::weak_ptr<MechCoreBaseState> GetState(MechCoreState state);
private:
	// インプットコマンド
	InputCommand inputCommand_;

	// ロックオン用のカメラ情報
	LockOnView lockOnView_;

	// 友好タグ
	FriendlyTag tag_;

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
	std::unique_ptr<BaseMechWeapon> leftHandWeapon_ = nullptr;

	// 右手武器
	std::unique_ptr<BaseMechWeapon> rightHandWeapon_ = nullptr;

};