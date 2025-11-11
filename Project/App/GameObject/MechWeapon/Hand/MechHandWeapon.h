#pragma once

// C++
#include <cstdint>
#include <string>
#include <memory>

// MyHedder
#include "Math/Types/AllMathTypes.h"

#include "Structs/Primitive3DStruct.h"
#include "Structs/ModelStruct.h"

// 前方宣言
class Transform3D;
class MechCore;
class AttackObjectManager;


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
		RocketLauncher,
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
		// 攻撃間隔(硬直付きの武器は硬直)
		float coolTime = 0.0f;
		// 装弾数
		int ammoCapacity = 0;
		// 攻撃が飛ぶスピード
		float speed = 0.0f;

		// 
		// 内部パラメータ
		// 

		// モデル名
		std::string modelName = "AssultRifle";
		// 攻撃発射ローカル座標
		Vector3 fireOffsetLocalPos;
	};

	// 
	// 内部処理用データ
	// 
	struct Data {
		// 攻撃発射位置行列
		Matrix4x4 fireOffsetLocalMatrix;

		// 攻撃発射ワールド座標
		Vector3 fireOffsetWorldPos;
		// 攻撃発射ワールド行列
		Matrix4x4 fireOffsetWorldMatrix;

		// 残弾数
		int ammo_ = 0;

		// クールタイム用タイマー
		float coolTimer_ = 0.0f;
		// リロード用タイマー
		float reloadTimer_ = 0.0f;

		// クールタイム中かどうか
		bool isCoolTime_ = false;
		// リロード中かどうか
		bool isReload_ = false;
	};

	// enumから文字列に変換する
	static std::string ComvertTypeToString(const MechHandWeapon::Type& type);
	// 文字列からenumに変換する
	static MechHandWeapon::Type ComvertStringToEnum(const std::string& typeString);

public:
	MechHandWeapon(const MechHandWeapon::Param& param, AttackObjectManager* attackObjectManager);
	~MechHandWeapon() = default;

	void Update();
	void Draw();

	// 攻撃
	void Attack(MechCore* mechCore);

	// リロード
	void Reload();

	// パラメータセット
	void SetParam(const MechHandWeapon::Param& param);

	// パラメータ取得
	MechHandWeapon::Param& GetParam();
	// データ取得
	MechHandWeapon::Data GetData()const;

	// トランスフォーム取得
	Transform3D* GetTransform();

private:
	// 弾の発射座標を計算
	void CulFirePos();
	// 前方ベクトルを計算
	void CulForward();
	// クールタイム更新
	void UpdateCoolTime();
	// リロード更新
	void UpdateReload();

private:
	// 武器パラメータ
	Param param_;

	// 内部処理用データ
	Data data_;

	// トランスフォーム
	Transform3D* transform_;

	// 前方ベクトル
	Vector3 forward_{ 0.0f,0.0f,1.0f };

	// マテリアル
	ModelMaterial material_;

	// 
	// デバッグ描画
	// 

	// 手持ち武器の発射位置マテリアル
	SphereData3D firePosSphereData_{};
	MaterialData3D firePosMaterial_{};

private:
	// 攻撃オブジェクトマネージャ
	AttackObjectManager* attackObjectManager_ = nullptr;

};