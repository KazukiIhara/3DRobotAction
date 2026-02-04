#include "GameUIManager.h"

// C++
#include <algorithm>

GameUIManager::GameUIManager() {
	uis_.clear();
}

BaseGameUI* GameUIManager::Add(std::unique_ptr<BaseGameUI> ui) {
	uis_.push_back(std::move(ui));
	return uis_.back().get();
}

void GameUIManager::Update() {
	// 生存中UIのみ更新
	for (auto& ui : uis_) {
		if (ui && ui->IsAlive()) {
			ui->Update();
		}
	}

	// 死んでいるUIを削除
	DeleteGarbage();
}

void GameUIManager::Draw() {
	// 生存中UIのみ描画
	for (auto& ui : uis_) {
		if (ui && ui->IsAlive()) {
			ui->Draw();
		}
	}
}

void GameUIManager::DeleteGarbage() {
	uis_.erase(
		std::remove_if(uis_.begin(), uis_.end(),
			[](const std::unique_ptr<BaseGameUI>& ui) {
				// nullは削除
				if (!ui) {
					return true;
				}
				// 死んでいるUIは削除
				if (!ui->IsAlive()) {
					return true;
				}
				return false;
			}),
		uis_.end()
	);
}
