#include "PrimitiveParticleGroup3D.h"

#include "3D/Shapes3D/PrimitiveShapes3D/Plane/Plane.h"
#include "3D/Shapes3D/PrimitiveShapes3D/Sphere/Sphere.h"

#include "Framework/MAGI.h"

PrimitiveParticleGroup3D::PrimitiveParticleGroup3D(const std::string& particleGroupName, const Primitive3DType& primitiveType, const std::string& textureName)
	:BaseParticleGroup3D(particleGroupName) {
	// 描画タイプを設定
	rendererType_ = Renderer3DType::Primitive;
	// シンプル形状のタイプを設定
	primitiveType_ = primitiveType;

	// 貼り付けるテクスチャの名前を設定
	textureName_ = textureName;
	// テクスチャが未設定の場合、デフォルトのテクスチャを割り当てる(uvCheckerはエンジン用のリソースのためフルパス指定)
	if (textureName_ == "") {
		textureName_ = "EngineAssets/Images/uvChecker.png";
	}
}

void PrimitiveParticleGroup3D::AssignShape() {
	switch (primitiveType_.value()) {
		case Primitive3DType::Plane:
			primitive_ = std::make_unique<Plane>();
			break;
		case Primitive3DType::Sphere:
			primitive_ = std::make_unique<Sphere>();
			break;
		case Primitive3DType::Ring:
			break;
		case Primitive3DType::Cylinder:
			break;
	}
}

void PrimitiveParticleGroup3D::Update() {
	// Primitive更新
	primitive_->Update();
	// 基底クラスの更新
	BaseParticleGroup3D::Update();
}

void PrimitiveParticleGroup3D::Draw() {
	// パーティクルグループの描画前設定
	PrepareForRendering();

	// Texture用のSRVをセット
	uint32_t textureSrvIndex = MAGISYSTEM::GetTexture()[textureName_].srvIndex;
	MAGISYSTEM::GetDirectXCommandList()->SetGraphicsRootDescriptorTable(2, MAGISYSTEM::GetSrvUavDescriptorHandleGPU(textureSrvIndex));

	// 描画
	primitive_->DrawInstanced(instanceCount_);
}

std::string& PrimitiveParticleGroup3D::GetTextureName() {
	return textureName_;
}
