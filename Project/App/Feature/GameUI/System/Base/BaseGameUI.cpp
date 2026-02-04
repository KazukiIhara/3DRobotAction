#include "BaseGameUI.h"

BaseGameUI::BaseGameUI(const Vector2& pos) {
	screenPos_ = pos;
}

void BaseGameUI::SetScreenPos(const Vector2& screenPos) {
	screenPos_ = screenPos;
}

bool BaseGameUI::IsAlive() const {
	return isAlive_;
}