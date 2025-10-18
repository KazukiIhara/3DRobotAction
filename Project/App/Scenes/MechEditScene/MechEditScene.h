#pragma once

// C++
#include <memory>
#include <array>

// MyHedder
#include "MAGI.h"

// 手持ち武器
#include "GameObject/MechWeapon/Hand/MechHandWeapon.h"

/// <summary>
/// 機体エディットシーン
/// </summary>
class MechEditScene:public BaseScene {
public:
	using BaseScene::BaseScene; // 親クラスのコンストラクタをそのまま継承
	~MechEditScene()override = default;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;

private:
	// 手持ち武器
	std::unique_ptr<MechHandWeapon> handWeapon_;

};