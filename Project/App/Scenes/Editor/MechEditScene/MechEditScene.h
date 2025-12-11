#pragma once

// C++
#include <memory>
#include <array>
#include <string>

// 
// MyHedders
// 

// フレームワーク
#include "MAGI.h"

// 手持ち武器
#include "GameObject/MechWeapon/Hand/MechHandWeapon.h"

// データIOクラス
#include "GameDataIO/HandWeaponDataIO/HandWeaponDataIO.h"


namespace magi {
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
	void UpdateGUI();

	void HandWeaponEditUpdate();

	void HandWeaponTypeUI(MechHandWeapon::Param& param);

private:
	// 手持ち武器
	std::unique_ptr<MechHandWeapon> handWeapon_;
	// ロード用バッファ
	std::string handWeaponLoadNameBuf_ = "";
	// モデル名変更用バッファ
	std::string handWeaponModelNameBuf_ = "";

private:
	// データIOクラス

	// 手持ち武器
	std::unique_ptr<HandWeaponDataIO> handWeaponDataIO_;

};
}
