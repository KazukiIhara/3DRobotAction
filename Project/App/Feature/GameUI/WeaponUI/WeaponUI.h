#pragma once

#include "Feature/GameUI/System/Base/BaseGameUI.h"
#include "Structs/SpriteStruct.h"

class PilotMech;

/// <summary>
/// 武器UIクラス
/// </summary>
class WeaponUI: public BaseGameUI {
public:
	WeaponUI(PilotMech* pilotMech);
	~WeaponUI()override = default;

	void Update()override;
	void Draw()override;

private:
	PilotMech* pilotMech_;

	SpriteData cannonRifle_;
	SpriteMaterialData cannonRifleMat_;

	SpriteData machineGun_;
	SpriteMaterialData machineGunMat_;

	SpriteData lt_;
	SpriteMaterialData ltMat_;

	SpriteData rt_;
	SpriteMaterialData rtMat_;

	SpriteData rb_;
	SpriteMaterialData rbMat_;

	SpriteData lb_;
	SpriteMaterialData lbMat_;


};