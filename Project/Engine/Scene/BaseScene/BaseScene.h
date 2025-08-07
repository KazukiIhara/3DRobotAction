#pragma once

#include <memory>
#include <string>

// MyHedder
#include "GameData/GameData.h"

// 前方宣言
class SceneManager;

class BaseScene {
public:
	BaseScene(SceneManager* sceneManager);

	virtual ~BaseScene() = default;

	virtual void Initialize() {}
	virtual void Update() {}
	virtual void Draw() {}
	virtual void Finalize() {}

protected:
	// シーン変更関数
	void ChangeScene(const std::string& sceneName);

	// データ取得関数
	GameData& GetData() const;

protected:
	// シーンマネージャのポインタ
	SceneManager* sceneManager_;
};