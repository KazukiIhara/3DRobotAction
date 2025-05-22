#pragma once

#include <optional>
#include <memory>

#include "3D/ParticleGroups3D/BaseParticleGroup3D/BaseParticleGroup3D.h"

#include "3D/Shapes3D/Model/Model.h"

class StaticParticleGroup3D: public BaseParticleGroup3D {
public:
	StaticParticleGroup3D(const std::string& particleGroupName, const std::string& modelName);
	~StaticParticleGroup3D()override = default;

	void AssignShape()override;
	void Update()override;
	void Draw()override;

private:
	// 受け取るモデルのデータ
	ModelData modelData_{};
	// 形状
	std::unique_ptr<Model> model_ = nullptr;
};