#include "Renderer3DManager.h"

#include "Logger/Logger.h"

#include <cassert>

Renderer3DManager::Renderer3DManager() {
	Clear();
	Logger::Log("Renderer3DManager Initialize\n");
}

Renderer3DManager::~Renderer3DManager() {
	Logger::Log("Renderer3DManager Finalize\n");
}

void Renderer3DManager::Update() {
	for (auto& renderer : renderers_) {
		if (renderer) {
			renderer->Update();
		}
	}
}

void Renderer3DManager::Draw() {
	for (auto& renderer : renderers_) {
		if (renderer && renderer->GetIsShow()) {
			renderer->Draw();
		}
	}
}

std::unique_ptr<PrimitiveRenderer3D> Renderer3DManager::CreatePrimitiveRenderer(const std::string& name, Primitive3DType primitiveRenderer, const std::string& textureName) {
	// 作成する描画オブジェクト
	std::unique_ptr<PrimitiveRenderer3D> newRenderer3D = std::make_unique<PrimitiveRenderer3D>(name, primitiveRenderer, textureName);
	// 形状を設定
	newRenderer3D->AssignShape();
	// 形状タイプを設定
	newRenderer3D->SetRenderer3DType(Renderer3DType::Primitive);

	// 作成したRendererの名前を返す
	return std::move(newRenderer3D);
}

std::unique_ptr<StaticRenderer3D> Renderer3DManager::CreateStaticRenderer(const std::string& name, const std::string& modelName) {
	// 追加する描画オブジェクト
	std::unique_ptr<StaticRenderer3D> newRenderer3D = std::make_unique<StaticRenderer3D>(name, modelName);
	// 形状を設定
	newRenderer3D->AssignShape();
	// 形状タイプを設定
	newRenderer3D->SetRenderer3DType(Renderer3DType::Static);

	return std::move(newRenderer3D);
}

std::unique_ptr<SkinningRenderer3D> Renderer3DManager::CreateSkinningRenderer(const std::string& name, const std::string& modelName) {
	// 追加する描画オブジェクト
	std::unique_ptr<SkinningRenderer3D> newRenderer3D = std::make_unique<SkinningRenderer3D>(name, modelName);
	// 形状を設定
	newRenderer3D->AssignShape();
	// 形状タイプを設定
	newRenderer3D->SetRenderer3DType(Renderer3DType::Skinning);

	return std::move(newRenderer3D);
}

void Renderer3DManager::AddRenderer(std::unique_ptr<BaseRenderable3D> newRenderer) {
	// コンテナに登録
	renderers_.push_back(std::move(newRenderer));
}

BaseRenderable3D* Renderer3DManager::Find(const std::string& name) {
	// ベクターを走査して、名前が一致する描画オブジェクトを探す
	for (auto& renderer : renderers_) {
		if (renderer && renderer->name == name) {
			return renderer.get();  // ポインタを返す
		}
	}

	// 見つからない場合
	assert(false && "Not Found Renderer3D");
	return nullptr;
}

void Renderer3DManager::Clear() {
	renderers_.clear();
}

void Renderer3DManager::DeleteGarbages() {
	renderers_.erase(
		std::remove_if(
			renderers_.begin(),
			renderers_.end(),
			[](const std::unique_ptr<BaseRenderable3D>& renderer3D) {
				return !renderer3D->isAlive;
			}
		),
		renderers_.end()
	);
}

const std::vector<std::unique_ptr<BaseRenderable3D>>& Renderer3DManager::GetRenderers() const {
	return renderers_;
}
