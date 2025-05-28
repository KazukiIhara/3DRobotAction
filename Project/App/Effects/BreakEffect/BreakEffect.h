#pragma once

#include <unordered_map>

#include "Framework/MAGI.h"

/// <summary>
/// 状態を表す
/// </summary>
enum class State {
	Electric,
	Haze,
	Explosion,
};

/// <summary>
/// 破壊時エフェクト
/// </summary>
class BreakEffect {
public:
	/// <param name="worldPosition">破壊エフェクトを出すワールド座標を設定</param>
	BreakEffect(Vector3 worldPosition);

	~BreakEffect();

	/// <param name="positionOffset">最初に指定した座標から動かしたい場合オフセット値を入力</param>
	void Update(Vector3	positionOffset = { 0.0f,0.0f,0.0f });

	void Draw();
	
	/// <summary>
	/// 終了通知
	/// </summary>
	bool IsFinished();

private:
	void UpdateElectric();
	void UpdateHaze();
	void UpdateExplosion();

private:
	//============================
	// エフェクトの構成要素
	//
	// 0 電撃がバチバチ発生
	// 1 オレンジがかった靄が発生
	// 2 メインのエフェクトのオレンジのリング4個 ここで縦にカメラシェイク
	// 3 粒子状に舞う水色のパーティクル

	// エフェクトの中心座標
	Vector3 corePosition_;

	// 現在の状態
	State currentState_;
	float timer_ = 0.0f;

	// ステートごとの時間
	float electricTime_ = 1.0f;
	float hazeTime_ = 0.5f;
	float explosionTime_ = 1.0f;

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

	//===================================
	// パーティクル用変数
	//===================================


};