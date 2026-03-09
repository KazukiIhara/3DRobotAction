#pragma once

#include "Feature/GameUI/System/Base/BaseGameUI.h"
#include "Structs/SpriteStruct.h"

class PilotMech;

/// <summary>
/// パイロットスコアUI
/// </summary>
class PilotScoreUI : public BaseGameUI {
public:
	PilotScoreUI(PilotMech* pilotMech);
	~PilotScoreUI() = default;

	void Update()override;
	void Draw()override;

private:
	PilotMech* mech_;

	SpriteData text_;
	SpriteMaterialData textMat_;
	
	SpriteData bar_;
	SpriteMaterialData barMat_;

	SpriteData gauge_;
	SpriteMaterialData gaugeMat_;
};