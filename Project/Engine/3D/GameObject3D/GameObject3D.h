#pragma once

// C++
#include <string>
#include <unordered_map>
#include <memory>

#include "Math/Types/Vector3.h"
#include "Math/Types/Quaternion.h"

// 前方宣言
class Transform3D;
class ModelRenderer;
class Camera3D;

/// <summary>
/// シーン上のオブジェクト
/// </summary>
class GameObject3D {
public:
	GameObject3D(const std::string& name, const Vector3& scale, const Vector3& rotate, const Vector3& translate);
	GameObject3D(const std::string& name, const Vector3& scale, const Quaternion& rotate, const Vector3& translate);
	GameObject3D(const std::string& name, const Vector3& translate = Vector3(0.0f, 0.0f, 0.0f));

	virtual ~GameObject3D() = default;
	virtual void Update();

	void Finalize();

	//=======================
	// コンポーネント追加関数
	//=======================

	void AddModelRenderer(std::shared_ptr<ModelRenderer> modelRenderer);
	void AddCamera3D(std::shared_ptr<Camera3D> camera3D);


	void SetIsAlive(bool isAlive);
	void SetIsActive(bool isActive);

	[[nodiscard]] const std::string& GetName()const;

	[[nodiscard]] bool GetIsAlive()const;
	[[nodiscard]] bool GetIsActive()const;

	[[nodiscard]] Transform3D* GetTransform();
	[[nodiscard]] std::weak_ptr<ModelRenderer> GetModelRenderer(const std::string& rendererName);
	[[nodiscard]] std::weak_ptr<Camera3D> GetCamera3D(const std::string& camera3DName);

private:
	// 名前
	std::string name_ = "";
	// 生存フラグ
	bool isAlive_ = true;
	// 有効フラグ
	bool isActive_ = true;

	//=======================
	// コンポーネント
	//=======================

	// トランスフォーム
	Transform3D* transformComponent_;
	// モデル描画コンポーネント
	std::unordered_map<std::string, std::weak_ptr<ModelRenderer>> modelRendererComponents_;
	// カメラコンポーネント
	std::unordered_map<std::string, std::weak_ptr<Camera3D>> camera3DComponents_;
};