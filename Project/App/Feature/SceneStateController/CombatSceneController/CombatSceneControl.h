#pragma once

class GameInputSystem;
class TPSCamera3D;
class Pilot;
class Boss;
class GameEffectManager;
class DamageCollisionSystem;
class DamageObjectManager;
class CombatSceneController;

namespace CombatSceneControl {
	struct ContextRef {
		GameInputSystem* inputSys;
		TPSCamera3D* camera;
		Pilot* pilot;
		Boss* boss;
		GameEffectManager* effectMgr;
		DamageCollisionSystem* collisionSys;
		DamageObjectManager* damageObjMgr;
	};

	struct StateContextRef {
		GameInputSystem* inputSys;
		TPSCamera3D* camera;
		Pilot* pilot;
		Boss* boss;
		GameEffectManager* effectMgr;
		DamageCollisionSystem* collisionSys;
		DamageObjectManager* damageObjMgr;
		CombatSceneController* cbc;
	};
}