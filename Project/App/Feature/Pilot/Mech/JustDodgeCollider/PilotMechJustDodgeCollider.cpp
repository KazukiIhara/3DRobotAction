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
	collider_.center = mech_->GetCenterPos();
	collider_.radius = MAGISYSTEM::GetParameterValue<float>({ "PilotMechParam","JustDodgeCollider","Radius" });

}

void PilotMechJustDodgeCollider::Draw() {
	MAGISYSTEM::DrawLineSphere(collider_.center, collider_.radius, Color::Coral);
}