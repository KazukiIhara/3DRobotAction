#include "Object2D.h"

#include "Framework/MAGI.h"

#include "DirectXTex/DirectXTex.h"

using namespace MAGIMath;

Object2D::Object2D(const std::string& objectName, const std::string& textureName) {
	Initialize(objectName, textureName);
}

Object2D::~Object2D() {

}

void Object2D::Initialize(const std::string& objectName, const std::string& textureName) {
	// 名前をセット
	name_ = objectName;
	// テクスチャの名前をセット
	textureName_ = textureName;
	// トランスフォームを初期化
	worldTransform_.Update();
	// マテリアルを初期化
	material_.color = { 1.0f,1.0f,1.0f,1.0f };
	material_.uvTransformMatrix = MakeIdentityMatrix4x4();
	// テクスチャ左上座標
	textureLeftTop_ = { 0.0f,0.0f };

	// テクスチャをロード
	MAGISYSTEM::LoadTexture(textureName);

	// 頂点リソースの作成
	CreateVertexResource();
	// 頂点バッファビューの作成
	CreateVretexBufferView();
	// 頂点データの書き込み
	MapVertexData();

	// 描画用のインデックスリソースを作成
	CreateIndexResource();
	// インデックスバッファビューの作成
	CreateIndexBufferView();
	// インデックスリソースにデータを書き込む
	MapIndexResource();

	// マテリアル用のリソース作成
	CreateMaterialResource();
	// マテリアルにデータを書き込む
	MapMaterialData();

	// wvp用のリソース作成
	CreateWVPResource();
	// データを書き込む
	MapWVPData();

	// テクスチャサイズを設定
	AdjustTextureSize();

}

void Object2D::Update() {
	// アンカーポイントの設定
	float left = 0.0f - anchorPoint_.x;
	float right = 01.0f - anchorPoint_.x;
	float top = 0.0f - anchorPoint_.y;
	float bottom = 1.0f - anchorPoint_.y;

	// 左右反転
	if (isFlipX_) {
		left = -left;
		right = -right;
	}
	// 上下反転
	if (isFlipY_) {
		top = -top;
		bottom = -bottom;
	}

	// データを書き込む
	vertexData_[0].position = { left,bottom,0.0f,1.0f };//左下
	vertexData_[1].position = { left,top,0.0f,1.0f };//左上
	vertexData_[2].position = { right,bottom,0.0f,1.0f };//右下
	vertexData_[3].position = { right,top,0.0f,1.0f };//右上

	// メタデータを取得
	const DirectX::TexMetadata& metaData =
		MAGISYSTEM::GetTextureMetaData(textureName_);
	float texLeft = textureLeftTop_.x / metaData.width;
	float texRight = (textureLeftTop_.x + cutOutSize_.x) / metaData.width;
	float texTop = textureLeftTop_.y / metaData.height;
	float texBottom = (textureLeftTop_.y + cutOutSize_.y) / metaData.height;

	// データを書き込む
	vertexData_[0].texcoord = { texLeft,texBottom };
	vertexData_[1].texcoord = { texLeft,texTop };
	vertexData_[2].texcoord = { texRight,texBottom };
	vertexData_[3].texcoord = { texRight,texTop };

	// ワールド行列の更新
	worldTransform_.Update();
	Matrix4x4 projectionMatrix = MakeOrthographicMatrix(0.0f, 0.0f, float(WindowApp::kClientWidth), float(WindowApp::kClientHeight), 0.0f, 100.0f);
	*wvpData_ = worldTransform_.worldMatrix * projectionMatrix;

	// マテリアルデータの更新
	material_.uvTransformMatrix = MakeUVMatrix(uvTransform_.scale, uvTransform_.rotateZ, uvTransform_.translate);
	materialData_->uvTransformMatrix = material_.uvTransformMatrix;
	materialData_->color = material_.color;

}

void Object2D::Draw() {
	// コマンドリストを取得
	ID3D12GraphicsCommandList* commandList = MAGISYSTEM::GetDirectXCommandList();
	// PSOを設定
	commandList->SetPipelineState(MAGISYSTEM::GetGraphicsPipelineState(GraphicsPipelineStateType::Object2D, blendMode_));
	// VBVの設定
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);
	// IBVの設定
	commandList->IASetIndexBuffer(&indexBufferView_);
	// マテリアルCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	// wvp用のCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(1, transformationResource_->GetGPUVirtualAddress());
	// テクスチャのSRVを設定
	uint32_t textureSrv = MAGISYSTEM::GetTexture()[textureName_].srvIndex;
	commandList->SetGraphicsRootDescriptorTable(2, MAGISYSTEM::GetSrvUavDescriptorHandleGPU(textureSrv));
	// 描画！(DrawCall/ドローコール)
	commandList->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

void Object2D::SetTextureName(const std::string& textureName) {
	textureName_ = textureName;
}

Vector2& Object2D::GetPosition() {
	return worldTransform_.position;
}

Vector2& Object2D::GetSize() {
	return worldTransform_.size;
}

Vector2& Object2D::GetAnchorPoint() {
	return anchorPoint_;
}

float& Object2D::GetRotate() {
	return worldTransform_.rotateZ;
}

float& Object2D::GetDepth() {
	return worldTransform_.depth;
}

Vector2& Object2D::GetUVTranslate() {
	return uvTransform_.translate;
}

Vector2& Object2D::GetUVScale() {
	return uvTransform_.scale;
}

float& Object2D::GetUVRotate() {
	return uvTransform_.rotateZ;
}

bool& Object2D::GetIsAlive() {
	return isAlive_;
}

bool& Object2D::GetIsShow() {
	return isShow_;
}

Material2D& Object2D::GetMaterial() {
	return material_;
}

void Object2D::CreateVertexResource() {
	vertexResource_ = MAGISYSTEM::CreateBufferResource(sizeof(VertexData2D) * 6);
}

void Object2D::CreateVretexBufferView() {
	//頂点バッファビューを作成する
	//リソースの先頭アドレスから使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点4つ分のサイズ
	vertexBufferView_.SizeInBytes = sizeof(VertexData2D) * 4;
	//1頂点あたりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData2D);
}

void Object2D::MapVertexData() {
	//頂点リソースにデータを書き込む
	vertexData_ = nullptr;

	//書き込むためのアドレスを取得
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

	//1枚目の三角形
	vertexData_[0].position = { 0.0f,1.0f,0.0f,1.0f };//左下
	vertexData_[0].texcoord = { 0.0f,1.0f };
	vertexData_[1].position = { 0.0f,0.0f,0.0f,1.0f };//左上
	vertexData_[1].texcoord = { 0.0f,0.0f };
	vertexData_[2].position = { 1.0f,1.0f,0.0f,1.0f };//右下
	vertexData_[2].texcoord = { 1.0f,1.0f };
	vertexData_[3].position = { 1.0f,0.0f,0.0f,1.0f };//右上
	vertexData_[3].texcoord = { 1.0f,0.0f };
}

void Object2D::CreateIndexResource() {
	indexResource_ = MAGISYSTEM::CreateBufferResource(sizeof(uint32_t) * 6);
}

void Object2D::CreateIndexBufferView() {
	// リソースの先頭アドレスから使う
	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
	// 使用するリソースのサイズはインデックス6つ分
	indexBufferView_.SizeInBytes = sizeof(uint32_t) * 6;
	// インデックスはuint32_tとする
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;
}

void Object2D::MapIndexResource() {
	// インデックスリソースにデータを書き込む
	indexData_ = nullptr;
	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));
	indexData_[0] = 0;	indexData_[1] = 1;	indexData_[2] = 2;
	indexData_[3] = 1;	indexData_[4] = 3;	indexData_[5] = 2;
}

void Object2D::CreateMaterialResource() {
	// マテリアル用のリソースを作る
	materialResource_ = MAGISYSTEM::CreateBufferResource(sizeof(Material2D));
}

void Object2D::MapMaterialData() {
	materialData_ = nullptr;
	// 書き込むためのアドレスを取得
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	// マテリアルデータを書き込む
	materialData_->color = material_.color;
	materialData_->uvTransformMatrix = material_.uvTransformMatrix;
}

void Object2D::CreateWVPResource() {
	// WVP用のリソースを作る
	transformationResource_ = MAGISYSTEM::CreateBufferResource(sizeof(Matrix4x4));
}

void Object2D::MapWVPData() {
	// データを書き込む
	wvpData_ = nullptr;
	// データを書き込むためのアドレスを取得
	transformationResource_->Map(0, nullptr, reinterpret_cast<void**>(&wvpData_));
	// 単位行列を書き込んでおく
	*wvpData_ = MakeIdentityMatrix4x4();
}

void Object2D::AdjustTextureSize() {
	// テクスチャデータ取得
	const DirectX::TexMetadata& metaData = MAGISYSTEM::GetTextureMetaData(textureName_);

	cutOutSize_.x = static_cast<float>(metaData.width);
	cutOutSize_.y = static_cast<float>(metaData.height);
	// 画面サイズをテクスチャサイズに合わせる
	worldTransform_.size = cutOutSize_;
}
