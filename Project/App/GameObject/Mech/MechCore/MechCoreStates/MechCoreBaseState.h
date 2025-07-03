#pragma once

#include <memory>

class MechCore;

class MechCoreBaseState {
public:
	virtual ~MechCoreBaseState() = default;
	virtual void Enter(MechCore* mechCore) = 0;
	virtual void Update(MechCore* mechCore) = 0;
	virtual void Exit(MechCore* mechCore) = 0;
};