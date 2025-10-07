#pragma once

// C++
#include <memory>

// MyHedder
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

	bool GetIsAlive()const;

private:
	// 重力加速度
	const float gravity_ = 9.8f;
	// 移動量
	Vector3 velocity_ = { 0.0f,0.0f,1.0f };

	// 生存時間
	float lifeTime_ = 1.0f;

	// 生存フラグ
	bool isAlive_ = true;

private:
	// パーティクルのトランスフォーム
	std::unique_ptr<Transform3D> transform_;
};