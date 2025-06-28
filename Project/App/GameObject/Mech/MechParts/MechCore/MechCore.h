#pragma once

// C++
#include <memory>
#include <unordered_map>

// MyHedder
#include "Math/Utility/MathUtility.h"

// 部位ごとのクラス
#include "GameObject/Mech/MechParts/MechHead/MechHead.h"
#include "GameObject/Mech/MechParts/MechBody/MechBody.h"
#include "GameObject/Mech/MechParts/MechArms/MechArms.h"
#include "GameObject/Mech/MechParts/MechLeg/MechLeg.h"

// コンポーネントクラス
#include "GameObject/Mech/MechComponents/Movement/MechMovementComponent.h"

// 前方宣言
class GameObject3D;
class MechCoreBaseState;

// 入力コマンド
struct InputCommand {
	Vector2 moveDirection = { 0.0f,0.0f };
	bool jump = false;
	bool quickBoost = false;
	bool assultBoost = false;
};

// 状態
enum class MechCoreState {
	Idle,
	Move,
	QuickBoost,
	AssultBoost
};

/// <summary>
/// 機体制御クラス
/// </summary>
class MechCore {
public:
	MechCore();
	~MechCore() = default;

	void Update();
	void ChangeState(MechCoreState nextState);

	std::weak_ptr<GameObject3D> GetGameObject()const;
	const MechCoreState& GetCurrentState()const;
	const InputCommand& GetInputCommand()const;
	MechMovementComponent* GetMovementComponent();


	void SetInputCommand(const InputCommand& command);
private:
	// 対応するステートを取得
	std::weak_ptr<MechCoreBaseState> GetState(MechCoreState state);
private:
	// インプットコマンド
	InputCommand inputCommand_;

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


	// 攻撃コンポーネント


	//======================= 
	// 各パーツ
	//=======================

	// 頭


	// 体
	std::unique_ptr<MechBody> body_ = nullptr;

	// 腕


	// 足
	std::unique_ptr<MechLeg> leg_ = nullptr;

	// 武器系


};