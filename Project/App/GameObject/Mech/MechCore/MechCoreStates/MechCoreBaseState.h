#pragma once

#include <memory>

class MechCore;

/// <summary>
/// 機体のステート基底クラス
/// </summary>
class MechCoreBaseState {
public:
	virtual ~MechCoreBaseState() = default;
	virtual void Enter(MechCore* mechCore) = 0;
	virtual void Update(MechCore* mechCore) = 0;
	virtual void Exit(MechCore* mechCore) = 0;
};