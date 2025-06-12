#include "Model.h"

#include "Framework/MAGI.h"

#include "MAGIUitility/MAGIUtility.h"

using namespace MAGIMath;
using namespace MAGIUtility;

Model::Model(const ModelData& modeldata) {
	modelData_ = modeldata;
}

Model::~Model() {
	for (auto& mesh : meshes_) {
		mesh.reset();
	}
}

void Model::Initialize() {
	CreateMeshes();
}

void Model::Update() {
	// 各メッシュの更新
	for (auto& mesh : meshes_) {
		mesh->Update();
	}
}

void Model::Draw() {
	for (auto& mesh : meshes_) {
		mesh->Draw();
	}
}

void Model::DrawInstanced(uint32_t instanceCount) {
	for (auto& mesh : meshes_) {
		mesh->DrawInstanced(instanceCount);
	}
}

void Model::DrawInstancedForParticle(uint32_t instanceCount) {
	for (auto& mesh : meshes_) {
		mesh->DrawInstancedForParticle(instanceCount);
	}
}

void Model::CreateMeshes() {
	for (uint32_t i = 0; i < modelData_.meshes.size(); i++) {
		std::unique_ptr<Mesh> newMesh = std::make_unique<Mesh>(modelData_.meshes[i]);
		newMesh->Initialize();
		meshes_.push_back(std::move(newMesh));
	}
}