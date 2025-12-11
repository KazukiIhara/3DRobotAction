#pragma once

// C++
#include <string>
#include <memory>

// MyHedder
#include "Structs/ModelStruct.h"

// 前方宣言
class Transform3D;

/// <summary>
/// モデル描画オブジェクト
/// </summary>
class ModelRenderer {
public:
	ModelRenderer(const std::string& name, const std::string& modelName, const ModelMaterial& material = ModelMaterial{});
	~ModelRenderer();

	void Draw();

	void Finalize();

	void SetIsAlive(bool isAlive);

	void SetMaterial(const ModelMaterial& material);
	void SetIsRender(bool isRender);

	[[nodiscard]] const std::string& GetName()const;
	[[nodiscard]] bool GetIsAlive()const;
	[[nodiscard]] bool GetIsRender()const;
	[[nodiscard]] Transform3D* GetTransform();

private:
	// 名前
	std::string name_ = "";
	// モデルの名前
	std::string modelName_ = "";
	// マテリアル
	ModelMaterial material_{};
	// トランスフォーム
	Transform3D* transform_;
	// 描画フラグ
	bool isRender_ = true;
	// 生存フラグ
	bool isAlive_ = true;
};