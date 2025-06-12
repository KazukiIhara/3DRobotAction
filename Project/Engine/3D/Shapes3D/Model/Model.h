#pragma once

// C++
#include <string>
#include <vector>
#include <memory>

#include "3D/Shapes3D/Mesh/Mesh.h"
#include "3D/Skeleton/Skeleton.h"
#include "Structs/AnimationStruct.h"

class Model {
public:
	Model(const ModelData& modeldata);
	virtual ~Model();

	virtual void Initialize();
	virtual void Update();
	void Draw();
	void DrawInstanced(uint32_t instanceCount);
	void DrawInstancedForParticle(uint32_t instanceCount);

private:
	// 受け取ったモデルからメッシュを作成
	virtual void CreateMeshes();

protected:
	// 受け取るモデルデータ
	ModelData modelData_{};
	// メッシュ
	std::vector<std::unique_ptr<Mesh>> meshes_;
};