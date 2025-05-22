#pragma once

#include "3D/RenderObjects3D/BaseRenderable3D/BaseRenderable3D.h"
#include "3D/Shapes3D/Model/Model.h"

/// <summary>
/// 3Dオブジェクト(インスタンスの生成後初期化メソッドを呼んでください)
/// </summary>
class StaticRenderer3D :public BaseRenderable3D {
public:
	StaticRenderer3D(const std::string& objectName, const std::string& modelName);
	virtual ~StaticRenderer3D()override;
	virtual void AssignShape()override;
	virtual void Update()override;
	void Draw()override;
	void DrawInstanced(uint32_t instanceCount)override;

	std::string GetModelName()const;
protected:
	// 受け取るモデルのデータ
	ModelData modelData_{};
	// モデル
	std::unique_ptr<Model> model_ = nullptr;
};