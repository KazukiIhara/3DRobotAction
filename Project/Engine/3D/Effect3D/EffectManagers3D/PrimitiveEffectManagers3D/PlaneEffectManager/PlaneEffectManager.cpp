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
	// トランスフォームを作成
	std::unique_ptr<Transform3D> transform = std::make_unique<Transform3D>(
		param.scale.animation.GetStartValue(),
		param.rotate.animation.GetStartValue(),
		param.translate.animation.GetStartValue());

	// トランスフォームをマネージャに入れる
	Transform3D* trans = transformManager_->Add(std::move(transform));

	// リストに追加するエフェクト
	PlaneEffect	newEffect{
		.param = param,
		.currentTrans = trans,
		.currentMaterial = MaterialData3D{
			.textureName = param.textureName,
			.baseColor = param.color.animation.GetStartValue(),
			.uvTranslate = param.uvTranslate.animation.GetStartValue(),
			.uvScale = param.uvScale.animation.GetStartValue(),
			.uvRotate = param.uvRotate.animation.GetStartValue(),
			.blendMode = param.blendMode
			},
		.currentShape = param.shape.animation.GetStartValue(),
		.currentTime = 0.0f,
	};

	// コンテナに追加
	effects_.push_back(newEffect);
}

void PlaneEffectManager::Update() {
	MAGIAssert::Assert(currentIndex_ <= kMaxEffectNum, "PlaneEffect Over MaxSize !");

	for (size_t i = 0; i < effects_.size();) {

		// 参照
		auto& e = effects_[i];

		// 経過時間を超えたら
		if (e.currentTime <= e.param.totalTime) {
			// スケールを求める
			const Vector3 scale = e.param.scale.GetCurrentValue(e.currentTime);
			// 回転を求める
			const Vector3 rotate = e.param.rotate.GetCurrentValue(e.currentTime);
			// 移動量を求める
			const Vector3 translate = e.param.translate.GetCurrentValue(e.currentTime);

			// 値をセット
			e.currentTrans->SetScale(scale);
			e.currentTrans->SetRotate(rotate);
			e.currentTrans->SetTranslate(translate);

			// デルタタイムを加算
			e.currentTime += deltaTimer_->GetDeltaTime();

			// インクリメント
			++i;
			continue;
		}

		// トランスフォームの生存フラグを切る
		e.currentTrans->SetIsAlive(false);

		// 最後尾と入れ替えて削除
		e = std::move(effects_.back());
		effects_.pop_back();

		// デルタタイムを加算
		e.currentTime += deltaTimer_->GetDeltaTime();
	}
}

void PlaneEffectManager::Draw() {
	for (size_t i = 0; i < effects_.size(); i++) {
		planeDrawer_->AddPlane(effects_[i].currentTrans->GetWorldMatrix(), effects_[i].currentShape, effects_[i].currentMaterial);
	}
}