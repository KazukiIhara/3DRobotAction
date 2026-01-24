#include "MechKinematicSystem.h"

// C++
#include <algorithm>

#include "Feature/Mech/Base/BaseMech.h"
#include "3D/Transform3D/Transform3D.h"
#include "Feature/CombatStage/Data/CombatStageData.h"

#include "MAGI.h"

using namespace Magi;

MechKinematicSystem::MechKinematicSystem(BaseMech* mech) {
	mech_ = mech;
}

void MechKinematicSystem::Update() {
	if (!mech_) return;

	// 機体AABB更新
	UpdateMechAABB();

	// ステージコライダー取得
	const auto* stage = mech_->GetCombatStageData();
	if (!stage) return;

	// 押し戻し
	ResolveStageCollision(stage->GetCollider());

	// 接地判定
	UpdateGrounded(stage->GetCollider());
}

void MechKinematicSystem::Draw() {
	// 接地なら色を変える
	Vector4 color = isGrounded_ ? Color::Yellow : Color::Green;

	// 機体AABB描画
	MAGISYSTEM::DrawLineAABB(mechAabb_.min, mechAabb_.max, color);
}

bool MechKinematicSystem::IsGrounded() const {
	return isGrounded_;
}

void MechKinematicSystem::ResolveStageCollision(const std::vector<CombatStage::AABB>& stageAabbs) {
	// 角ケース対策の反復回数
	const int kIter = 4;

	for (int iter = 0; iter < kIter; ++iter) {
		// 機体AABB更新
		UpdateMechAABB();

		const Vector3 mechCenter = Center(mechAabb_);
		bool solved = true;

		for (const auto& s : stageAabbs) {
			// 無効は無視
			if (!s.isActive_) continue;

			// 非交差は無視
			if (!Intersect(mechAabb_, s)) continue;

			// 各軸の重なり量
			const float ox = Overlap1D(mechAabb_.min.x, mechAabb_.max.x, s.min.x, s.max.x);
			const float oy = Overlap1D(mechAabb_.min.y, mechAabb_.max.y, s.min.y, s.max.y);
			const float oz = Overlap1D(mechAabb_.min.z, mechAabb_.max.z, s.min.z, s.max.z);

			// 最小押し出し軸を選ぶ
			float push = ox;
			Vector3 axis{ 1.0f, 0.0f, 0.0f };

			if (oy < push) {
				push = oy; axis = { 0.0f, 1.0f, 0.0f };
			}
			if (oz < push) {
				push = oz; axis = { 0.0f, 0.0f, 1.0f };
			}

			// 押し出し方向を決める
			const Vector3 sCenter = Center(s);
			const float dir = (axis.x != 0.0f) ? ((mechCenter.x < sCenter.x) ? -1.0f : 1.0f)
				: (axis.y != 0.0f) ? ((mechCenter.y < sCenter.y) ? -1.0f : 1.0f)
				: ((mechCenter.z < sCenter.z) ? -1.0f : 1.0f);

			// 少し余分に押す
			const float eps = 1.0e-4f;

			// 中心を更新
			Vector3 newCenter = mechCenter;
			newCenter.x += axis.x * dir * (push + eps);
			newCenter.y += axis.y * dir * (push + eps);
			newCenter.z += axis.z * dir * (push + eps);

			// 反映
			ApplyCenter(newCenter);

			// 機体AABB更新
			UpdateMechAABB();

			// 再評価
			solved = false;
			break;
		}

		// 解決済みなら終了
		if (solved) break;
	}
}

void MechKinematicSystem::UpdateGrounded(const std::vector<CombatStage::AABB>& stageAabbs) {
	// 接地フラグ初期化
	isGrounded_ = false;

	// 機体AABB更新
	UpdateMechAABB();

	for (const auto& s : stageAabbs) {
		// 無効は無視
		if (!s.isActive_) continue;

		// XZが重なっているか
		const bool overlapX = (mechAabb_.max.x > s.min.x) && (mechAabb_.min.x < s.max.x);
		const bool overlapZ = (mechAabb_.max.z > s.min.z) && (mechAabb_.min.z < s.max.z);
		if (!overlapX || !overlapZ) continue;

		// 地面の高さとの差
		const float dy = mechAabb_.min.y - s.max.y;

		// ほぼ接触していれば接地
		if (dy >= -groundEps_ && dy <= groundEps_) {
			isGrounded_ = true;
			return;
		}
	}
}

void MechKinematicSystem::UpdateMechAABB() {
	// 機体
	const Vector3 mechPos = mech_->GetTransform()->GetWorldPosition();

	// パラメータを取得
	offset_ = MAGISYSTEM::GetParameterValue<Vector3>({ "PilotMechParam","KinematicSystem","AABBOffset" });
	const Vector3 halfSize = MAGISYSTEM::GetParameterValue<Vector3>({ "PilotMechParam","KinematicSystem","AABBHalfSize" });

	const Vector3 center = mechPos + offset_;

	// min/max を作る
	mechAabb_.min = { center.x - halfSize.x, center.y - halfSize.y, center.z - halfSize.z };
	mechAabb_.max = { center.x + halfSize.x, center.y + halfSize.y, center.z + halfSize.z };
}

void MechKinematicSystem::ApplyCenter(const Vector3& center) {
	// ルートTransformに反映
	Transform3D* t = mech_->GetTransform();
	if (!t) return;

	const Vector3 rootPos = center - offset_;
	t->SetTranslate(rootPos);
}

bool MechKinematicSystem::Intersect(const AABB& a, const CombatStage::AABB& b) {
	// AABB交差判定
	if (a.max.x <= b.min.x || a.min.x >= b.max.x) return false;
	if (a.max.y <= b.min.y || a.min.y >= b.max.y) return false;
	if (a.max.z <= b.min.z || a.min.z >= b.max.z) return false;
	return true;
}

Vector3 MechKinematicSystem::Center(const AABB& a) {
	// 中心計算
	return { (a.min.x + a.max.x) * 0.5f, (a.min.y + a.max.y) * 0.5f, (a.min.z + a.max.z) * 0.5f };
}

Vector3 MechKinematicSystem::Center(const CombatStage::AABB& a) {
	// 中心計算
	return { (a.min.x + a.max.x) * 0.5f, (a.min.y + a.max.y) * 0.5f, (a.min.z + a.max.z) * 0.5f };
}

float MechKinematicSystem::Overlap1D(float aMin, float aMax, float bMin, float bMax) {
	// 1D重なり量
	const float d1 = bMax - aMin;
	const float d2 = aMax - bMin;
	return std::min(d1, d2);
}
