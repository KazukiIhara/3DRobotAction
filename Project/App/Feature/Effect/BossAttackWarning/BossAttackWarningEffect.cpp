#define NOMINMAX

#include "BossAttackWarningEffect.h"

#include "Feature/Boss/Mech/BossMech.h"

#include "MAGI.h"

using namespace MAGIMath;
using namespace Magi;

BossAttackWarningEffect::BossAttackWarningEffect(const Vector3& emitPos, BossMech* mech) :
	BaseGameEffect(emitPos) {

	mech_ = mech;

	// アセットロード
	MAGISYSTEM::LoadTexture("star_08.png");
	MAGISYSTEM::LoadTexture("lensFlare.png");

	{
		// ワールド座標からスクリーン座標に変換
		const Vector2 screenPos = MAGIUtility::TransformWorldToScreen(worldPos_).second;
		// スプライトの設定
		for (size_t i = 0; i < 2; i++) {
			sprite_[i].position = screenPos;
		}
	}


	// マテリアルの設定
	mat_.anchorPoint = { 0.5f,0.5f };
	mat_.textureName = "lensFlare.png";
	mat_.color = Color::Red;
	mat_.blendmode = BlendMode::Add;

	// タイム取得
	timer_ = MAGISYSTEM::GetParameterValue<float>({ "EffectParam","BossAttackWarning","Time" });

	// サウンド再生
	MAGISYSTEM::PlayWaveSound("BossAttackWarning.wav");

}

void BossAttackWarningEffect::Update() {
	// タイマー更新
	const float dt = MAGISYSTEM::GetDeltaTime();
	const float time = MAGISYSTEM::GetParameterValue<float>({ "EffectParam","BossAttackWarning","Time" });
	timer_ -= dt;
	timer_ = std::max(0.0f, timer_);
	if (timer_ <= 0.0f) {
		isAlive_ = false;
	}
	float t = 1.0f - (timer_ / time);


	// パラメータからアニメーション作成

	// サイズ
	const Vector2 sizeStart = MAGISYSTEM::GetParameterValue<Vector2>({ "EffectParam","BossAttackWarning","SizeStart" });
	const Vector2 sizeEnd = MAGISYSTEM::GetParameterValue<Vector2>({ "EffectParam","BossAttackWarning","SizeEnd" });
	SimpleAnimation<Vector2> size = SimpleAnimation<Vector2>(sizeStart, sizeEnd, EasingType::EaseInOutCubic, true,
		SimpleAnimation<Vector2>::LoopType::PingPong);

	// 回転
	std::array<const float, 2> rotateStart = {
		MAGISYSTEM::GetParameterValue<float>({ "EffectParam","BossAttackWarning","RotateStart" }),
		MAGISYSTEM::GetParameterValue<float>({ "EffectParam","BossAttackWarning","RotateStart_1" })
	};
	std::array<const float, 2> rotateEnd = {
		MAGISYSTEM::GetParameterValue<float>({ "EffectParam","BossAttackWarning","RotateEnd" }),
		MAGISYSTEM::GetParameterValue<float>({ "EffectParam","BossAttackWarning","RotateEnd_1" })
	};

	std::array<SimpleAnimation<float>, 2> rotate = {
		SimpleAnimation<float>(rotateStart[0], rotateEnd[0]),
		SimpleAnimation<float>(rotateStart[1], rotateEnd[1])
	};

	// アニメーション適用

	// ワールド座標からスクリーン座標に変換
	Vector2 screenPos = { 0.0f,0.0f };
	if (mech_) {
		worldPos_ = mech_->GetPartsTransform(MechAnimation::TransType::Head)->GetWorldPosition();
		
		auto screenPosPair = MAGIUtility::TransformWorldToScreen(worldPos_);
		drawFlag_ = screenPosPair.first;
		screenPos = screenPosPair.second;
	}

	for (size_t i = 0; i < 2; i++) {
		// サイズ
		sprite_[i].size = size.GetValue(t * 2.0f);
		// 座標
		sprite_[i].position = screenPos;
		// 回転
		sprite_[i].rotate = rotate[i].GetValue(t);
	}

}

void BossAttackWarningEffect::Draw() {
	if (drawFlag_) {
		for (size_t i = 0; i < 2; i++) {
			MAGISYSTEM::DrawSprite(sprite_[i], mat_);
		}
	}
}