#pragma once

// 数学ヘッダ
#include "Math/Utility/MathUtility.h"

/// <summary>
/// 3D空間トランスフォーム
/// </summary>
class Transform3D {
public:
	Transform3D(
		const Vector3& scale,
		const Vector3& rotate,
		const Vector3& translate
	);

	Transform3D(
		const Vector3& scale,
		const Quaternion& rotate,
		const Vector3& translate
	);

	Transform3D(
		const Vector3& translate = { 0.0f,0.0f,0.0f }
	);

	~Transform3D() = default;

	void Initialize(
		const Vector3& scale,
		const Vector3& rotate,
		const Vector3& translate
	);

	void Initialize(
		const Vector3& scale,
		const Quaternion& rotate,
		const Vector3& translate
	);

	void Initialize(
		const Vector3& translate
	);

	void Update();

	void Finalize();

	//
	// セッター
	//

	// 親をセット
	void SetParent(Transform3D* parent, bool keepWorld = true);

	// 親子付けを解除
	void RemoveParent(bool keepWorld = true);

	void SetIsChange(bool isChange);
	void SetIsAlive(bool isAlive);

	void SetScale(const Vector3& scale);
	void SetRotate(const Vector3& rotate);
	void SetTranslate(const Vector3& translate);
	void SetTranslateX(float x);
	void SetTranslateY(float y);
	void SetTranslateZ(float z);
	void SetQuaternion(const Quaternion& rotate);

	void AddScale(const Vector3& scale);
	void AddRotate(const Vector3& rotate);
	void AddTranslate(const Vector3& translate);
	void AddQuaterion(const Quaternion& deltaQ);

	//
	// ゲッター
	//

	[[nodiscard]] Transform3D* GetParent()const;

	[[nodiscard]] const bool& GetIsChanged()const;
	[[nodiscard]] const bool& GetisAlive()const;

	[[nodiscard]] const Vector3& GetScale()const;
	[[nodiscard]] const Vector3& GetRotate()const;
	[[nodiscard]] const Vector3& GetTranslate()const;
	[[nodiscard]] const Quaternion& GetQuaternion()const;

	[[nodiscard]] const Matrix4x4& GetWorldMatrix()const;
	[[nodiscard]] const Vector3& GetWorldPosition()const;

private:
	//---------------------------- 
	// トランスフォーム
	//----------------------------
	Vector3 scale_ = { 1.0f,1.0f,1.0f };
	Quaternion rotate_ = { 0.0f,0.0f,0.0f,1.0f };
	Vector3 translate_ = { 0.0f,0.0f,0.0f };

	Vector3 inputRadians_ = { 0.0f,0.0f,0.0f };

	// ワールド座標
	Vector3 worldPosition_ = { 0.0f,0.0f,0.0f };

	// ワールド行列
	Matrix4x4 worldMatrix_;

	//---------------------------- 
	// 1フレーム前の値保存用変数
	//----------------------------
	Quaternion preRotate_ = { 0.0f,0.0f,0.0f,1.0f };
	Vector3 preInputRadians_ = { 0.0f,0.0f,0.0f };

	// 親
	Transform3D* parent_ = nullptr;
	// 子
	std::vector<Transform3D*> children_;

	// 変更フラグ
	bool isChanged_ = false;

	// 生存フラグ
	bool isAlive_ = true;

private:
	// コピー禁止
	Transform3D(const Transform3D&) = delete;
	Transform3D& operator=(const Transform3D&) = delete;
};