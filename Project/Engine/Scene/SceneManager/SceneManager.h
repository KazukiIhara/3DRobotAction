#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <functional>
#include <cassert>
#include <type_traits>

#include "BaseScene/BaseScene.h"
#include "GameData/GameData.h"

class SceneManager {
public:
	SceneManager();
	~SceneManager();

	// 更新
	void Update();
	// 描画
	void Draw();

	// 新しいシーンの追加
	template <class T>
	void AddScene(const std::string& name) {
		// BaseScene を継承しているかをチェック
		static_assert(std::is_base_of<BaseScene, T>::value,
			"SceneTemplate<Data> must derive from BaseScene<Data>.");

		// ファクトリ関数を登録
		factory_[name] = [this]() {
			return std::make_unique<T>(this);
			};
	}

	// シーン変更
	void ChangeScene(const std::string& sceneName);

	// 共有データへの参照を取得
	GameData& GetData()const;
private:
	// シーン変更処理
	void SwitchScene();
private:
	// シーンファクトリ関数
	using SceneFactoryFunc = std::function<std::unique_ptr<BaseScene>()>;
	// シーンファクトリ
	std::unordered_map<std::string, SceneFactoryFunc> factory_;
	// 現在シーン
	std::unique_ptr<BaseScene> currentScene_;
	// 次のシーン
	std::unique_ptr<BaseScene> nextScene_;
	// 共有データ
	std::unique_ptr<GameData> data_;

};