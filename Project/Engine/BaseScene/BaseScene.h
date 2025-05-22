#pragma once

#include <memory>
#include <string>

// 前方宣言
template <typename Data>
class SceneManager;

template <typename Data>
class BaseScene {
public:
	BaseScene(std::shared_ptr<Data> data, SceneManager<Data>* sceneManager)
		: data_(data), sceneManager_(sceneManager) {
	}

	virtual ~BaseScene() = default;

	virtual void Initialize() {}
	virtual void Update() {}
	virtual void Draw() {}
	virtual void Finalize() {}

protected:
	// シーン変更関数
	void ChangeScene(const std::string& sceneName);
	// データ取得関数
	Data& GetData() const;

protected:
	// 共有データ
	std::shared_ptr<Data> data_;
	// シーンマネージャのポインタ
	SceneManager<Data>* const sceneManager_;
};

template<typename Data>
inline void BaseScene<Data>::ChangeScene(const std::string& sceneName) {
	sceneManager_->ChangeScene(sceneName);
}

template<typename Data>
inline Data& BaseScene<Data>::GetData()const {
	return *data_;
}
