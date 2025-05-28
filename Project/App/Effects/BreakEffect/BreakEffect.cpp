#include "BreakEffect.h"

BreakEffect::BreakEffect(Vector3 worldPosition) {
	corePosition_ = worldPosition;


}

BreakEffect::~BreakEffect() {

}

void BreakEffect::Update(Vector3 positionOffset) {
	positionOffset;
}

bool BreakEffect::IsFinished() {
	return false;
}
