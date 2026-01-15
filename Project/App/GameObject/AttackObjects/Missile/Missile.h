#pragma once

// C++
#include <memory>

// MyHedder
#include "GameObject/AttackObjects/BaseAttackObject/BaseAttackObject.h"

#include "Math/Utility/MathUtility.h"
#include "Structs/ModelStruct.h"

// 前方宣言
class Transform3D;
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
class Missile : public BaseAttackObject {
public:
	Missile(
		const MissileType& missileType,
		const Vector3& wPos,
		const Vector3& dir,
		MechCore* target,
		std::weak_ptr<AttackCollider> attackCollider
	);
	~Missile() = default;

	void Update();
	void Draw();

private:
	// ブーストから追従状態に遷移する時の処理
	void EnterGuidedDualMissile();

	// 双対ミサイルの更新処理
	void UpdateDualMissile();

private:
	// ロックオン対象の機体
	MechCore* target_;

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

	// 初期生存時間
	float baseLifeTime_ = 5.0f;

	ModelMaterial material_{};
};
