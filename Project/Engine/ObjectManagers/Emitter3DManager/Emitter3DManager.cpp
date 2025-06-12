#include "Emitter3DManager.h"

#include <cassert>

#include "Logger/Logger.h"

Emitter3DManager::Emitter3DManager() {
	Logger::Log("Emitter3DManager Initialize\n");
}

Emitter3DManager::~Emitter3DManager() {
	Logger::Log("Emitter3DManager Finalize\n");
}

void Emitter3DManager::Update() {
	for (auto& emitter : emitters3D_) {
		if (emitter) {
			emitter->Update();
		}
	}
}

std::string Emitter3DManager::CreateEmitter(const std::string& emitterName, const Vector3& position) {
	// 新しいレンダラー名を決定
	std::string uniqueName = emitterName;

	// 追加するエミッター
	std::unique_ptr<Emitter3D> newEmitter3D = std::make_unique<Emitter3D>(emitterName, position);

	// コンテナに追加
	emitters3D_.push_back(std::move(newEmitter3D));

	return uniqueName;
}

void Emitter3DManager::Remove(const std::string& emitterName) {
	emitterName;
}

Emitter3D* Emitter3DManager::Find(const std::string& emitterName) {
	// ベクターを走査して、名前が一致する描画オブジェクトを探す
	for (auto& emitter : emitters3D_) {
		if (emitter && emitter->GetName() == emitterName) {
			return emitter.get();  // ポインタを返す
		}
	}

	// 見つからない場合
	assert(false && "Not Found Emitter3D");
	return nullptr;
}

void Emitter3DManager::Clear() {
	emitters3D_.clear();
}

void Emitter3DManager::DeleteGarbage() {
}
