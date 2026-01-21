#include "PilotMechJustDodgeCollider.h"

#include "Feature/Pilot/Mech/PilotMech.h"

#include "MAGI.h"

using namespace Magi;

PilotMechJustDodgeCollider::PilotMechJustDodgeCollider(PilotMech* mech) {
	mech_ = mech;
	collider_.center = mech_->GetCenterPos();
	collider_.radius = MAGISYSTEM::GetParameterValue<float>({ "PilotMechParam","JustDodgeCollider","Radius" });

}

void PilotMechJustDodgeCollider::Update() {
	isHit_ = false;
	collider_.center = mech_->GetCenterPos();
	collider_.radius = MAGISYSTEM::GetParameterValue<float>({ "PilotMechParam","JustDodgeCollider","Radius" });

}

PilotMechJustDodgeCollider::Sphere PilotMechJustDodgeCollider::GetCollider() {
	return collider_;
}

void PilotMechJustDodgeCollider::Draw() {
	MAGISYSTEM::DrawLineSphere(collider_.center, collider_.radius, Color::Coral);
}

void PilotMechJustDodgeCollider::SetIsHit(bool isHit) {
	isHit_ = isHit;
}

bool PilotMechJustDodgeCollider::GetIsHit() {
	return isHit_;
}