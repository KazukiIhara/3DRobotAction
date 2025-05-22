#include "StaticParticleGroup3D.h"

#include "Framework/MAGI.h"

StaticParticleGroup3D::StaticParticleGroup3D(const std::string& particleGroupName, const std::string& modelName)
	:BaseParticleGroup3D(particleGroupName) {
	// 描画タイプを設定
	rendererType_ = Renderer3DType::Static;
	// 名前からモデルデータを取得
	modelData_ = MAGISYSTEM::FindModel(modelName);

}

void StaticParticleGroup3D::AssignShape() {
	model_ = std::make_unique<Model>(modelData_);
	model_->Initialize();
	assert(model_ && "Warning: Not found model");
}

void StaticParticleGroup3D::Update() {
	// モデル更新
	model_->Update();
	// 基底クラスの更新
	BaseParticleGroup3D::Update();
}

void StaticParticleGroup3D::Draw() {
	// パーティクルグループの描画前設定
	PrepareForRendering();

	// 描画
	model_->DrawInstancedForParticle(instanceCount_);
}
