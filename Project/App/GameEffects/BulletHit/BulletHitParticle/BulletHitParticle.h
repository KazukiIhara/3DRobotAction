#pragma once

// C++
#include <memory>

// MyHedder
#include "magi::Transform3D/magi::Transform3D.h"

#include "Structs/ModelStruct.h"

/// <summary>
/// 弾着弾時のパーティクル
/// </summary>
class BulletHitParticle {
public:
	BulletHitParticle(const magi::Vector3& pos, const magi::Vector3& velo, float gravity);
	~BulletHitParticle() = default;

	void Update();

	void Draw();

	bool GetIsAlive()const;

private:
	// 重力加速度
	float gravity_ = -9.8f;
	// 移動量
	magi::Vector3 velocity_ = { 0.0f,0.0f,1.0f };

	// 生存時間
	float lifeTime_ = 0.3f;

	// 生存フラグ
	bool isAlive_ = true;

	// マテリアル
	ModelMaterial mat_;

private:
	// パーティクルのトランスフォーム
	std::unique_ptr<magi::Transform3D> transform_;
};