#pragma once

// 基底クラス
#include "Feature/Damage/Object/Base/BaseDamageObject.h"

/// <summary>
/// 発射物の攻撃基底クラス
/// </summary>
class BaseProjectileAttack:public BaseDamageObject {
public:
	struct InitParam {
		Vector3 emitPos{};
		Vector3 dir{};
		float speed = 0.0f;
		float life = 0.0f;
		FriendlyTag tag;
	};
public:
	BaseProjectileAttack();
	~BaseProjectileAttack() = default;

private:

};

BaseProjectileAttack::BaseProjectileAttack() {}

BaseProjectileAttack::~BaseProjectileAttack() {}