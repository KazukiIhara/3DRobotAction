#pragma once

#include <string>

#include "MechAnimation/MechAnimationConatainer/MechAnimationContainer.h"

class BossMech;

class MechAnimationEdit {
public:
	explicit MechAnimationEdit(BossMech* mech);
	~MechAnimationEdit() = default;

	void Update();

	void SetBossMech(BossMech* mech);
	void SetAnimationContainer(MechAnimationContainer* container);

	bool AddAnimationClip(const std::string& name, const MechAnimationContainer::Clip& clip, bool overwrite = false);

private:
	void ShowWindow();
	void DrawRotate(BossMech* mech, int typeValue, const char* label);

private:
	BossMech* mech_ = nullptr;
	MechAnimationContainer* container_ = nullptr;
};
