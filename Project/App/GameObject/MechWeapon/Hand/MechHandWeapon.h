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
public:
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

		// 
		// ゲーム内で表示されるパラメータ
		// 

		// 武器の名前
		std::string name = "Weapon";
		// 武器タイプ
		Type type = Type::Gun;
		// ダメージ
		int damage = 0;
		// リロード時間
		float reloadTime = 0.0f;
		// 攻撃間隔
		float coolTime = 0.0f;
		// 装弾数
		int capacity = 0;
		// 攻撃が飛ぶスピード
		float speed = 0.0f;

		// 
		// 内部パラメータ
		// 

		// モデル名
		std::string modelName = "AssultRifle";
		// 攻撃発射ローカル座標
		Vector3 fireOffsetLocalPos;

		//
		// 内部処理用パラメータ
		//


		// 攻撃発射位置行列
		Matrix4x4 fireOffsetLocalMatrix;

		// 攻撃発射ワールド座標
		Vector3 fireOffsetWorldPos;
		// 攻撃発射ワールド行列
		Matrix4x4 fireOffsetWorldMatrix;
	};

	// enumから文字列に変換する
	static std::string ComvertTypeToString(const MechHandWeapon::Type& type);
	// 文字列からenumに変換する
	static MechHandWeapon::Type ComvertStringToEnum(const std::string& typeString);

public:
	MechHandWeapon(const MechHandWeapon::Param& param);
	~MechHandWeapon() = default;

	void Update();
	void Draw();

	// 攻撃
	void Attack();

	// パラメータ取得
	MechHandWeapon::Param& GetParam();

	// トランスフォーム取得
	Transform3D* GetTransform();

private:
	// 弾の発射座標を計算
	void CulFirePos();

private:
	// 武器パラメータ
	Param param_;

	// トランスフォーム
	Transform3D* transform_;

	// マテリアル
	ModelMaterial material_;
};