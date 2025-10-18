#pragma once

// C++
#include <cstdint>
#include <string>
#include <memory>

// MyHedder
#include "Math/Types/AllMathTypes.h"
#include "Transform3D/Transform3D.h"

#include "Structs/ModelStruct.h"

/// <summary>
/// カスタマイズ対応型手持ち武器ベースクラス
/// </summary>
class MechHandWeapon {
private:
	/// <summary>
	/// 武器の種類
	/// </summary>
	enum class Type {
		Gun,
		Melee,
	};

	/// <summary>
	/// 共通パラメータ
	/// </summary>
	struct Param {
		// 武器の名前
		std::string name;
		// 武器タイプ
		Type type;
		// ダメージ
		float damage;
		// リロード時間
		float reloadTime;
		// 攻撃間隔
		float coolTime;
		// 装弾数
		uint32_t capacity;
		// 攻撃が飛ぶスピード
		float speed;

		//
		// 内部パラメータ
		//

		// モデル名
		std::string modelName;

		// 攻撃発射位置オフセット
		Vector3 fireOffsetPos;

	};

public:
	MechHandWeapon(const MechHandWeapon::Param& param);
	virtual ~MechHandWeapon() = default;

	virtual void Initialize();
	virtual void Update();
	virtual void Draw();

private:
	// 武器パラメータ
	Param param_;

	// トランスフォーム
	std::unique_ptr<Transform3D> transform_;

	// マテリアル
	ModelMaterial material_;
};