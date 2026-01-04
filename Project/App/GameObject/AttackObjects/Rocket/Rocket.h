#pragma once

// C++
#include <memory>

// MyHedder
#include "Math/Utility/MathUtility.h"
#include "GameObject/AttackObjects/BaseAttackObject/BaseAttackObject.h"
#include "Structs/Primitive3DStruct.h"

// 前方宣言
class Transform3D;

/// <summary>
/// ロケット弾
/// </summary>
class Rocket : public BaseAttackObject {
public:
	Rocket(const Vector3& dir,
		float speed,
		const Vector3& wPos,
		std::weak_ptr<AttackCollider> attackCollider);

	~Rocket() = default;

	void Update();
	void Draw();
	void Finalize();

	Vector3 GetWorldPos();

private:
	void OnFinalize() override;

private:
	// トランスフォーム
	Transform3D* transform_;
	// 球体データ
	SphereData3D sphereData_;
	// マテリアル
	MaterialData3D material_;
	// 弾の向き
	Vector3 dir_ = { 0.0f,0.0f,0.0f };
	// 弾の速度
	float speed_ = 0.0f;

	// 初期生存時間
	float baseLifeTime_ = 5.0f;
};