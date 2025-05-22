#pragma once

#include <optional>
#include <memory>

#include "Enums/Primitive3DEnum.h"

#include "3D/ParticleGroups3D/BaseParticleGroup3D/BaseParticleGroup3D.h"
#include "3D/Shapes3D/PrimitiveShapes3D/BasePrimitiveShape3D/BasePrimitiveShape3D.h"

/// <summary>
///	シンプル形状のパーティクルグループクラス
/// </summary>
class PrimitiveParticleGroup3D: public BaseParticleGroup3D {
public:
	PrimitiveParticleGroup3D(const std::string& particleGroupName, const Primitive3DType& primitiveType, const std::string& textureName);
	~PrimitiveParticleGroup3D()override = default;

	void AssignShape()override;
	void Update()override;
	void Draw()override;

	// テクスチャを取得
	std::string& GetTextureName();

private:
	// プリミティブ形状タイプ
	std::optional<Primitive3DType> primitiveType_ = std::nullopt;
	// 貼り付けるテクスチャファイル
	std::string textureName_ = "";
	// 形状
	std::unique_ptr<BasePrimitiveShape3D> primitive_ = nullptr;
};