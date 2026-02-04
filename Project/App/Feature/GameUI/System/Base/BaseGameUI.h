#pragma once

#include "Math/Types/AllMathTypes.h"

/// <summary>
/// ゲームUIクラス
/// </summary>
class BaseGameUI {
public:
	BaseGameUI(const Vector2& pos = Vector2(0.0f, 0.0f));
	virtual ~BaseGameUI() = default;

	virtual void Update() = 0;
	virtual void Draw() = 0;

	void SetScreenPos(const Vector2& screenPos);

	bool IsAlive()const;

private:
	Vector2 screenPos_{};
	bool isAlive_ = true;
};