#pragma once

#include "Structs/SpriteStruct.h"

class MechCore;

/// <summary>
/// プレイヤーUIクラス
/// </summary>
class PlayerUI {
public:
	PlayerUI();
	~PlayerUI() = default;

	void Update(MechCore* mechCore);

	void Draw();

	void DrawLockonUI();

private:
	// LockonUIのスプライトデータ
	SpriteData lockonGrayData_{};
	SpriteMaterialData lockonGrayMat_{};

	SpriteData lockonRedData_{};
	SpriteMaterialData lockonRedMat_{};

};