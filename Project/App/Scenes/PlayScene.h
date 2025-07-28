#pragma once

#include <memory>
#include <array>

#include "MAGI.h"
#include "MAGIUitility/MAGIUtility.h"

using namespace MAGIUtility;

//-------------------------------------------
// シーンオブジェクト
//-------------------------------------------
#include "GameObject/Player/Player.h"
#include "GameObject/Enemy/Enemy.h"

#include "GameObject/AttackCollisionManager/AttackCollisionManager.h"
#include "GameObject/BulletManager/BulletManager.h"

/// <summary>
/// 終了判定
/// </summary>
enum class FinishJudgment {
	Player,
	Enemy,
	Draw,
};

/// <summary>
/// プレイシーンの情報構造体
/// </summary>
struct PlaySceneInfo {
	int32_t battleTime = 90;
	FinishJudgment judge;
	bool finish = false;
	bool isPause = false;

};

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
	std::weak_ptr<Camera3D> mainCamera_;

	// DirectionalLight
	DirectionalLight directionalLight_{};

	//----------------------------------------- 
	// シーンオブジェクト
	//-----------------------------------------

	// プレイヤー
	std::unique_ptr<Player> player_;

	// 敵
	std::unique_ptr<Enemy> enemy_;

	//----------------------------------------- 
	// マネージャ
	//-----------------------------------------

	// 攻撃判定マネージャ
	std::unique_ptr<AttackCollisionManager> attackCollisionManager_;

	// 弾のマネージャ
	std::unique_ptr<BulletManager> bulletManger_;


	// ポストエフェクトの用の変数
	float vignetteScale_ = 18.0f;
	float vignetteFalloff_ = 0.8f;
	Vector2 radialBlurCenter_ = { 0.5f,0.5f };
	float radialBlurWidth_ = 0.01f;
	float gaussianSigma_ = 0.5f;

	// プレイシーンの情報
	PlaySceneInfo info;

	// 戦闘タイマー
	float tempBattleTime_ = 0.0;
	int32_t kMaxBattleTime_ = 90;

	// 
	// デバッグ用
	// 

	// 板ポリエフェクトのパラメータ
	PlaneEffectParam planeEffect_;

};

template<typename Data>
inline void PlayScene<Data>::Initialize() {
	//-------------------------------------------------------
	// シーンの必須設定(はよ基底クラスに移せや)
	//-------------------------------------------------------

	// 2Dカメラ作成
	std::unique_ptr<Camera2D> sceneCamera2D = std::make_unique<Camera2D>("SpriteCamera");
	// マネージャに追加
	MAGISYSTEM::AddCamera2D(std::move(sceneCamera2D));
	// カメラを設定
	MAGISYSTEM::SetCurrentCamera2D("SpriteCamera");

	directionalLight_.direction = Normalize(Vector3(1.0f, -1.0f, 0.5f));

	//-------------------------------------------------------
	// アセットのロード
	//-------------------------------------------------------
	uint32_t skyBoxTexutreIndex = MAGISYSTEM::LoadTexture("kloppenheim_06_puresky_2k.dds");

	//===================================
	// テクスチャのロード
	//===================================

	// ロックオン用テクスチャ
	MAGISYSTEM::LoadTexture("LockonUIGray.png");
	MAGISYSTEM::LoadTexture("LockonUIRed.png");

	// APUI用テクスチャ
	MAGISYSTEM::LoadTexture("ApGauge.png");
	MAGISYSTEM::LoadTexture("ApBar.png");

	MAGISYSTEM::LoadTexture("BossApBar.png");
	MAGISYSTEM::LoadTexture("BossApGauge.png");

	// 円形テクスチャ
	MAGISYSTEM::LoadTexture("Circle2.png");

	// ホワイトテクスチャ
	MAGISYSTEM::LoadTexture("white.png");

	// 靄用
	MAGISYSTEM::LoadTexture("smoke.png");
	// リング用
	MAGISYSTEM::LoadTexture("gradation.png");
	// 粒子用
	MAGISYSTEM::LoadTexture("star.png");

	// エミッターとパーティクルを作成
	MAGISYSTEM::CreateEmitter3D("Star", Vector3(0.0f, 0.0f, 0.0f));
	MAGISYSTEM::CreatePrimitiveParticleGroup3D("Star", Primitive3DType::Plane, "star.png");

	// エミッターとパーティクルを作成
	MAGISYSTEM::CreateEmitter3D("Haze", Vector3(0.0f, 0.0f, 0.0f));
	MAGISYSTEM::CreatePrimitiveParticleGroup3D("Haze", Primitive3DType::Plane, "smoke.png");

	//===================================
	// モデルのロード
	//===================================

	MAGISYSTEM::LoadModel("teapot");
	MAGISYSTEM::CreateModelDrawer("teapot", MAGISYSTEM::FindModel("teapot"));

	MAGISYSTEM::LoadModel("StageObj0");
	MAGISYSTEM::CreateModelDrawer("StageObj0", MAGISYSTEM::FindModel("StageObj0"));

	MAGISYSTEM::LoadModel("Ground");
	MAGISYSTEM::CreateModelDrawer("Ground", MAGISYSTEM::FindModel("Ground"));

	MAGISYSTEM::LoadModel("MechHead");
	MAGISYSTEM::CreateModelDrawer("MechHead", MAGISYSTEM::FindModel("MechHead"));

	MAGISYSTEM::LoadModel("MechBody");
	MAGISYSTEM::CreateModelDrawer("MechBody", MAGISYSTEM::FindModel("MechBody"));

	MAGISYSTEM::LoadModel("MechRightArm");
	MAGISYSTEM::CreateModelDrawer("MechRightArm", MAGISYSTEM::FindModel("MechRightArm"));

	MAGISYSTEM::LoadModel("MechLeftArm");
	MAGISYSTEM::CreateModelDrawer("MechLeftArm", MAGISYSTEM::FindModel("MechLeftArm"));

	MAGISYSTEM::LoadModel("MechLeg");
	MAGISYSTEM::CreateModelDrawer("MechLeg", MAGISYSTEM::FindModel("MechLeg"));

	MAGISYSTEM::LoadModel("AssultRifle");
	MAGISYSTEM::CreateModelDrawer("AssultRifle", MAGISYSTEM::FindModel("AssultRifle"));

	MAGISYSTEM::LoadModel("Bullet");
	MAGISYSTEM::CreateModelDrawer("Bullet", MAGISYSTEM::FindModel("Bullet"));


	//-------------------------------------------------------
	// シーン固有の初期化処理
	//-------------------------------------------------------

	// スカイボックスを設定
	MAGISYSTEM::SetSkyBoxTextureIndex(skyBoxTexutreIndex);

	//===========================
	// マネージャの初期化
	//===========================

	// 攻撃コリジョンマネージャ
	attackCollisionManager_ = std::make_unique<AttackCollisionManager>();

	// 弾マネージャ
	bulletManger_ = std::make_unique<BulletManager>(attackCollisionManager_.get());

	// プレイヤー作成
	player_ = std::make_unique<Player>(bulletManger_.get());

	// 敵作成
	enemy_ = std::make_unique<Enemy>(bulletManger_.get(), player_->GetMechCore());

	// プレイヤーのターゲット対象に敵を追加
	player_->GetMechCore().lock()->GetLockOnComponent()->AddMech(enemy_->GetMechCore());
	// エネミーのターゲット対象にプレイヤーを追加
	enemy_->GetMechCore().lock()->GetLockOnComponent()->AddMech(player_->GetMechCore());

	// プレイヤーのボス対象にエネミーを追加
	player_->SetBossMech(enemy_->GetMechCore());


	// 攻撃コリジョンマネージャにワールドに存在するmechを追加
	attackCollisionManager_->AddMech(player_->GetMechCore());
	attackCollisionManager_->AddMech(enemy_->GetMechCore());

	//===========================
	// 以下ほぼほぼデバッグ用
	//===========================


	planeEffect_.Initialize();
	planeEffect_.blendMode = BlendMode::Normal;
	planeEffect_.translate.isAnimated = true;
	planeEffect_.translate.startTime = 0.0f;
	planeEffect_.translate.animation.SetEasing(EasingType::EaseInCubic);
	planeEffect_.translate.animation.SetEnd(Vector3(0.0f, 0.5f, 0.0f));
	planeEffect_.color.isAnimated = true;
	planeEffect_.color.animation.SetEnd(Vector4(1.0f, 1.0f, 1.0f, 0.0f));


	//-------------------------------------------------------
	// シーンデータのロード
	//-------------------------------------------------------

	MAGISYSTEM::LoadSceneDataFromJson("SceneData");
	MAGISYSTEM::ImportSceneData("SceneData", true);


	//
	// タイマーリセット
	// 
	tempBattleTime_ = 0.0f;
	info.battleTime = kMaxBattleTime_;

}

template<typename Data>
inline void PlayScene<Data>::Update() {

#if defined(DEBUG) || defined(DEVELOP)
	ImGui::Begin("SceneDebugUI");
	ImGui::Text("BattleTime:%u", info.battleTime);
	ImGui::End();
#endif

	// ライト変数
	MAGISYSTEM::SetDirectionalLight(directionalLight_);

	// プレイヤー更新
	player_->Update();

	// 敵更新
	enemy_->Update();

	// 弾マネージャ更新
	bulletManger_->Update();

	// 攻撃判定更新
	attackCollisionManager_->Update();

	// ポストエフェクト適用
	MAGISYSTEM::ApplyPostEffectVignette(vignetteScale_, vignetteFalloff_);
	MAGISYSTEM::ApplyPostEffectGaussianX(gaussianSigma_, 13);
	MAGISYSTEM::ApplyPostEffectGaussianY(gaussianSigma_, 13);
	MAGISYSTEM::ApplyPostEffectDepthNormalOutline();

	// 
	// タイマー更新
	// 

	tempBattleTime_ += MAGISYSTEM::GetDeltaTime();

	// 一秒経ったらタイマーをマイナス
	if (tempBattleTime_ >= 1.0f) {
		tempBattleTime_ = 0.0f;
		info.battleTime--;
	}


	// 
	// シーン終了判定
	// 

	if (player_->GetMechCore().lock()->GetStatusComponent()->GetHp() == 0) {
		info.judge = FinishJudgment::Enemy;
		info.finish = true;
	}

	if (enemy_->GetMechCore().lock()->GetStatusComponent()->GetHp() == 0) {
		info.judge = FinishJudgment::Player;
		info.finish = true;
	}

	if (player_->GetMechCore().lock()->GetStatusComponent()->GetHp() == 0 && enemy_->GetMechCore().lock()->GetStatusComponent()->GetHp() == 0) {
		info.judge = FinishJudgment::Draw;
		info.finish = true;
	}

	if (info.battleTime == 0) {
		// 体力割合が多いほうが勝利
		float playerHPRaito = player_->GetMechCore().lock()->GetStatusComponent()->GetHPRaito();
		float enemyHPraito = enemy_->GetMechCore().lock()->GetStatusComponent()->GetHPRaito();

		if (playerHPRaito > enemyHPraito) {
			info.judge = FinishJudgment::Player;
		} else if (enemyHPraito < playerHPRaito) {
			info.judge = FinishJudgment::Enemy;
		} else {
			info.judge = FinishJudgment::Draw;
		}

		info.finish = true;
	}

	// ひとまず即タイトルに戻る
	if (info.finish) {
		this->ChangeScene("Title");
	}

}

template<typename Data>
inline void PlayScene<Data>::Draw() {
	// プレイヤーにまつわるもの描画
	player_->Draw();

	// 弾マネージャ描画
	bulletManger_->Draw();

	// 攻撃判定マネージャ描画
	attackCollisionManager_->Draw();

}

template<typename Data>
inline void PlayScene<Data>::Finalize() {
	MAGISYSTEM::ClearGameObject3D();
}