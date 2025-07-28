#pragma once

#include "Logger/Logger.h"
#include "SceneManager.h"


template <typename Data>
SceneManager<Data>::SceneManager()
	: data_(std::make_shared<Data>()) {
	Logger::Log("SceneManager Initialize\n");
}

template <typename Data>
SceneManager<Data>::~SceneManager() {
	if (currentScene_) {
		currentScene_->Finalize();
		currentScene_.reset();
	}
	Logger::Log("SceneManager Finalize\n");
}

template <typename Data>
void SceneManager<Data>::Update() {
	SwitchScene();
	if (currentScene_) {
		currentScene_->Update();
	}
}

template <typename Data>
void SceneManager<Data>::Draw() {
	if (currentScene_) {
		currentScene_->Draw();
	}
}

template <typename Data>
template <template <class> class SceneTemplate>
void SceneManager<Data>::AddScene(const std::string& sceneName) {
	// SceneTemplate<Data> 型をエイリアス
	using SceneType = SceneTemplate<Data>;

	// BaseScene<Data> を継承しているかをチェック
	static_assert(std::is_base_of<BaseScene<Data>, SceneType>::value,
		"SceneTemplate<Data> must derive from BaseScene<Data>.");

	// ファクトリ関数を登録
	factory_[sceneName] = [this]() {
		return std::make_unique<SceneType>(this->data_, this);
		};
}

template <typename Data>
void SceneManager<Data>::ChangeScene(const std::string& sceneName) {
	auto it = factory_.find(sceneName);
	assert(it != factory_.end() && "No scene found with the given name");

	nextScene_ = it->second();
}

template<typename Data>
inline Data& SceneManager<Data>::GetData() const {
	return *data_;
}

template <typename Data>
void SceneManager<Data>::SwitchScene() {
	if (nextScene_) {
		if (currentScene_) {
			currentScene_->Finalize();
			currentScene_.reset();
		}
		currentScene_ = std::move(nextScene_);
		currentScene_->Initialize();
	}
}