#include "ParticleGroup3DManager.h"

#include <cassert>

#include "Logger/Logger.h"

ParticleGroup3DManager::ParticleGroup3DManager() {
	Logger::Log("ParticleGroup3DManager Initialize\n");
}

ParticleGroup3DManager::~ParticleGroup3DManager() {
	Logger::Log("ParticleGroup3DManager Finalize\n");
}

void ParticleGroup3DManager::Update() {
	for (auto& particleGroup3D : particleGroups3D_) {
		if (particleGroup3D) {
			particleGroup3D->Update();
		}
	}
}

void ParticleGroup3DManager::Draw() {
	for (auto& particleGroup3D : particleGroups3D_) {
		if (particleGroup3D) {
			particleGroup3D->Draw();
		}
	}
}

std::string ParticleGroup3DManager::CreatePrimitiveParticleGroup(const std::string& particleGroupName, const Primitive3DType& primitiveType, const std::string& textureName) {
	// 新しいパーティクルグループ名を決定
	std::string uniqueName = particleGroupName;
	int suffix = 1;

	// 同じ名前が既に存在する場合、一意な名前を生成
	auto isNameUsed = [&](const std::string& testName) {
		return std::any_of(particleGroups3D_.begin(), particleGroups3D_.end(), [&](const auto& particleGroup3D) {
			return particleGroup3D->name == testName;
			});
		};

	while (isNameUsed(uniqueName)) {
		uniqueName = particleGroupName + "_" + std::to_string(suffix);
		suffix++;
	}

	// 追加するパーティクルグループ
	std::unique_ptr<BaseParticleGroup3D> newParticleGroupManager = std::make_unique<PrimitiveParticleGroup3D>(uniqueName, primitiveType, textureName);
	newParticleGroupManager->AssignShape();

	// 追加
	particleGroups3D_.push_back(std::move(newParticleGroupManager));

	return uniqueName;
}

std::string ParticleGroup3DManager::CreateStaticParticleGroup(const std::string& particleGroupName, const std::string& modelName) {
	// 新しいパーティクルグループ名を決定
	std::string uniqueName = particleGroupName;
	int suffix = 1;

	// 同じ名前が既に存在する場合、一意な名前を生成
	auto isNameUsed = [&](const std::string& testName) {
		return std::any_of(particleGroups3D_.begin(), particleGroups3D_.end(), [&](const auto& particleGroup3D) {
			return particleGroup3D->name == testName;
			});
		};

	while (isNameUsed(uniqueName)) {
		uniqueName = particleGroupName + "_" + std::to_string(suffix);
		suffix++;
	}

	// 追加するパーティクルグループ
	std::unique_ptr<BaseParticleGroup3D> newParticleGroupManager = std::make_unique<StaticParticleGroup3D>(uniqueName, modelName);
	newParticleGroupManager->AssignShape();

	// 追加
	particleGroups3D_.push_back(std::move(newParticleGroupManager));

	return uniqueName;
}

void ParticleGroup3DManager::Remove(const std::string& name) {
	// ベクターを走査して、名前が一致するパーティクルグループを探す
	for (auto it = particleGroups3D_.begin(); it != particleGroups3D_.end(); ++it) {
		// (*it)->name_ で描画オブジェクト名名を取得
		if ((*it)->name == name) {
			particleGroups3D_.erase(it);
			return;  // 見つかったら削除して関数を抜ける
		}
	}
	// 見つからなかった場合
	assert(false && "Not Found ParticleGroup3D to Remove");
}

BaseParticleGroup3D* ParticleGroup3DManager::Find(const std::string& name) {
	// ベクターを走査して、名前が一致するパーティクルグループを探す
	for (auto& particleGroup3D : particleGroups3D_) {
		if (particleGroup3D && particleGroup3D->name == name) {
			return particleGroup3D.get();  // ポインタを返す
		}
	}

	// 見つからない場合
	assert(false && "Not Found ParticleGroup3D");
	return nullptr;
}

const std::vector<std::unique_ptr<BaseParticleGroup3D>>& ParticleGroup3DManager::GetParticleGroups() {
	return particleGroups3D_;
}
