#include "TransformManager.h"

#include <cassert>

#include "Logger/Logger.h"

TransformManager::TransformManager() {
	Clear();
	Logger::Log("TransformManager Initialized\n");
}

TransformManager::~TransformManager() {
	Clear();
	Logger::Log("TransformManager Finalized\n");
}

Transform3D* TransformManager::Add(std::unique_ptr<Transform3D> transform) {
	assert(transform && "Transform must not be null");
	transforms_.push_back(std::move(transform));
	return transforms_.back().get();
}

void TransformManager::Update() {
	for (auto& transform : transforms_) {
		if (!transform->GetParent()) {
			transform->Update();
		}
	}
}

void TransformManager::DeleteGarbage() {
	// 生存フラグが消えているトランスフォームの後始末関数を呼ぶ
	for (auto& transform : transforms_) {
		if (transform && !transform->GetisAlive()) {
			transform->Finalize();
		}
	}
	// 生存フラグが消えているトランスフォームを削除
	std::erase_if(transforms_, [](const std::unique_ptr<Transform3D>& transform) {
		return transform && !transform->GetisAlive();
		});
}

void TransformManager::Clear() {
	transforms_.clear();
}