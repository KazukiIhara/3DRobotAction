#pragma once

// C++
#include <memory>

// MyHeader
#include "Math/Utility/MathUtility.h"
#include "GameObject/AttackObjects/BaseAttackObject/BaseAttackObject.h"
#include "Common/Particle/ParticleEffectCommon.h"
#include "Structs/Primitive3DStruct.h"
#include "Structs/ModelStruct.h"

// 前方宣言
class Transform3D;

/// <summary>
/// 弾
/// </summary>
class Bullet: public BaseAttackObject {
public:
	Bullet(const Vector3& dir,
		float speed,
		const Vector3& wPos,
		std::weak_ptr<AttackCollider> attackCollider);

	~Bullet() = default;

	void Update();
	void Draw();

private:

	// マテリアル
	ModelMaterial material_{};

	// 弾の向き
	Vector3 dir_ = { 0.0f,0.0f,0.0f };
	// 弾の速度
	float speed_ = 0.0f;

	// 初期生存時間
	float baseLifeTime_ = 5.0f;

	// パーティクルのデータ
	GPUParticleEmitData particleData_;

	// 直前に軌跡用パーティクルを発生した位置
	Vector3 lastEmitPos_{};

	// パーティクルの生存時間
	float particleBaseLife_ = 0.5f;

	// 粒子/秒
	float emitRate_ = 1000.0f;
	// 積算レート
	float emitAcc_ = 0.0f;
};
