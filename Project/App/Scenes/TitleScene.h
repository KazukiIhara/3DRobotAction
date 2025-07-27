#pragma once

#include <memory>
#include <array>

#include "MAGI.h"

/// <summary>
/// タイトルシーン
/// </summary>
template <typename Data>
class TitleScene:public BaseScene<Data> {
public:
	using BaseScene<Data>::BaseScene; // 親クラスのコンストラクタをそのまま継承
	~TitleScene()override = default;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;

private:

};

template<typename Data>
inline void TitleScene<Data>::Initialize() {
	// 2Dカメラ作成
	std::unique_ptr<Camera2D> sceneCamera2D = std::make_unique<Camera2D>("SpriteCamera");
	// マネージャに追加
	MAGISYSTEM::AddCamera2D(std::move(sceneCamera2D));
	// カメラを設定
	MAGISYSTEM::SetCurrentCamera2D("SpriteCamera");

	// 3Dカメラ作成
	std::unique_ptr<Camera3D> sceneCamera3D = std::make_unique<Camera3D>("SceneCamera3D");
	// マネージャに追加
	MAGISYSTEM::AddCamera2D(std::move(sceneCamera3D));
	// カメラを設定
	MAGISYSTEM::SetCurrentCamera3D("SceneCamera3D");

}

template<typename Data>
inline void TitleScene<Data>::Update() {

}

template<typename Data>
inline void TitleScene<Data>::Draw() {

}

template<typename Data>
inline void TitleScene<Data>::Finalize() {

}
