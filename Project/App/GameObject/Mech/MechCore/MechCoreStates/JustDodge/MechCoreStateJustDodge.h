#pragma once


#include "GameObject/Mech/MechCore/MechCoreStates/MechCoreBaseState.h"

/// <summary>
/// ジャスト回避ステート
/// </summary>
class MechCoreStateJustDodge:public MechCoreBaseState {
public:
	~MechCoreStateJustDodge() = default;
	void Enter(MechCore* mechCore)override;
	void Update(MechCore* mechCore)override;
	void Exit(MechCore* mechCore)override;
};