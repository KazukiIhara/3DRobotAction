#include "GameObject3DGroup.h"

#include "Framework/MAGI.h"

using namespace MAGIMath;

GameObject3DGroup::GameObject3DGroup(const std::string& groupName) {
	name_ = groupName;
	CreateInstancingResource();
	MapInstancingData();
	CreateMaterialResource();
	MapMaterialData();
}

void GameObject3DGroup::AddObject(std::unique_ptr<GameObject3D> newObject) {
	gameObjects_.insert(std::pair(newObject->name, std::move(newObject)));
}

void GameObject3DGroup::AddRenderer(std::unique_ptr<BaseRenderable3D> newRenderer) {
	thisGameObjectRenderer_ = std::move(newRenderer);
}

void GameObject3DGroup::UpdateData() {
	// オブジェクトを更新
	for (auto& gameObject : gameObjects_) {
		// ポインタが有効かつ親がいない場合更新
		if (gameObject.second && !gameObject.second->GetParent()) {
			// オブジェクトの子を更新
			gameObject.second->UpdateHierarchy();
			// ワールド行列の更新書影
			gameObject.second->UpdateWorldTransformHierarchy();
		}
	}
	// Update
	UpdateDrawData();
	// instance
	UpdateInstancingData();
}

void GameObject3DGroup::Draw() {
	// 描画前処理
	PrepareForRendering();
	// 描画
	thisGameObjectRenderer_->DrawInstanced(instanceCount_);
}

void GameObject3DGroup::DeleteGarbage() {

}

std::string& GameObject3DGroup::GetName() {
	return name_;
}

void GameObject3DGroup::PrepareForRendering() {
	// コマンドリストを取得
	ID3D12GraphicsCommandList* commandList = MAGISYSTEM::GetDirectXCommandList();
	// PSOを設定
	commandList->SetPipelineState(MAGISYSTEM::GetGraphicsPipelineState(GraphicsPipelineStateType::Object3DGroup, blendMode_));
	// マテリアルCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	// StructuredBufferのSRVを設定する
	commandList->SetGraphicsRootDescriptorTable(1, MAGISYSTEM::GetSrvUavDescriptorHandleGPU(srvIndex_));
	// カメラを転送
	MAGISYSTEM::TransferCamera3D(cameraRootParamaterIndex_);
	// ライトを転送
	MAGISYSTEM::TransferPunctualLight(lightRootParamaterIndex_);
}

void GameObject3DGroup::CreateInstancingResource() {
	// instancing用のリソースを作る
	instancingResource_ = MAGISYSTEM::CreateBufferResource(sizeof(GameObject3DDataForGPU) * kMaxInstance_);
	// srvのインデックスを割り当て
	srvIndex_ = MAGISYSTEM::SrvUavAllocate();
	// Srvを作成
	MAGISYSTEM::CreateSrvStructuredBuffer(srvIndex_, instancingResource_.Get(), kMaxInstance_, sizeof(GameObject3DDataForGPU));
}

void GameObject3DGroup::MapInstancingData() {
	instancingData_ = nullptr;
	instancingResource_->Map(0, nullptr, reinterpret_cast<void**>(&instancingData_));
}

void GameObject3DGroup::UpdateInstancingData() {
	// 描画すべきインスタンス数
	instanceCount_ = 0;

	for (std::list<GameObject3DData>::iterator objectIterator = gameObjectsData_.begin();
		objectIterator != gameObjectsData_.end();) {

		if (instanceCount_ < kMaxInstance_) {
			// Wマトリックスを求める
			// Scale
			Matrix4x4 scaleMatrix = MakeScaleMatrix((*objectIterator).transform.scale);
			// 
			Matrix4x4 rotateMatrix = MakeRotateXYZMatrix((*objectIterator).transform.rotate);

			// translate
			Matrix4x4 translateMatrix = MakeTranslateMatrix((*objectIterator).transform.translate);

			// ワールド行列を作成
			Matrix4x4 worldMatrix = scaleMatrix * rotateMatrix * translateMatrix;

			// ワールド行列
			instancingData_[instanceCount_].worldMatrix = worldMatrix;
			instancingData_[instanceCount_].worldInverseTranspose = MakeInverseTransposeMatrix(worldMatrix);

			// 描画するオブジェクトの数を1つカウントする
			instanceCount_++;
		}
		// 次のイテレーターに進める
		++objectIterator;
	}

}

void GameObject3DGroup::UpdateDrawData() {
	// 描画データをクリア
	gameObjectsData_.clear();

	for (auto& gameobject : gameObjects_) {
		GameObject3DData newObjectData{};
		newObjectData.transform.scale = gameobject.second->GetScale();
		newObjectData.transform.rotate = gameobject.second->GetRotate();
		newObjectData.transform.translate = gameobject.second->GetTranslate();

		gameObjectsData_.push_back(newObjectData);
	}
}

void GameObject3DGroup::CreateMaterialResource() {
	// マテリアル用のリソース作成
	materialResource_ = MAGISYSTEM::CreateBufferResource(sizeof(Material3DForGPU));
}

void GameObject3DGroup::MapMaterialData() {
	materialData_ = nullptr;
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color = material_.color;
	materialData_->enableSpecularRef = material_.enableSpecularRef;
	materialData_->enableLighting = material_.enableLighting;
	materialData_->shininess = material_.shininess;
	materialData_->uvTransformMatrix = MakeUVMatrix(material_.uvTransform.scale, material_.uvTransform.rotateZ, material_.uvTransform.translate);
}

void GameObject3DGroup::UpdateMaterialData() {
	materialData_->color = material_.color;
	materialData_->enableSpecularRef = material_.enableSpecularRef;
	materialData_->enableLighting = material_.enableLighting;
	materialData_->shininess = material_.shininess;
	materialData_->uvTransformMatrix = MakeUVMatrix(material_.uvTransform.scale, material_.uvTransform.rotateZ, material_.uvTransform.translate);
}
