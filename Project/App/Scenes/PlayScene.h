#pragma once

#include <memory>
#include <array>

#include "Framework/MAGI.h"
#include "MAGIUitility/MAGIUtility.h"

using namespace MAGIUtility;


//-------------------------------------------
// シーンオブジェクト
//-------------------------------------------
#include "GameObject/Player/Player.h"


/// <summary>
/// ゲームプレイシーン
/// </summary>
/// <typeparam name="Data"></typeparam>
template <typename Data>
class PlayScene :public BaseScene<Data> {
public:
	using BaseScene<Data>::BaseScene; // 親クラスのコンストラクタをそのまま継承
	~PlayScene()override = default;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;

private:
	// カメラ
	std::unique_ptr<Camera3D> sceneCamera_ = nullptr;
	std::unique_ptr<Camera2D> sceneCamera2D_ = nullptr;

	// DirectionalLight
	DirectionalLight directionalLight_{};


	//----------------------------------------- 
	// シーンオブジェクト
	//-----------------------------------------
	std::unique_ptr<Player> player = nullptr;

};

template<typename Data>
inline void PlayScene<Data>::Initialize() {
	//-------------------------------------------------------
	// シーンの必須設定(はよ基底クラスに移せや)
	//-------------------------------------------------------

	// シーンカメラ作成
	sceneCamera_ = std::make_unique<Camera3D>("SceneCamera");
	// マネージャに追加
	MAGISYSTEM::AddCamera3D(std::move(sceneCamera_));
	// カメラを設定
	MAGISYSTEM::SetCurrentCamera3D("SceneCamera");

	// 2Dカメラ作成
	sceneCamera2D_ = std::make_unique<Camera2D>("SpriteCamera");
	// マネージャに追加
	MAGISYSTEM::AddCamera2D(std::move(sceneCamera2D_));
	// カメラを設定
	MAGISYSTEM::SetCurrentCamera2D("SpriteCamera");

	// プレイヤー作成
	player = std::make_unique<Player>();

}

template<typename Data>
inline void PlayScene<Data>::Update() {
	// プレイヤー更新
	player->Update();
}

template<typename Data>
inline void PlayScene<Data>::Draw() {
	// プレイヤー描画
	player->Draw();
}

template<typename Data>
inline void PlayScene<Data>::Finalize() {


}
