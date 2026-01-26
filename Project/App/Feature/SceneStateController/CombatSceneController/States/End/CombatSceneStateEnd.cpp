#define NOMINMAX

#include "CombatSceneStateEnd.h"

//-------------------------------------------
// 操作クラス
//-------------------------------------------
#include "Feature/GameInputSystem/GameInputSystem.h"

//-------------------------------------------
// カメラ
//-------------------------------------------
#include "Feature/TPSCamera3D/TPSCamera3D.h"

//-------------------------------------------
// パイロット
//-------------------------------------------
#include "Feature/Pilot/Pilot.h"

//-------------------------------------------
// ボス
//-------------------------------------------
#include "Feature/Boss/Boss.h"

//-------------------------------------------
// マネージャ
//-------------------------------------------
#include "Feature/Effect/System/GameEffectManager/GameEffectManager.h"
#include "Feature/Damage/Object/Manager/DamageObjectManager.h"
#include "Feature/Damage/CollisionSystem/DamageCollisionSystem.h"

#include "Feature/SceneStateController/CombatSceneController/CombatSceneController.h"

#include "MAGI.h"

using namespace Magi;

void CombatSceneStateEnd::Enter([[maybe_unused]] CombatSceneControl::StateContextRef ref) {
	// ステートをフラッシュにする
	phase_ = Phase::Flash;
	// タイマーセット
	timer_ = MAGISYSTEM::GetParameterValue<float>({ "CombatSceneParam","End","Flash","Time" });


	// スプライト設定
	flashSprite_.size = { WindowApp::kClientWidth,WindowApp::kClientHeight };
	flashSpriteMat_.textureName = "white.png";
	flashSpriteMat_.color.w = MAGISYSTEM::GetParameterValue<float>({ "CombatSceneParam","End","Flash","StartAlpha" });
}

void CombatSceneStateEnd::Update([[maybe_unused]] CombatSceneControl::StateContextRef ref) {
	// フェーズごとに更新
	switch (phase_) {
		case CombatSceneStateEnd::Phase::Flash:
			UpdateFlash(ref);
			break;
		case CombatSceneStateEnd::Phase::Destroy:
			UpdateDestroy(ref);
			break;
		case CombatSceneStateEnd::Phase::Explosion:
			UpdateExplosion(ref);
			break;
		case CombatSceneStateEnd::Phase::Finish:
			UpdateFinish(ref);
			break;
	}
	// タイマー更新
	timer_ -= MAGISYSTEM::GetRawDeltaTime();
	timer_ = std::max(0.0f, timer_);
}

void CombatSceneStateEnd::Draw([[maybe_unused]] CombatSceneControl::StateContextRef ref) {

	switch (phase_) {
		case CombatSceneStateEnd::Phase::Flash:
			DrawFlash(ref);
			break;
		case CombatSceneStateEnd::Phase::Destroy:
			DrawDestroy(ref);
			break;
		case CombatSceneStateEnd::Phase::Explosion:
			DrawExplosion(ref);
			break;
		case CombatSceneStateEnd::Phase::Finish:
			DrawFinish(ref);
			break;
	}

}

void CombatSceneStateEnd::Exit([[maybe_unused]] CombatSceneControl::StateContextRef ref) {
	MAGISYSTEM::SetDeltaTimeMultiplier(1.0f);
}

void CombatSceneStateEnd::UpdateFlash(CombatSceneControl::StateContextRef ref) {
	// デルタタイム倍率をセット
	const float dtMul = MAGISYSTEM::GetParameterValue<float>({ "CombatSceneParam","End","Flash","DeltaTimeMul" });
	// デルタタイムの倍率をセット
	MAGISYSTEM::SetDeltaTimeMultiplier(dtMul);

	// フラッシュアニメーション処理
	const float stateTime = MAGISYSTEM::GetParameterValue<float>({ "CombatSceneParam","End","Flash","Time" });
	const float startAlpha = MAGISYSTEM::GetParameterValue<float>({ "CombatSceneParam","End","Flash","StartAlpha" });
	const float endAlpha = MAGISYSTEM::GetParameterValue<float>({ "CombatSceneParam","End","Flash","EndAlpha" });
	const int32_t flashCount = MAGISYSTEM::GetParameterValue<int32_t>({ "CombatSceneParam","End","Flash","Count" });

	auto flashAnimation = SimpleAnimation<float>(startAlpha, endAlpha,
		EasingType::Linear, true, SimpleAnimation<float>::LoopType::PingPong);
	const float flashT = 1.0f - timer_ / stateTime;
	const float alpha = flashAnimation.GetValue(flashT * static_cast<float>(flashCount));
	flashSpriteMat_.color.w = alpha;

	// 終了処理
	if (timer_ <= 0.0f) {
		phase_ = Phase::Destroy;

		// デストロイの初期化処理と初回更新　(時間がないとはいえなんてひどい設計なんだ...)
		timer_ = MAGISYSTEM::GetParameterValue<float>({ "CombatSceneParam","End","Destroy","Time" });
		UpdateDestroy(ref);
	}
}

void CombatSceneStateEnd::DrawFlash([[maybe_unused]] CombatSceneControl::StateContextRef ref) {
	// フラッシュっぽいテクスチャを加算合成で重ねる
	// αをアニメーションさせる
	MAGISYSTEM::DrawSprite(flashSprite_, flashSpriteMat_);
}

void CombatSceneStateEnd::UpdateDestroy(CombatSceneControl::StateContextRef ref) {



	// 終了処理
	if (timer_ <= 0.0f) {
		phase_ = Phase::Explosion;

		// Explosionの初期化処理と初回更新　(時間がないとはいえなんてひどい設計なんだ...)
		timer_ = MAGISYSTEM::GetParameterValue<float>({ "CombatSceneParam","End","Explosion","Time" });
		UpdateExplosion(ref);
	}
}

void CombatSceneStateEnd::DrawDestroy([[maybe_unused]] CombatSceneControl::StateContextRef ref) {

}

void CombatSceneStateEnd::UpdateExplosion(CombatSceneControl::StateContextRef ref) {



	// 終了処理
	if (timer_ <= 0.0f) {
		phase_ = Phase::Finish;

		// Finishの初期化処理と初回更新　(時間がないとはいえなんてひどい設計なんだ...)
		timer_ = MAGISYSTEM::GetParameterValue<float>({ "CombatSceneParam","End","Finish","Time" });
		UpdateFinish(ref);
	}
}

void CombatSceneStateEnd::DrawExplosion([[maybe_unused]] CombatSceneControl::StateContextRef ref) {

}

void CombatSceneStateEnd::UpdateFinish(CombatSceneControl::StateContextRef ref) {


	// 終了処理
	if (timer_ <= 0.0f) {

		// ステート終了処理
		ref.cbc->End();
	}
}

void CombatSceneStateEnd::DrawFinish([[maybe_unused]] CombatSceneControl::StateContextRef ref) {

}
