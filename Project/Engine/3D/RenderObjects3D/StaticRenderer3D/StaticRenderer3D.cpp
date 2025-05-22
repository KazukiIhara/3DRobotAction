#include "StaticRenderer3D.h"

#include <cassert>

#include "Framework/MAGI.h"

using namespace MAGIMath;

StaticRenderer3D::StaticRenderer3D(const std::string& objectName, const std::string& modelName)
	:BaseRenderable3D(objectName) {
	modelData_ = MAGISYSTEM::FindModel(modelName);
}

StaticRenderer3D::~StaticRenderer3D() {
	model_.reset();
}

void StaticRenderer3D::AssignShape() {
	// モデル取得
	model_ = std::make_unique<Model>(modelData_);
	model_->Initialize();
	assert(model_ && "Warning: Not found model");
}

void StaticRenderer3D::Update() {
	// モデル更新
	model_->Update();

	// 基底クラスの更新
	BaseRenderable3D::Update();
}

void StaticRenderer3D::Draw() {
	// 描画用の設定
	PrepareForRendering();
	// 3Dモデル描画
	model_->Draw();
}

void StaticRenderer3D::DrawInstanced(uint32_t instanceCount) {
	// 3Dモデル描画
	model_->DrawInstanced(instanceCount);
}

std::string StaticRenderer3D::GetModelName() const {
	return modelData_.name;
}
