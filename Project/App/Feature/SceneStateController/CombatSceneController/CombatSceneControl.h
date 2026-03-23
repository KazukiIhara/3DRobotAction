#pragma once

class GameInputSystem;
class Pilot;
class Boss;
class GameEffectManager;
class DamageCollisionSystem;
class DamageObjectManager;
class GameUIManager;
class CombatSceneController;

namespace Magi {
	class TPSCamera3D;
}

namespace CombatSceneControl {
	struct ContextRef {
		GameInputSystem* inputSys;
		Magi::TPSCamera3D* camera;
		Pilot* pilot;
		Boss* boss;
		GameEffectManager* effectMgr;
		DamageCollisionSystem* collisionSys;
		DamageObjectManager* damageObjMgr;
		GameUIManager* gameUIMgr;
	};

	struct StateContextRef {
		GameInputSystem* inputSys;
		Magi::TPSCamera3D* camera;
		Pilot* pilot;
		Boss* boss;
		GameEffectManager* effectMgr;
		DamageCollisionSystem* collisionSys;
		DamageObjectManager* damageObjMgr;
		GameUIManager* gameUIMgr;
		CombatSceneController* cbc;
	};
}