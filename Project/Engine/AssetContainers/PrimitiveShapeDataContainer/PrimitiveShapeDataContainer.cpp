#include "PrimitiveShapeDataContainer.h"

using namespace MAGIMath;

PrimitiveShapeDataContainer::PrimitiveShapeDataContainer() {
	Initialize();
}

PrimitiveShapeDataContainer::~PrimitiveShapeDataContainer() {}

PrimitiveData PrimitiveShapeDataContainer::GetPrimitiveShapeData(const Primitive3DType& primitiveType) {
	return primitiveDatas_[primitiveType];
}

void PrimitiveShapeDataContainer::Initialize() {
	CreatePlane();
	CreateSphere();
}

void PrimitiveShapeDataContainer::CreatePlane() {
	PrimitiveData primitiveData{};
	// 頂点データの設定
	primitiveData.vertices.push_back({ .position = { -1.0f,  1.0f, 0.0f, 1.0f }, .texcoord = { 0.0f, 0.0f }, .normal = { 0.0f, 0.0f, -1.0f }, .tangent = { 1.0f, 0.0f, 0.0f } });
	primitiveData.vertices.push_back({ .position = {  1.0f,  1.0f, 0.0f, 1.0f }, .texcoord = { 1.0f, 0.0f }, .normal = { 0.0f, 0.0f, -1.0f }, .tangent = { 1.0f, 0.0f, 0.0f } });
	primitiveData.vertices.push_back({ .position = { -1.0f, -1.0f, 0.0f, 1.0f }, .texcoord = { 0.0f, 1.0f }, .normal = { 0.0f, 0.0f, -1.0f }, .tangent = { 1.0f, 0.0f, 0.0f } });
	primitiveData.vertices.push_back({ .position = {  1.0f, -1.0f, 0.0f, 1.0f }, .texcoord = { 1.0f, 1.0f }, .normal = { 0.0f, 0.0f, -1.0f }, .tangent = { 1.0f, 0.0f, 0.0f } });
	
	// インデックスデータの設定
	primitiveData.indices.push_back(0);
	primitiveData.indices.push_back(1);
	primitiveData.indices.push_back(2);
	primitiveData.indices.push_back(2);
	primitiveData.indices.push_back(1);
	primitiveData.indices.push_back(3);

	primitiveData.color = { 1.0f,1.0f,1.0f,1.0f };
	primitiveData.uvMatrix = MakeIdentityMatrix4x4();

	// コンテナに挿入
	primitiveDatas_.insert(std::make_pair(Primitive3DType::Plane, primitiveData));
}

void PrimitiveShapeDataContainer::CreateSphere() {
	PrimitiveData primitiveData{};
	// 球分割数
	const uint32_t kSubdivision = 32;
	const float kLonEvery = std::numbers::pi_v<float> *2.0f / static_cast<float>(kSubdivision);
	const float kLatEvery = std::numbers::pi_v<float> / static_cast<float>(kSubdivision);

	// 頂点の生成
	for (uint32_t latIndex = 0; latIndex <= kSubdivision; ++latIndex) {
		float lat = -std::numbers::pi_v<float> / 2.0f + kLatEvery * latIndex;
		float sinLat = std::sinf(lat);
		float cosLat = std::cosf(lat);

		for (uint32_t lonIndex = 0; lonIndex <= kSubdivision; ++lonIndex) {
			float lon = kLonEvery * lonIndex;
			float sinLon = std::sinf(lon);
			float cosLon = std::cosf(lon);

			VertexData3D vertex{};
			vertex.position = { cosLat * cosLon, sinLat, cosLat * sinLon, 1.0f };
			vertex.normal = { cosLat * cosLon, sinLat, cosLat * sinLon };
			vertex.texcoord = { static_cast<float>(lonIndex) / static_cast<float>(kSubdivision),
								1.0f - static_cast<float>(latIndex) / static_cast<float>(kSubdivision) };
			vertex.tangent = { -sinLon, 0.0f, cosLon };

			primitiveData.vertices.push_back(vertex);
		}
	}

	// インデックスの生成
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			uint32_t first = latIndex * (kSubdivision + 1) + lonIndex;
			uint32_t second = first + kSubdivision + 1;

			// 三角形1
			primitiveData.indices.push_back(first);
			primitiveData.indices.push_back(second);
			primitiveData.indices.push_back(first + 1);

			// 三角形2
			primitiveData.indices.push_back(second);
			primitiveData.indices.push_back(second + 1);
			primitiveData.indices.push_back(first + 1);
		}
	}

	primitiveData.color = { 1.0f, 1.0f, 1.0f, 1.0f };
	primitiveData.uvMatrix = MakeIdentityMatrix4x4();

	// コンテナに挿入
	primitiveDatas_.insert(std::make_pair(Primitive3DType::Sphere, primitiveData));
}
