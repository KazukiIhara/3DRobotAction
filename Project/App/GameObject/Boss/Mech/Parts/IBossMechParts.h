#pragma once

/// <summary>
/// ボス機体パーツのインターフェース
/// </summary>
class IBossMechParts {
public:
	virtual ~IBossMechParts() = default;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void DebugDraw() = 0;
};
