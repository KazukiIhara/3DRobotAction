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
	int suffix = 1;

	// 同じ名前が既に存在する場合、一意な名前を生成
	auto isNameUsed = [&](const std::string& testName) {
		return std::any_of(emitters3D_.begin(), emitters3D_.end(), [&](const auto& emitter) {
			return emitter->name == testName;
			});
		};

	while (isNameUsed(uniqueName)) {
		uniqueName = emitterName + "_" + std::to_string(suffix);
		suffix++;
	}

	// 追加するエミッター
	std::unique_ptr<Emitter3D> newEmitter3D = std::make_unique<Emitter3D>(uniqueName, position);

	// コンテナに追加
	emitters3D_.push_back(std::move(newEmitter3D));

	return uniqueName;
}

void Emitter3DManager::Remove(const std::string& emitterName) {
	// ベクターを走査して、名前が一致するエミッターsを探す
	for (auto it = emitters3D_.begin(); it != emitters3D_.end(); ++it) {
		// (*it)->name_ で描画オブジェクト名名を取得
		if ((*it)->name == emitterName) {
			emitters3D_.erase(it);
			return;  // 見つかったら削除して関数を抜ける
		}
	}
	// 見つからなかった場合
	assert(false && "Not Found Emitter3D to Remove");
}

Emitter3D* Emitter3DManager::Find(const std::string& emitterName) {
	// ベクターを走査して、名前が一致する描画オブジェクトを探す
	for (auto& emitter : emitters3D_) {
		if (emitter && emitter->name == emitterName) {
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
	emitters3D_.erase(
		std::remove_if(
			emitters3D_.begin(),
			emitters3D_.end(),
			[](const std::unique_ptr<Emitter3D>& emitter) {
				return !emitter->isAlive;
			}
		),
		emitters3D_.end()
	);
}
