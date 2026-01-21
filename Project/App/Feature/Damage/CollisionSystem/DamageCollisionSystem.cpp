#include "DamageCollisionSystem.h"

// C++
#include <algorithm>
#include <cmath>
#include <limits>
#include <vector>

#include "Math/Utility/MathUtility.h"

using namespace MAGIMath;

namespace {
	// 絶対値
	float Abs(float v) {
		return std::fabs(v);
	}
	// クランプ
	float Clamp(float v, float lo, float hi) {
		return std::clamp(v, lo, hi);
	}

	// 点と線分の距離二乗
	float DistSqPointSegment(const Vector3& p, const Vector3& a, const Vector3& b) {
		// 線分方向
		const Vector3 ab = b - a;
		const float abLenSq = LengthSquared(ab);

		// 退化
		if (abLenSq <= 1e-8f) {
			return LengthSquared(p - a);
		}

		// 射影t
		float t = Dot(p - a, ab) / abLenSq;
		t = Clamp(t, 0.0f, 1.0f);

		// 最近点
		const Vector3 q = a + ab * t;
		return LengthSquared(p - q);
	}

	// 線分と線分の距離二乗
	float DistSqSegmentSegment(const Vector3& p1, const Vector3& q1, const Vector3& p2, const Vector3& q2) {
		// 方向
		const Vector3 d1 = q1 - p1;
		const Vector3 d2 = q2 - p2;
		const Vector3 r = p1 - p2;

		// 係数
		const float a = Dot(d1, d1);
		const float e = Dot(d2, d2);
		const float f = Dot(d2, r);

		float s = 0.0f;
		float t = 0.0f;

		// 退化
		if (a <= 1e-8f && e <= 1e-8f) {
			return LengthSquared(p1 - p2);
		}

		// d1が退化
		if (a <= 1e-8f) {
			s = 0.0f;
			t = Clamp(f / e, 0.0f, 1.0f);
		}
		// d2が退化
		else if (e <= 1e-8f) {
			t = 0.0f;
			s = Clamp(-Dot(d1, r) / a, 0.0f, 1.0f);
		} else {
			// 一般
			const float b = Dot(d1, d2);
			const float c = Dot(d1, r);
			const float denom = a * e - b * b;

			// s算出
			if (denom != 0.0f) {
				s = Clamp((b * f - c * e) / denom, 0.0f, 1.0f);
			} else {
				s = 0.0f;
			}

			// t算出
			t = (b * s + f) / e;

			// tクランプ
			if (t < 0.0f) {
				t = 0.0f;
				s = Clamp(-c / a, 0.0f, 1.0f);
			} else if (t > 1.0f) {
				t = 1.0f;
				s = Clamp((b - c) / a, 0.0f, 1.0f);
			}
		}

		// 最近点差
		const Vector3 c1 = p1 + d1 * s;
		const Vector3 c2 = p2 + d2 * t;
		return LengthSquared(c1 - c2);
	}

	// 線分とAABB(原点中心)の最短距離二乗（厳密）
	float DistSqSegmentAABBCentered(const Vector3& p0, const Vector3& p1, const Vector3& e) {
		// 方向
		const Vector3 d = p1 - p0;

		// 候補t
		std::vector<float> ts;
		ts.reserve(8);
		ts.push_back(0.0f);
		ts.push_back(1.0f);

		// ブレークポイント追加
		auto addBreak = [&](float p, float dir, float bound) {
			if (Abs(dir) <= 1e-8f) {
				return;
			}
			const float t = (bound - p) / dir;
			if (t > 0.0f && t < 1.0f) {
				ts.push_back(t);
			}
			};

		addBreak(p0.x, d.x, -e.x);
		addBreak(p0.x, d.x, e.x);
		addBreak(p0.y, d.y, -e.y);
		addBreak(p0.y, d.y, e.y);
		addBreak(p0.z, d.z, -e.z);
		addBreak(p0.z, d.z, e.z);

		// ソート
		std::sort(ts.begin(), ts.end());

		// 近接計算
		float best = std::numeric_limits<float>::infinity();

		for (size_t i = 0; i + 1 < ts.size(); ++i) {
			const float aT = ts[i];
			const float bT = ts[i + 1];

			// 区間代表
			const float mid = (aT + bT) * 0.5f;

			// mid点
			const Vector3 pm = p0 + d * mid;

			// クランプ先
			bool anyOutside = false;
			Vector3 s{};
			s.x = pm.x < -e.x ? -e.x : (pm.x > e.x ? e.x : pm.x);
			s.y = pm.y < -e.y ? -e.y : (pm.y > e.y ? e.y : pm.y);
			s.z = pm.z < -e.z ? -e.z : (pm.z > e.z ? e.z : pm.z);

			// 外側判定
			if (pm.x != s.x) anyOutside = true;
			if (pm.y != s.y) anyOutside = true;
			if (pm.z != s.z) anyOutside = true;

			// 全部内側なら距離0
			if (!anyOutside) {
				return 0.0f;
			}

			// 二次式係数
			float qa = 0.0f;
			float qb = 0.0f;
			float qc = 0.0f;

			// x寄与
			if (pm.x != s.x) {
				const float p = p0.x - s.x;
				qa += d.x * d.x;
				qb += d.x * p;
				qc += p * p;
			}
			// y寄与
			if (pm.y != s.y) {
				const float p = p0.y - s.y;
				qa += d.y * d.y;
				qb += d.y * p;
				qc += p * p;
			}
			// z寄与
			if (pm.z != s.z) {
				const float p = p0.z - s.z;
				qa += d.z * d.z;
				qb += d.z * p;
				qc += p * p;
			}

			// 区間内最小t
			float t = aT;
			if (qa > 1e-8f) {
				t = -qb / qa;
				t = Clamp(t, aT, bT);
			}

			// 評価
			auto eval = [&](float tt) {
				const Vector3 p = p0 + d * tt;

				const float cx = p.x < -e.x ? -e.x : (p.x > e.x ? e.x : p.x);
				const float cy = p.y < -e.y ? -e.y : (p.y > e.y ? e.y : p.y);
				const float cz = p.z < -e.z ? -e.z : (p.z > e.z ? e.z : p.z);

				const Vector3 diff{ p.x - cx, p.y - cy, p.z - cz };
				return LengthSquared(diff);
				};

			best = std::min(best, eval(aT));
			best = std::min(best, eval(bT));
			best = std::min(best, eval(t));
		}

		return best;
	}
}

DamageCollisionSystem::DamageCollisionSystem() {
	colliders_.clear();
	hitPairs_.clear();
}

void DamageCollisionSystem::Update() {
	// 死亡コライダー除去
	RemoveDeadColliders_();

	// 衝突結果リセット
	hitPairs_.clear();

	// コライダー更新
	UpdateColliders_();

	// 形状同士の判定
	CheckCollision_();
}

void DamageCollisionSystem::Draw() {
#if defined(DEBUG) || defined(DEVELOP)
	for (auto& c : colliders_) {
		c->DebugDraw();
	}
#endif
}

std::weak_ptr<DamageCollider> DamageCollisionSystem::AddCollider(std::shared_ptr<DamageCollider> collider) {
	colliders_.push_back(std::move(collider));
	return colliders_.back();
}

void DamageCollisionSystem::Clear() {
	colliders_.clear();
	hitPairs_.clear();
}

const std::vector<std::pair<const DamageCollider*, const DamageCollider*>>& DamageCollisionSystem::GetHitPairs() const {
	return hitPairs_;
}

void DamageCollisionSystem::RemoveDeadColliders_() {
	colliders_.erase(
		std::remove_if(colliders_.begin(), colliders_.end(),
			[](const std::shared_ptr<DamageCollider>& c) { return !c->GetIsAlive(); }),
		colliders_.end()
	);
}

void DamageCollisionSystem::UpdateColliders_() {
	for (auto& c : colliders_) {
		c->Update();
	}
}

void DamageCollisionSystem::CheckCollision_() {
	// 全ペア判定
	for (size_t i = 0; i < colliders_.size(); ++i) {
		for (size_t j = i + 1; j < colliders_.size(); ++j) {
			auto& a = colliders_[i];
			auto& b = colliders_[j];

			// 生存のみ
			if (!a->GetIsAlive() || !b->GetIsAlive()) {
				continue;
			}

			// 形状同士判定
			if (IsCollision(a->GetParam(), b->GetParam())) {
				// 衝突ペア登録
				hitPairs_.push_back({ a.get(), b.get() });

				// 衝突情報セット
				a->SetHitInfo({ true });
				b->SetHitInfo({ true });
			}
		}
	}
}

bool DamageCollisionSystem::IsCollision(const DamageCollider::Param& a, const DamageCollider::Param& b) {
	return std::visit([&](const auto& sa, const auto& sb) -> bool {
		using A = std::decay_t<decltype(sa)>;
		using B = std::decay_t<decltype(sb)>;

		if constexpr (std::is_same_v<A, DamageCollider::Sphere> && std::is_same_v<B, DamageCollider::Sphere>) {
			return IsCollisionSphereToSphere(sa, sb);
		}
		else if constexpr (std::is_same_v<A, DamageCollider::Sphere> && std::is_same_v<B, DamageCollider::OBB>) {
			return IsCollisionSphereToOBB(sa, sb);
		}
		else if constexpr (std::is_same_v<A, DamageCollider::OBB> && std::is_same_v<B, DamageCollider::Sphere>) {
			return IsCollisionSphereToOBB(sb, sa);
		}
		else if constexpr (std::is_same_v<A, DamageCollider::OBB> && std::is_same_v<B, DamageCollider::OBB>) {
			return IsCollisionOBBToOBB(sa, sb);
		}
		else if constexpr (std::is_same_v<A, DamageCollider::Capsule> && std::is_same_v<B, DamageCollider::Sphere>) {
			return IsCollisionCapsuleToSphere(sa, sb);
		}
		else if constexpr (std::is_same_v<A, DamageCollider::Sphere> && std::is_same_v<B, DamageCollider::Capsule>) {
			return IsCollisionCapsuleToSphere(sb, sa);
		}
		else if constexpr (std::is_same_v<A, DamageCollider::Capsule> && std::is_same_v<B, DamageCollider::Capsule>) {
			return IsCollisionCapsuleToCapsule(sa, sb);
		}
		else if constexpr (std::is_same_v<A, DamageCollider::Capsule> && std::is_same_v<B, DamageCollider::OBB>) {
			return IsCollisionCapsuleToOBB(sa, sb);
		}
		else if constexpr (std::is_same_v<A, DamageCollider::OBB> && std::is_same_v<B, DamageCollider::Capsule>) {
			return IsCollisionCapsuleToOBB(sb, sa);
		}
		else {
			return false;
		}
	}, a, b);
}


bool DamageCollisionSystem::IsCollisionSphereToSphere(const DamageCollider::Sphere& a, const DamageCollider::Sphere& b) {
	// 距離二乗
	const Vector3 d = a.center - b.center;
	const float distSq = d.x * d.x + d.y * d.y + d.z * d.z;

	// 半径二乗
	const float r = a.radius + b.radius;
	return distSq <= r * r;
}

bool DamageCollisionSystem::IsCollisionSphereToOBB(const DamageCollider::Sphere& s, const DamageCollider::OBB& b) {
	// ローカル差分
	const Vector3 d = s.center - b.center;

	// OBBローカル座標
	float lx = Dot(d, b.axis[0]);
	float ly = Dot(d, b.axis[1]);
	float lz = Dot(d, b.axis[2]);

	// AABBへクランプ
	lx = Clamp(lx, -b.halfSize.x, b.halfSize.x);
	ly = Clamp(ly, -b.halfSize.y, b.halfSize.y);
	lz = Clamp(lz, -b.halfSize.z, b.halfSize.z);

	// 最近点
	const Vector3 closest =
		b.center +
		b.axis[0] * lx +
		b.axis[1] * ly +
		b.axis[2] * lz;

	// 距離二乗
	const Vector3 diff = s.center - closest;
	const float distSq = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;

	return distSq <= s.radius * s.radius;
}

bool DamageCollisionSystem::IsCollisionOBBToOBB(const DamageCollider::OBB& a, const DamageCollider::OBB& b) {
	// 軸
	const Vector3 A[3] = { a.axis[0], a.axis[1], a.axis[2] };
	const Vector3 B[3] = { b.axis[0], b.axis[1], b.axis[2] };

	// 半径
	const float EA[3] = { a.halfSize.x, a.halfSize.y, a.halfSize.z };
	const float EB[3] = { b.halfSize.x, b.halfSize.y, b.halfSize.z };

	// 回転行列
	float R[3][3]{};
	float AbsR[3][3]{};
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			R[i][j] = Dot(A[i], B[j]);
			AbsR[i][j] = Abs(R[i][j]) + 1e-6f;
		}
	}

	// 位置差（Aローカル）
	const Vector3 tW = b.center - a.center;
	float t[3] = { Dot(tW, A[0]), Dot(tW, A[1]), Dot(tW, A[2]) };

	// A軸
	for (int i = 0; i < 3; ++i) {
		const float ra = EA[i];
		const float rb = EB[0] * AbsR[i][0] + EB[1] * AbsR[i][1] + EB[2] * AbsR[i][2];
		if (Abs(t[i]) > ra + rb) {
			return false;
		}
	}

	// B軸
	for (int j = 0; j < 3; ++j) {
		const float ra = EA[0] * AbsR[0][j] + EA[1] * AbsR[1][j] + EA[2] * AbsR[2][j];
		const float rb = EB[j];
		const float tj = Abs(t[0] * R[0][j] + t[1] * R[1][j] + t[2] * R[2][j]);
		if (tj > ra + rb) {
			return false;
		}
	}

	// 交差軸（AixBj）
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			const int i1 = (i + 1) % 3;
			const int i2 = (i + 2) % 3;
			const int j1 = (j + 1) % 3;
			const int j2 = (j + 2) % 3;

			const float ra = EA[i1] * AbsR[i2][j] + EA[i2] * AbsR[i1][j];
			const float rb = EB[j1] * AbsR[i][j2] + EB[j2] * AbsR[i][j1];

			const float tij = Abs(t[i2] * R[i1][j] - t[i1] * R[i2][j]);
			if (tij > ra + rb) {
				return false;
			}
		}
	}

	return true;
}

bool DamageCollisionSystem::IsCollisionCapsuleToSphere(const DamageCollider::Capsule& c, const DamageCollider::Sphere& s) {
	// 最近距離
	const float distSq = DistSqPointSegment(s.center, c.p0, c.p1);

	// 半径二乗
	const float r = c.radius + s.radius;
	return distSq <= r * r;
}

bool DamageCollisionSystem::IsCollisionCapsuleToCapsule(const DamageCollider::Capsule& a, const DamageCollider::Capsule& b) {
	// 最近距離
	const float distSq = DistSqSegmentSegment(a.p0, a.p1, b.p0, b.p1);

	// 半径二乗
	const float r = a.radius + b.radius;
	return distSq <= r * r;
}

bool DamageCollisionSystem::IsCollisionCapsuleToOBB(const DamageCollider::Capsule& c, const DamageCollider::OBB& b) {
	// OBBローカルへ変換
	const Vector3 d0 = c.p0 - b.center;
	const Vector3 d1 = c.p1 - b.center;

	Vector3 p0L{
		Dot(d0, b.axis[0]),
		Dot(d0, b.axis[1]),
		Dot(d0, b.axis[2])
	};
	Vector3 p1L{
		Dot(d1, b.axis[0]),
		Dot(d1, b.axis[1]),
		Dot(d1, b.axis[2])
	};

	// 線分とAABB距離
	const float distSq = DistSqSegmentAABBCentered(p0L, p1L, b.halfSize);

	// 半径二乗
	return distSq <= c.radius * c.radius;
}
