#pragma once

#include "Logger/Logger.h"
#include "SceneManager.h"

using namespace Magi;

SceneManager::SceneManager()
	: data_(std::make_unique<GameData>()) {
	Logger::Log("SceneManager Initialize\n");
}

SceneManager::~SceneManager() {
	if (currentScene_) {
		currentScene_->Finalize();
		currentScene_.reset();
	}
	Logger::Log("SceneManager Finalize\n");
}

void SceneManager::Update() {
	// フェード更新
	fade_.Update();

	// シーン切り替え
	SwitchScene();
	if (currentScene_) {
		currentScene_->Update();
	}
}

void SceneManager::Draw() {
	if (currentScene_) {
		currentScene_->Draw();
	}

	// フェード描画
	fade_.Draw();
}

void SceneManager::StartScene(const std::string& sceneName) {
	// 最初は黒で固定
	fade_.SetBlack();

	// シーン生成
	auto it = factory_.find(sceneName);
	assert(it != factory_.end() && "No scene found with the given name");

	currentScene_ = it->second();
	currentScene_->Initialize();

	// 初回はフェードイン
	fade_.StartFadeIn(fadeInSec_);
}

void SceneManager::ChangeScene(const std::string& sceneName) {
	auto it = factory_.find(sceneName);
	assert(it != factory_.end() && "No scene found with the given name");
	
	// 次シーンが既に入っている場合
	if (nextScene_) {
		return;
	}

	// 次シーン生成
	nextScene_ = it->second();

	// 遷移予約
	isPendingSceneSwitch_ = true;
	needsFadeInAfterSwitch_ = true;

	// フェードアウト開始
	fade_.StartFadeOut(fadeOutSec_);

}

GameData& SceneManager::GetData() const {
	return *data_;
}

void SceneManager::SwitchScene() {
	// 遷移予約なし
	if (!isPendingSceneSwitch_) {
		return;
	}

	// 暗転待ち
	if (!fade_.IsFadeOutFinished()) {
		return;
	}

	// 現在シーン終了
	if (currentScene_) {
		currentScene_->Finalize();
		currentScene_.reset();
	}

	// 次シーン適用
	currentScene_ = std::move(nextScene_);
	if (currentScene_) {
		currentScene_->Initialize();
	}

	// 遷移完了
	isPendingSceneSwitch_ = false;

	// フェードイン開始
	if (needsFadeInAfterSwitch_) {
		needsFadeInAfterSwitch_ = false;
		fade_.StartFadeIn(fadeInSec_);
	}
}