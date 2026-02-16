#pragma once

#include "Feature/GameUI/System/Base/BaseGameUI.h"
#include "Structs/SpriteStruct.h"

class ILockOnTarget;

/// <summary>
/// ロックオンUI
/// </summary>
class LockOnUI: public BaseGameUI {
public:
	LockOnUI(ILockOnTarget* target);
	~LockOnUI()override = default;

	void Update()override;
	void Draw()override;

private:
	float innerRot_ = 0.0f;

	ILockOnTarget* target_;

	SpriteData inner_;
	SpriteMaterialData innerMat_;

	SpriteData outer_;
	SpriteMaterialData outerMat_;
	

	SpriteData bossStatus_;
	SpriteMaterialData bossStatusMat_;
};