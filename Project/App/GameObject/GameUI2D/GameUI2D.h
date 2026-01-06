#pragma once

// 数学タイプヘッダ
#include "Math/Types/AllMathTypes.h"

/// <summary>
/// 2DゲームUI
/// </summary>
class GameUI2D {
public:
	virtual ~GameUI2D() {};

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

protected:
	// ポジションからUIごとのアンカーポイントへ座標を変換する
	Vector2 TransPosition(const Vector2& anchor);
protected:
	// UI中央座標
	Vector2 centerPos_{};
};