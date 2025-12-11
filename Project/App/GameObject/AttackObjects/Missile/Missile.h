#pragma once

// C++
#include <memory>

// MyHedder
#include "Math/Utility/MathUtility.h"
#include "GameObject/AttackCollider/AttackCollider.h"

// 前方宣言
class GameObject3D;
class MechCore;

// ミサイルの種類
enum class MissileType {
	Dual,
};

// ミサイルのフェーズ
enum class MissilePhase {
	Boost,
	Guided,
};

/// <summary>
/// ミサイル
/// </summary>
class Missile {
public:
	Missile(
		const MissileType& missileType,
		const Vector3& wPos,
		const Vector3& dir,
		std::weak_ptr<MechCore> target,
		std::weak_ptr<AttackCollider> attackCollider
	);
	~Missile() = default;

	void Update();

	void Finalize();

	bool GetIsAlive()const;

	AttackCollider* GetAttackCollider();

	Vector3 GetWorldPos();

private:
	// ブーストから追従状態に遷移する時の処理
	void EnterGuidedDualMissile();

	// 双対ミサイルの更新処理
	void UpdateDualMissile();
	

private:
	// ゲームオブジェクト
	std::weak_ptr<GameObject3D> missile_;

	// ロックオン対象の機体
	std::weak_ptr<MechCore> target_;

	// ミサイルの種類
	MissileType type_;

	// 現在のフェーズ
	MissilePhase phase_;

	// 進行方向
	Vector3 dir_ = { 0.0f,0.0f,0.0f };

	// ブースト時間
	float boostTime_ = 0.0f;

	// 速度
	float speed_ = 0.0f;
	// ブースト時の加速量
	float boostAcc_ = 0.0f;
	// ブースト時の最大速度
	float maxBoostSpeed_ = 0.0f;

	// 追尾時の加速量
	float guidedAcc_ = 0.0f;
	// 追尾時の最大速度
	float maxGuidedSpeed_ = 0.0f;

	// 生存フラグ
	bool isAlive_ = false;

	// 最大生存時間
	float lifeTime_ = 5.0f;

	// 攻撃コライダー
	std::weak_ptr<AttackCollider> collider_;
};