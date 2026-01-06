#pragma once

// C++
#include <string>

// 数学タイプヘッダ
#include "Math/Types/AllMathTypes.h"

/// <summary>
/// 2DゲームUIの基底クラス
/// </summary>
class GameUI2D {
public:
	GameUI2D(const std::string& name);
	virtual ~GameUI2D() {};

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	void SetPos(const Vector2& pos);

	const Vector2 GetPos()const;
	const std::string GetName()const;

protected:
	// ポジションからUIごとのアンカーポイントへ座標を変換する
	Vector2 TransPosition(const Vector2& anchor, const Vector2& size) const;
protected:
	// UI名前
	std::string name_;

	// UI中央座標
	Vector2 centerPos_{};
};