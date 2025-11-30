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
	PlayerUI();
	~PlayerUI() = default;

	void SetBoss(std::weak_ptr<MechCore> bossMechCore);

	void Update(MechCore* mechCore);

	void Draw(MechCore* mechCore);

private:
	void UpdateLockonUI(MechCore* mechCore);
	void UpdateAPUI(MechCore* mechCore);
	void UpdateENUI(MechCore* mechCore);

	void DrawLockonUI();
	void DrawAPUI();
	void DrawENUI();

	void DrawDebugUI(MechCore* mechCore);
private:
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

private:
	// 描画用パラメータ

	// 描画用の現在HP
	float currentUIHp_ = 0.0f;
	// 描画用ターゲットHP
	float targetUIHp_ = 0.0f;
};