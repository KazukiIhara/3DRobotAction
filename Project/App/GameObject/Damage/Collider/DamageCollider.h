#pragma once

// C++
#include <variant>

#include "Math/Types/AllMathTypes.h"
#include "MAGIAssert/MAGIAssert.h"

/// <summary>
/// ダメージコライダー
/// </summary>
class DamageCollider {
public:
	// 形状
	struct Sphere {
		Vector3 center;
		float radius;
	};
	struct OBB {
		Vector3 center;
		Vector3 axis[3];
		Vector3 halfSize;
	};

	// 衝突情報
	struct HitInfo {
		bool isHit_ = false;
	};
public:
	using Param = std::variant<
		Sphere,
		OBB
	>;
public:
	DamageCollider(Param param);
	~DamageCollider() = default;

	void Update();
	void DebugDraw();

	// パラメータ取得
	const Param& GetParam() const;

	// パラメータの参照を取得
	template<class T>
	T& GetParamMutableRef() {
		T* p = std::get_if<T>(&param_);
		MAGIAssert::Assert(p);
		return *p;
	}

	// 生存フラグをセット
	void SetIsAlive(bool isAlive);

	// 生存フラグを取得
	bool GetIsAlive()const;

private:
	// パラメータ
	Param param_{};

	// 衝突情報
	HitInfo hitInfo_{};

	// 生存フラグ
	bool isAlive_ = true;

};