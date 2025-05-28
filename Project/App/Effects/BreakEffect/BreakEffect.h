#pragma once


#include <unordered_map>

#include "Framework/MAGI.h"


/// <summary>
/// 状態を表す
/// </summary>
enum State {
	Electric,
	Haze,
	Explosion,
};

/// <summary>
/// フェーズ
/// </summary>
struct Phase {
	// どの状態か
	State state;
	// この状態の時間
	uint32_t time;
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

	/// <summary>
	/// 終了通知
	/// </summary>
	bool IsFinished();

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



	//====================================
	// リングエフェクト用変数
	//====================================

	// リング用のデータ
	std::array<RingData3D, 4> ringDatas_;


	//===================================
	// パーティクル用変数
	//===================================


};