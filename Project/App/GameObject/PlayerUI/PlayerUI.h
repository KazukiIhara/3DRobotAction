#pragma once

#include <memory>
#include "Structs/SpriteStruct.h"

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

	// ENゲージのスプライトデータ
	SpriteData enGaugeData_{};
	SpriteMaterialData enGaugeMaterialData_{};
	const float kENGaugeWidth_ = 696.0f;

	// ENバーのスプライトデータ
	SpriteData enBarData_{};
	SpriteMaterialData enBarMaterialData_{};
	const float kENBarWidth_ = 702.0f;
};