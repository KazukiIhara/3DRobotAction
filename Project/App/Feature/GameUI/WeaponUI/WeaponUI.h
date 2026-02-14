#pragma once

#include "Feature/GameUI/System/Base/BaseGameUI.h"
#include "Structs/SpriteStruct.h"

/// <summary>
/// 武器UIクラス
/// </summary>
class WeaponUI: public BaseGameUI {
public:
	WeaponUI();
	~WeaponUI()override = default;

	void Update()override;
	void Draw()override;

private:
	SpriteData data_;
	SpriteMaterialData mat_;

};