#pragma once

class GameInputSystem;
class Pilot;
class Boss;
class GameEffectManager;
class DamageCollisionSystem;
class DamageObjectManager;

namespace CombatSceneControl {
	struct ContextRef {
		GameInputSystem* inputSys;
		Pilot* pilot;
		Boss* boss;
		GameEffectManager* effectMgr;
		DamageCollisionSystem* collisionSys;
		DamageObjectManager* damageObjMgr;
	};
}