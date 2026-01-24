#pragma once

// C++
#include <variant>

#include "Math/Types/AllMathTypes.h"
#include "MAGIAssert/MAGIAssert.h"
#include "GameCommon/GameCommon.h"
#include "Feature/Damage/Damage.h"

// メモ

/*
	ParamはどちらかというとShapeだった
	直感的ではないが、今変更するのはコストなので、今後余裕があったら名称をただす
*/

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
	struct Capsule {
		Vector3 p0;
		Vector3 p1;
		float radius;
	};

	// ゲーム内で使うパラメータ
	struct GameParam {
		int32_t damage = 0;
		Damage::Power power = Damage::Power::Small;
		FriendlyTag tag;
	};

	// 衝突情報
	struct HitInfo {
		bool isHit_ = false;
	};
public:
	using Param = std::variant<
		Sphere,
		OBB,
		Capsule
	>;
public:
	DamageCollider(Param param, const GameParam& gParam);
	~DamageCollider() = default;

	void Update();
	void DebugDraw();

	// パラメータ取得
	const Param& GetParam() const;

	// パラメータの参照を取得
	template<class T>
	T& GetParamMutableRef() {
		T* p = std::get_if<T>(&param_);
		MAGIAssert::Assert(p, "DamageCollider: Param type mismatch");
		return *p;
	}

	void SetHitInfo(const HitInfo& info);
	HitInfo GetHitInfo()const;

	// 生存フラグをセット
	void SetIsAlive(bool isAlive);

	// 生存フラグを取得
	bool GetIsAlive()const;

	// ゲームで使うパラメータを取得
	GameParam GetGameParam()const;

private:
	// パラメータ
	Param param_{};

	// 衝突情報
	HitInfo hitInfo_{};

	// ゲーム内で使うパラメータ
	GameParam gParam_{};

	// 生存フラグ
	bool isAlive_ = true;

};