#pragma once

// C++
#include <unordered_map>

// MyHedder
#include "MechCoreStates/MechCoreBaseState.h"

// 前方宣言
class GameObject3D;

// 状態
enum class MechCoreState {
	Root,
	
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

private:
	std::weak_ptr<MechCoreBaseState> GetState(MechCoreState state);
private:
	// オブジェクト
	std::weak_ptr<GameObject3D> core_;
	// ステートテーブル
	std::unordered_map<MechCoreState, std::shared_ptr<MechCoreBaseState>> states_;
	// 現在のステート
	std::weak_ptr<MechCoreBaseState> currentState_;
};