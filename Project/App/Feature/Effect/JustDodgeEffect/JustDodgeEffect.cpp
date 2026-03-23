#define NOMINMAX

#include "JustDodgeEffect.h"

#include "MAGI.h"

#include "Random/Random.h"

using namespace Magi;
using namespace MAGIMath;

JustDodgeEffect::JustDodgeEffect(const Vector3& emitPos) :
	BaseGameEffect(emitPos) {

	// エディットシーン用に一応ロード処理を書く
	MAGISYSTEM::LoadTexture("gradation.png");
	MAGISYSTEM::LoadTexture("dodgeEffect.png");
	MAGISYSTEM::LoadTexture("Circle2.png");
	MAGISYSTEM::LoadTexture("electric_0.png");
	MAGISYSTEM::LoadTexture("lensFlare.png");

	MAGISYSTEM::LoadTexture("gradationToon.png");

	// エフェクトのタイマーセット
	time_ = MAGISYSTEM::GetParameterValue<float>({ "EffectParam","JustDodge","EffectTime" });

	// 座標をセット
	ringTrans_ = emitPos;
	planeTrans_ = emitPos;

	// エフェクト初期設定

	// リング
	// 形状
	const Vector2 ringDataStart = MAGISYSTEM::GetParameterValue<Vector2>({ "EffectParam","JustDodge","RingDataStart(outer,inner)" });
	ringData_.outerRadius = ringDataStart.x;
	ringData_.innerRadius = ringDataStart.y;
	// マテリアル
	ringMat_.blendMode = BlendMode::Add;
	ringMat_.textureName = "dodgeEffect.png";
	ringMat_.baseColor = MAGISYSTEM::GetParameterValue<Vector4>({ "EffectParam","JustDodge","RingColorStart" });

	// 板ポリ

	// 形状
	planeScale_ = MAGISYSTEM::GetParameterValue<Vector3>({ "EffectParam","JustDodge","PlaneScaleStart" });

	// マテリアル
	planeMat_.blendMode = BlendMode::Add;
	planeMat_.textureName = "dodgeEffect.png";

	// パーティクル

	// 数
	const int32_t justDodgeParticleNum = MAGISYSTEM::GetParameterValue<int32_t>({ "EffectParam","JustDodge","ParticleNum" });

	// 設定
	const int32_t particleTex = MAGISYSTEM::GetTextureIndex("lensFlare.png");

	const Vector4 particleColor = MAGISYSTEM::GetParameterValue<Vector4>({ "EffectParam","JustDodge","ParticleColor" });
	const float particlePosRadius = MAGISYSTEM::GetParameterValue<float>({ "EffectParam","JustDodge","ParticlePosRadius" });
	const float particleLife = MAGISYSTEM::GetParameterValue<float>({ "EffectParam","JustDodge","ParticleLife" });
	const float particleSize = MAGISYSTEM::GetParameterValue<float>({ "EffectParam","JustDodge","ParticleSize" });
	const float particleSpeed = MAGISYSTEM::GetParameterValue<float>({ "EffectParam","JustDodge","ParticleSpeed" });

	// 発生
	for (int32_t i = 0; i < justDodgeParticleNum; i++) {

		const Vector3 pos = Normalize(Random::GenerateVector3(-100.0f, 100.0f)) * particlePosRadius;
		const Vector3 velo = Normalize(Random::GenerateVector3(-100.0f, 100.0f)) * particleSpeed;

		GPUParticleEmitData particleData;
		particleData.texIndex = particleTex;
		particleData.pos = worldPos_ + pos;
		particleData.velo = velo;
		particleData.size = Vector2(particleSize, particleSize);
		particleData.color = particleColor;
		particleData.life = particleLife;

		MAGISYSTEM::EmitParticle(particleData);
	}


	// サウンド再生
	MAGISYSTEM::PlayWaveSound("JustDodge.wav");
}

void JustDodgeEffect::Update() {
	// デルタタイムを取得
	const float kDt = MAGISYSTEM::GetDeltaTime();
	// タイマーを更新
	time_ -= kDt;
	time_ = std::max(0.0f, time_);

	// タイマーが0になったら生存フラグを切る
	if (time_ <= 0.0f) {
		isAlive_ = false;
		return;
	}

	// 定数取得
	const float kEffectTime = MAGISYSTEM::GetParameterValue<float>({ "EffectParam","JustDodge","EffectTime" });
	const float t = 1.0f - time_ / kEffectTime;

	// 形状データをあれこれする

	// リング
	// 形状
	const Vector2 ringDataStart = MAGISYSTEM::GetParameterValue<Vector2>({ "EffectParam","JustDodge","RingDataStart(outer,inner)" });
	const Vector2 ringDataEnd = MAGISYSTEM::GetParameterValue<Vector2>({ "EffectParam","JustDodge","RingDataEnd(outer,inner)" });
	const Vector2 ringData = SimpleAnimation<Vector2>(ringDataStart, ringDataEnd, EasingType::EaseOutCubic).GetValue(t);
	ringData_.outerRadius = ringData.x;
	ringData_.innerRadius = ringData.y;

	// マテリアル
	const Vector4 ringColorStart = MAGISYSTEM::GetParameterValue<Vector4>({ "EffectParam","JustDodge","RingColorStart" });
	const Vector4 ringColorEnd = MAGISYSTEM::GetParameterValue<Vector4>({ "EffectParam","JustDodge","RingColorEnd" });
	const Vector4 ringColor = SimpleAnimation<Vector4>(ringColorStart, ringColorEnd, EasingType::EaseInCubic).GetValue(t);
	ringMat_.baseColor = ringColor;

	// 板ポリ
	// 形状
	const Vector3 planeScaleStart = MAGISYSTEM::GetParameterValue<Vector3>({ "EffectParam","JustDodge","PlaneScaleStart" });
	const Vector3 planeScaleEnd = MAGISYSTEM::GetParameterValue<Vector3>({ "EffectParam","JustDodge","PlaneScaleEnd" });
	const Vector3 planeScale = SimpleAnimation<Vector3>(planeScaleStart, planeScaleEnd, EasingType::EaseInOutCubic, true, SimpleAnimation<Vector3>::LoopType::PingPong).GetValue(t * 2.0f);
	planeScale_ = planeScale;
}

void JustDodgeEffect::Draw() {

	// カメラからビルボード行列作成
	const Camera3D* currentCamera = MAGISYSTEM::GetCurrentCamera3D();
	const Matrix4x4 ringWMat = currentCamera->MakeBillboardMat(ringTrans_);
	const Matrix4x4 planeWMat = currentCamera->MakeBillboardMat(planeTrans_, 0.0f, planeScale_);

	// リング描画
	MAGISYSTEM::DrawRing3D(ringWMat, ringData_, ringMat_);
	// 板ポリ描画
	MAGISYSTEM::DrawPlane3D(planeWMat, planeData_, planeMat_);

}
