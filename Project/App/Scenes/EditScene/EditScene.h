#pragma once

#include <memory>
#include <array>

#include "MAGI.h"

/// <summary>
/// エディターシーン
/// </summary>
class EditScene:public BaseScene {
public:
	using BaseScene::BaseScene; // 親クラスのコンストラクタをそのまま継承
	~EditScene()override = default;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;

private:

};