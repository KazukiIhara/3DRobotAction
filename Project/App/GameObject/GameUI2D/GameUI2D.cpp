#include "GameUI2D.h"

// 数学関数ヘッダ
#include "Math/Utility/MathUtility.h"

GameUI2D::GameUI2D(const std::string& name) {
	name_ = name;
}

void GameUI2D::SetPos(const Vector2& pos) {
	centerPos_ = pos;
}

const Vector2 GameUI2D::GetPos() const {
	return centerPos_;
}

const std::string GameUI2D::GetName()const {
	return name_;
}

Vector2 GameUI2D::TransPosition(const Vector2& anchor, const Vector2& size) const {
	const Vector2 centerAnchor{ 0.5f, 0.5f };
	return centerPos_ + (anchor - centerAnchor) * size;
}
