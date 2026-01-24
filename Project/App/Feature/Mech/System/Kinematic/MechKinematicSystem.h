#pragma once

// C++
#include <vector>

#include "Math/Types/Vector3.h"
#include "Feature/CombatStage/CombatStage.h"

class BaseMech;

class MechKinematicSystem {
public:
	explicit MechKinematicSystem(BaseMech* mech);
	~MechKinematicSystem() = default;

	void Update();
	void Draw();

	bool IsGrounded() const;

private:
	struct AABB {
		Vector3 min;
		Vector3 max;
	};

	void ResolveStageCollision(const std::vector<CombatStage::AABB>& stageAabbs);
	void UpdateGrounded(const std::vector<CombatStage::AABB>& stageAabbs);

	// この関数のために派生を作る可能性あり
	void UpdateMechAABB();
	void ApplyCenter(const Vector3& center);

	static bool Intersect(const AABB& a, const CombatStage::AABB& b);
	static Vector3 Center(const AABB& a);
	static Vector3 Center(const CombatStage::AABB& a);
	static float Overlap1D(float aMin, float aMax, float bMin, float bMax);

private:
	BaseMech* mech_ = nullptr;

	// 機体AABB
	AABB mechAabb_{};

	Vector3 offset_{ 0.0f, 0.0f, 0.0f };

	// 接地判定の許容誤差
	float groundEps_ = 0.02f;

	bool isGrounded_ = false;
};
