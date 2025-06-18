#pragma once

#include <memory>

class MechCore;

class MechCoreBaseState {
public:
	virtual ~MechCoreBaseState() = default;
	virtual void Enter(MechCore* mechCore) {};
	virtual void Update(MechCore* mechCore) {};
	virtual void Exit(MechCore* mechCore) {};
};