#include "DevelopSceneh.h"

using namespace Magi;

void DevelopScene::Initialize() {
	//-------------------------------------------------------
	// シーンの必須設定(はよ基底クラスに移せや)
	//-------------------------------------------------------

	// 2Dカメラ作成
	std::unique_ptr<Camera2D> sceneCamera2D = std::make_unique<Camera2D>("SpriteCamera");
	// マネージャに追加
	MAGISYSTEM::AddCamera2D(std::move(sceneCamera2D));
	// カメラを設定
	MAGISYSTEM::SetCurrentCamera2D("SpriteCamera");
	// ライトを設定
	directionalLight_.direction = Normalize(Vector3(1.0f, -1.0f, 0.5f));


	//===========================
	// マネージャの初期化
	//===========================

	// ゲームエフェクトマネージャ
	gameEffectManager_ = std::make_unique<GameEffectManager>();

	// 攻撃コリジョンマネージャ
	attackCollisionManager_ = std::make_unique<AttackCollisionManager>();

	// 弾マネージャ
	attackObjectManger_ = std::make_unique<AttackObjectManager>(attackCollisionManager_.get());

	// プレイヤー作成
	player_ = std::make_unique<Player>(attackObjectManger_.get(), gameEffectManager_.get());

	// ボス作成
	boss_ = std::make_unique<Boss>(attackObjectManger_.get(), gameEffectManager_.get(), player_->GetMechCore());


}

void DevelopScene::Update() {
	// 平行光源をセット
	MAGISYSTEM::SetDirectionalLight(directionalLight_);

	// プレイヤーを更新
	player_->Update();
	// ボスを更新
	boss_->Update();

	// 攻撃オブジェクトマネージャ更新
	attackObjectManger_->Update();

	// 攻撃判定更新
	attackCollisionManager_->Update();

	// エフェクトマネージャ更新
	gameEffectManager_->Update();

}

void DevelopScene::Draw() {
	// プレイヤーを描画
	player_->Draw();
	// ボスを描画
	boss_->Draw();

	// 弾マネージャ描画
	attackObjectManger_->Draw();
	// 攻撃判定マネージャ描画
	attackCollisionManager_->Draw();
	// エフェクトマネージャ描画
	gameEffectManager_->Draw();
}

void DevelopScene::Finalize() {
	MAGISYSTEM::DeleteAll();
}
