#include "GameEffectManager.h"

GameEffectManager::GameEffectManager() {
	effects_.clear();
}

void GameEffectManager::Add(std::unique_ptr<BaseGameEffect> effect) {
	effects_.push_back(std::move(effect));
}

void GameEffectManager::Update() {
	for (auto& effect : effects_) {
		if (effect->IsAlive()) {
			effect->Update();
		}
	}
	// 死んでいるエフェクトを削除
	DeleteGarbage();
}

void GameEffectManager::Draw() {
	for (auto& effect : effects_) {
		if (effect->IsAlive()) {
			effect->Draw();
		}
	}
}

void GameEffectManager::DeleteGarbage() {
	effects_.erase(
		std::remove_if(effects_.begin(), effects_.end(),
			[](const std::unique_ptr<BaseGameEffect>& e) {return !e->IsAlive(); }),
		effects_.end()
	);
}