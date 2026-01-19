#pragma once

class IMechParts {
public:
	virtual ~IMechParts() = default;

	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void DebugDraw() {}
};