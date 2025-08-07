#pragma once

#include "Logger/Logger.h"
#include "SceneManager.h"

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
	SwitchScene();
	if (currentScene_) {
		currentScene_->Update();
	}
}

void SceneManager::Draw() {
	if (currentScene_) {
		currentScene_->Draw();
	}
}

void SceneManager::ChangeScene(const std::string& sceneName) {
	auto it = factory_.find(sceneName);
	assert(it != factory_.end() && "No scene found with the given name");

	nextScene_ = it->second();
}

GameData& SceneManager::GetData() const {
	return *data_;
}

void SceneManager::SwitchScene() {
	if (nextScene_) {
		if (currentScene_) {
			currentScene_->Finalize();
			currentScene_.reset();
		}
		currentScene_ = std::move(nextScene_);
		currentScene_->Initialize();
	}
}