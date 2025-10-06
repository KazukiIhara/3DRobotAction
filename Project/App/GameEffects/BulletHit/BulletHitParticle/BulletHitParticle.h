#pragma once

#include "Transform3D/Transform3D.h"

/// <summary>
/// 弾着弾時のパーティクル
/// </summary>
class BulletHitParticle {
public:
	BulletHitParticle(const Vector3& pos, const Vector3& velo);
	~BulletHitParticle() = default;

	void Update();

	void Draw();

private:

private:
	Transform3D transform_;
};