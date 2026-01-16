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
	struct Sphere {
		Vector3 center;
		float radius;
	};
	struct OBB {
		Vector3 center;
		Vector3 axis[3];
		Vector3 halfSize;
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
		MAGIAssert::Assert(p); // 違ったらバグとして止める
		return *p;
	}

private:
	// パラメータ
	Param param_;

};