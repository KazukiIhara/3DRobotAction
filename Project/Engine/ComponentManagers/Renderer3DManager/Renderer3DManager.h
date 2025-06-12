#pragma once

// C++
#include <vector>
#include <memory>

// MyHedder
#include "Renderer3D/ModelRenderer/ModelRenderer.h"

/// <summary>
/// 3D描画オブジェクト管理クラス
/// </summary>
class Renderer3DManager {
public:
	Renderer3DManager();
	~Renderer3DManager();

	[[nodiscard]] std::weak_ptr<ModelRenderer> Add(std::shared_ptr<ModelRenderer> modelRenderer);
	void Draw();
	void DeleteGarbage();
	void Clear();

private:
	// モデル描画オブジェクトコンテナ
	std::vector<std::shared_ptr<ModelRenderer>> modelRenderers_;

};