#include "PlaneEffectManager.h"

#include <cassert>

// MyHedder
#include "MAGIAssert/MAGIAssert.h"
#include "Core/DeltaTimer/DeltaTimer.h"
#include "ComponentManagers/TransformManager/TransformManager.h"
#include "3D/Drawer3D/PrimitiveDrawers/PlaneDrawer3D/PlaneDrawer3D.h"

PlaneEffectManager::PlaneEffectManager(DeltaTimer* deltaTimer, TransformManager* transformManager, PlaneDrawer3D* planeDrawer) {
	assert(deltaTimer);
	assert(transformManager);
	assert(planeDrawer);

	deltaTimer_ = deltaTimer;
	transformManager_ = transformManager;
	planeDrawer_ = planeDrawer;

	currentIndex_ = 0;
	Logger::Log("PlaneEffectManager Initialize\n");
}

PlaneEffectManager::~PlaneEffectManager() {
	Logger::Log("PlaneEffectManager Finalize\n");
}

void PlaneEffectManager::Add(const PlaneEffectParam& param) {
	// 追加する場所の参照
	PlaneEffect& writeEffect = effects_[currentIndex_];

	// トランスフォームを作成
	std::unique_ptr<Transform3D> transform = std::make_unique<Transform3D>(
		param.scale.animation.GetStartValue(),
		param.rotate.animation.GetStartValue(),
		param.translate.animation.GetStartValue());

	// トランスフォームをマネージャに入れる
	Transform3D* trans = transformManager_->Add(std::move(transform));

	// リストに追加するエフェクト
	writeEffect = PlaneEffect{
		.param = param,
		.currentTrans = trans,
		.currentMaterial = MaterialData3D{
			.textureName = param.textureName,
			.baseColor = param.color.animation.GetStartValue(),
			.uvTranslate = param.uvTranslate.animation.GetStartValue(),
			.uvScale = param.uvScale.animation.GetStartValue(),
			.uvRotate = param.uvRotate.animation.GetStartValue(),
			.blendMode = param.blendMode,
		}
	};

	// 現在インデックスをインクリメント
	currentIndex_++;
}

void PlaneEffectManager::Update() {
	MAGIAssert::Assert(currentIndex_ <= kMaxEffectNum, "PlaneEffect Over MaxSize !");
	currentIndex_ = 0;
	for (auto& effect : effects_) {

		// 経過時間を超えたら
		if (effect.currentTime > effect.param.totalTime) {
			effect.isActive = false;
			continue;
		}

		// スケールを求める
		const Vector3 scale = effect.param.scale.GetCurrentValue(effect.currentTime);
		// 回転を求める
		const Vector3 rotate = effect.param.rotate.GetCurrentValue(effect.currentTime);
		// 移動量を求める
		const Vector3 translate = effect.param.translate.GetCurrentValue(effect.currentTime);

		// 値をセット
		effect.currentTrans->SetScale(scale);
		effect.currentTrans->SetRotate(rotate);
		effect.currentTrans->SetTranslate(translate);

		// デルタタイムを加算
		effect.currentTime += deltaTimer_->GetDeltaTime();
	}

}

void PlaneEffectManager::Draw() {
	for (auto& effect : effects_) {
		if (effect.isActive) {
			planeDrawer_->AddPlane(effect.currentTrans->GetWorldMatrix(), effect.currentShape, effect.currentMaterial);
		}
	}
}

void PlaneEffectManager::DeleteGarbage() {
	for (auto& effect : effects_) {
		if (!effect.isActive) {

		}
	}
}