#pragma once

// フレームワーク
#include "MAGI.h"


namespace magi {
/// <summary>
/// 機体カスタマイズシーン
/// </summary>
class MechCustomScene :public BaseScene {
public:
	using BaseScene::BaseScene; // 親クラスのコンストラクタをそのまま継承
	~MechCustomScene() = default;

	void Initialize()override;
	void Update()override;
	void Draw()override;
	void Finalize()override;

private:

};
}
