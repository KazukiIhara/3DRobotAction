#include "BasePilotMechWeapon.h"

#include "Feature/Pilot/Mech/PilotMech.h"

PilotMech* BasePilotMechWeapon::GetPilotMech() {
	return dynamic_cast<PilotMech*>(mech_);
}