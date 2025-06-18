#pragma once

// C++
#include <unordered_map>

// MyHedder
#include "Math/Utility/MathUtility.h"

#include "MechCoreStates/MechCoreBaseState.h"

// 部位ごとのクラス
#include "GameObject/Mech/MechLeg/MechLeg.h"


// 前方宣言
class GameObject3D;

// 入力コマンド
struct InputCommand {
	Vector2 direction = { 0.0f,0.0f };
	bool jump = false;
	bool quickBoost = false;
	bool assultBoost = false;
};

// 状態
enum class MechCoreState {
	Root,
	QuickBoost,
	AsslutBoost
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

	const MechCoreState& GetCurrentState()const;
	const Vector3& GetVelocity()const;

	void SetVelocity(const Vector3& velocity);

private:
	std::weak_ptr<MechCoreBaseState> GetState(MechCoreState state);
private:
	// オブジェクト
	std::weak_ptr<GameObject3D> core_;
	// ステートテーブル
	std::unordered_map<MechCoreState, std::shared_ptr<MechCoreBaseState>> states_;
	// 現在のステート
	std::pair<MechCoreState, std::weak_ptr<MechCoreBaseState>> currentState_;
	


private:
	// パラメータ
	Vector3 velocity_ = { 0.0f,0.0f,0.0f };
	bool onGround_ = false;
};