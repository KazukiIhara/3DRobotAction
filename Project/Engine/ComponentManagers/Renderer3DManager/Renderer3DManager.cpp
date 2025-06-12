#include "Renderer3DManager.h"

#include <cassert>

#include "Logger/Logger.h"

Renderer3DManager::Renderer3DManager() {
	Clear();
	Logger::Log("Renderer3DManager Initialize\n");
}

Renderer3DManager::~Renderer3DManager() {
	Clear();
	Logger::Log("Renderer3DManager Finalize\n");
}

std::weak_ptr<ModelRenderer> Renderer3DManager::Add(std::shared_ptr<ModelRenderer> modelRenderer) {
	assert(modelRenderer && "ModelRenderer must not be null");
	modelRenderers_.push_back(std::move(modelRenderer));
	return modelRenderers_.back();
}

void Renderer3DManager::Draw() {
	for (auto& modelRenderer : modelRenderers_) {
		modelRenderer->Draw();
	}
}

void Renderer3DManager::DeleteGarbage() {
	for (auto& modelRenderer : modelRenderers_) {
		if (modelRenderer && !modelRenderer->GetIsAlive()) {
			modelRenderer->Finalize();
		}
	}
	std::erase_if(modelRenderers_, [](const std::shared_ptr<ModelRenderer>& modelRenderer) {
		return modelRenderer && !modelRenderer->GetIsAlive();
		});

}

void Renderer3DManager::Clear() {
	modelRenderers_.clear();
}