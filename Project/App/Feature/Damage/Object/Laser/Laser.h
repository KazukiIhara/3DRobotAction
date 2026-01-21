#pragma once

// 基底クラス
#include "Feature/Damage/Object/Base/BaseDamageObject.h"

/// <summary>
/// レーザー
/// </summary>
class Laser:public BaseDamageObject {
public:
	struct InitParam {
		Vector3 emitPos{};
		Vector3 dir{};
		float speed = 0.0f;
		float life = 0.0f;
	};
public:
	Laser(const Laser::InitParam& initParam);
	~Laser()override = default;

	void Update()override;
	void Draw()override;
	void Finalize()override;

private:
	Vector3 emitPos_;
	Vector3 dir_;
	float speed_;
	float life_;
};