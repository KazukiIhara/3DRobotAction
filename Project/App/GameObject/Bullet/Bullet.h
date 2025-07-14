#pragma once

// C++
#include <memory>

// MyHedder
#include "Math/Utility/MathUtility.h"


// 前方宣言
class GameObject3D;

/// <summary>
/// 識別タグ
/// </summary>
enum class FriendlyTag {
	Player,
	Enemy
};

/// <summary>
/// 弾
/// </summary>
class Bullet {
public:
	Bullet(const FriendlyTag& tag, const Vector3& dir, float speed, const Vector3& wPos);
	~Bullet() = default;

	void Update();
	void Draw();

	void Finalize();

	bool GetIsAlive()const;

private:
	// ゲームオブジェクト
	std::weak_ptr<GameObject3D> bullet_;
	// 識別タグ
	FriendlyTag tag_;
	// 弾の向き
	Vector3 dir_ = { 0.0f,0.0f,0.0f };
	// 弾の速度
	float speed_ = 0.0f;

	// 最大生存時間
	float lifeTime_ = 10.0f;
	// 生存フラグ
	bool isAlive_ = false;
};