#pragma once

class GameInputSystem;
class TPSCamera3D;
class Pilot;
class Boss;
class GameEffectManager;
class DamageCollisionSystem;
class DamageObjectManager;

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
}