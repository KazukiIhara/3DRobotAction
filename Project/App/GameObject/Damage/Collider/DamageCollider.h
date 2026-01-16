#pragma once

// C++
#include <variant>

#include "Math/Types/AllMathTypes.h"

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

	const Param& GetParam() const;

protected:
	template<class T>
	T& GetParamMutableRef() {
		return std::get_if<T>(&param_);
	}

private:
	Param param_;

};