#pragma once

#include <vector>
#include <string>

#include "Enums/Primitive3DEnum.h"
#include "3D/RenderObjects3D/BaseRenderable3D/BaseRenderable3D.h"
#include "3D/RenderObjects3D/PrimitiveRenderer3D/PrimitiveRenderer3D.h"
#include "3D/RenderObjects3D/StaticRenderer3D/StaticRenderer3D.h"
#include "3D/RenderObjects3D/SkinningRenderer3D/SkinningRenderer3D.h"

/// <summary>
/// 3D描画オブジェクトマネージャ
/// </summary>
class Renderer3DManager {
public:
	Renderer3DManager();
	~Renderer3DManager();

	void Update();
	void Draw();

	std::unique_ptr<PrimitiveRenderer3D> CreatePrimitiveRenderer(const std::string& name, Primitive3DType primitiveRenderer, const std::string& textureName);
	std::unique_ptr<StaticRenderer3D> CreateStaticRenderer(const std::string& name, const std::string& modelName);
	std::unique_ptr<SkinningRenderer3D> CreateSkinningRenderer(const std::string& name, const std::string& modelName);

	// マネージャにレンダラーを追加
	void AddRenderer(std::unique_ptr<BaseRenderable3D> newRenderer);

	// 名前からレンダラーを検索
	BaseRenderable3D* Find(const std::string& name);

	// レンダラーをクリア
	void Clear();

	// 削除フラグの立っているレンダラーを削除
	void DeleteGarbages();

	// 生成してある全オブジェクトのリストを取得
	const std::vector<std::unique_ptr<BaseRenderable3D>>& GetRenderers() const;

private:
	// 描画オブジェクトコンテナ
	std::vector<std::unique_ptr<BaseRenderable3D>> renderers_;
};