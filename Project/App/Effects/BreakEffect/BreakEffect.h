#pragma once

#include <unordered_map>

#include "Framework/MAGI.h"

#include "SimpleAnimation/SimpleAnimation.h"

/// <summary>
/// 破壊時エフェクト
/// </summary>
class BreakEffect {
private:
	/// <summary>
	/// 状態を表す
	/// </summary>
	enum class State {
		Electric,
		Haze,
		Explosion,
		Finish,
	};
public:

	/// <param name="worldPosition">破壊エフェクトを出すワールド座標を設定</param>
	BreakEffect(Vector3 worldPosition);

	~BreakEffect();

	void SetRingRotates(std::array<Vector3, 4> rotates);
	void SetRingColor(Vector4 color);

	/// <param name="positionOffset">最初に指定した座標から動かしたい場合オフセット値を入力</param>
	void Update();

	void Draw();

	/// <summary>
	/// 終了通知
	/// </summary>
	bool IsFinished();

private:
	void UpdateElectric();
	void UpdateHaze();
	void UpdateExplosion();
	void UpdateFinish();

	void DrawElectric();
	void DrawHaze();
	void DrawExplosion();
	void DrawFinish();

private:
	//============================
	// エフェクトの構成要素
	//
	// 0 電撃がバチバチ発生
	// 1 オレンジがかった靄が発生
	// 2 メインのエフェクトのオレンジのリング4個 青いフラッシュを出す　ここで縦にカメラシェイク
	// 3 粒子状に舞う水色のパーティクル

	// エフェクトの中心座標
	Vector3 corePosition_;

	// 現在の状態
	State currentState_;
	float timer_ = 0.0f;

	// ステートごとの時間
	float electricTime_ = 1.0f;
	float hazeTime_ = 0.5f;
	float explosionTime_ = 1.5f;
	float finishTime_ = 1.0f;

	// 終了フラグ
	bool isFinished_ = false;

	//====================================
	// リングエフェクト用変数
	//====================================

	// リング用のデータ
	// 形状
	std::array<RingData3D, 4> ringDatas_;
	// マテリアル
	PrimitiveMaterialData3D ringMaterial_{};
	// 回転
	std::array<Vector3, 4> ringRotates_;

	// リングのアニメーション
	std::unique_ptr<SimpleAnimation<float>> ringOuterAnimation_[2];
	std::unique_ptr<SimpleAnimation<float>> ringInnerAnimation_[2];

	//===================================
	// 靄用のパーティクル
	//===================================

	Emitter3D* hazeEmitter_;
	BaseParticleGroup3D* hazeParticle_;
	EmitterSetting hazeSetting_;

	//===================================
	// パーティクル用変数
	//===================================

	Emitter3D* starEmitter_;
	BaseParticleGroup3D* starParticle_;
	EmitterSetting starSetting_;

};