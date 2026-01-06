#pragma once

#include <memory>
#include "Structs/SpriteStruct.h"

#include "Math/Types/AllMathTypes.h"

class MechCore;

/// <summary>
/// プレイヤーUIクラス
/// </summary>
class PlayerUI {
public:
	PlayerUI(std::weak_ptr<MechCore> playerMechCore, std::weak_ptr<MechCore> bossMechCore);
	~PlayerUI() = default;

	void Update();

	void Draw();

private:
	void UpdateLockonUI(MechCore* mechCore);
	void UpdateAPUI(MechCore* mechCore);
	void UpdateENUI(MechCore* mechCore);

	void DrawLockonUI();
	void DrawAPUI();
	void DrawENUI();

	void DrawWeaponUI();

	void DrawDebugUI(MechCore* mechCore);
private:
	// プレイヤーのMechCoreのポインタ
	std::weak_ptr<MechCore> playerMech_;

	// ボスがいる際のMechCoreのポインタ
	std::weak_ptr<MechCore> bossMech_;

	// LockonUIのスプライトデータ
	SpriteData lockonGrayData_{};
	SpriteMaterialData lockonGrayMat_{};

	// 射撃盤UIのスプライト設定
	SpriteData lockonRedData_{};
	SpriteMaterialData lockonRedMat_{};
	Vector3 lockOnWPos_{};
	Vector3 lerpStartPos_{};

	// Apゲージのスプライトデータ
	SpriteData apGaugeData_{};
	SpriteMaterialData apGaugeMaterialData_{};
	const float kAPSpriteWidth_ = 464.0f;

	// Apバーのスプライトデータ
	SpriteData apBarData_{};
	SpriteMaterialData apBarMaterialData_{};

	// APUIのスプライトデータ
	SpriteData apData_{};
	SpriteMaterialData apMaterialData_{};

	// ボスのAPゲージのスプライトデータ
	SpriteData bossApGaugeData_{};
	SpriteMaterialData bossApGaugeMaterialData_{};
	const float kBossApGaugeWidth_ = 912.0f;

	// ボスのAPバーのスプライトデータ
	SpriteData bossApBarData_{};
	SpriteMaterialData bossApBarMaterialData_{};
	const float kBossAPBarWidth_ = 916.0f;

	// エネルギーバーのスプライトデータ
	SpriteData energyBarData_{};
	SpriteMaterialData energyBarMatData_{};
	Vector2 energyBarPos_{ 140.0f,1090.0f };

	// エネルギーゲージのスプライトデータ
	SpriteData energyGaugeData_{};
	SpriteMaterialData energyGaugeMatData_{};
	Vector2 energyGaugePos_{ 101.0f,978.0f };
	const float kEnergyGaugeHeight_ = 662.0f;

	// エネルギーテキストのスプライトデータ
	SpriteData energyTextData_{};
	SpriteMaterialData energyTextMatData_{};
	Vector2 energyTextPos_{ 75.0f,280.0f };


	// 右手武器情報のスプライトデータ

	// 武器表示
	SpriteData rightHandWeaponData_{};
	SpriteMaterialData rightHandWeaponMat_{};
	Vector2 rightHandWeaponPos_{};

	// 武器種
	SpriteData rightHandWeaponTextData_{};
	SpriteMaterialData rightHandWeaponTextMat_{};
	Vector2 rightHandWeaponTextPos_{};

	// 残弾
	SpriteData rightHandWeaponNumData_[2]{};
	SpriteMaterialData rightHandWeaponNumMat_{};
	Vector2 rightHandWeaponNumPos_[2]{};


	// 左手武器情報のスプライトデータ

	// 武器表示
	SpriteData leftHandWeaponData_{};
	SpriteMaterialData leftHandWeaponMat_{};
	Vector2 leftHandWeaponPos_{};

	// 武器種
	SpriteData leftHandWeaponTextData_{};
	SpriteMaterialData leftHandWeaponTextMat_{};
	Vector2 leftHandWeaponTextPos_{};

	// 残弾
	SpriteData leftHandWeaponNum_[2]{};
	SpriteMaterialData leftHandWeaponNumMat_{};
	Vector2 leftHandWeaponNumPos_[2]{};


private:
	// 描画用パラメータ

	// 描画用の現在HP
	float currentUIHp_ = 0.0f;
	// 描画用ターゲットHP
	float targetUIHp_ = 0.0f;
};