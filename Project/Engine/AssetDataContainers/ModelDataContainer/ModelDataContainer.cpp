#include "ModelDataContainer.h"

#include <iostream>
#include <cassert>
#include <filesystem>

#include "Logger/Logger.h"

#include "TextureDataContainer/TextureDataContainer.h"

using namespace MAGIMath;

ModelDataContainer::ModelDataContainer(TextureDataContainer* textureDataContainer) {
	Initialize(textureDataContainer);
	Logger::Log("ModelDataContainer Initialize\n");
}

ModelDataContainer::~ModelDataContainer() {
	modelDatas_.clear();
	Logger::Log("ModelDataContainer Finalize\n");
}

void ModelDataContainer::Initialize(TextureDataContainer* textureDataContainer) {
	SetTextureDataContainer(textureDataContainer);
	// コンテナをクリア
	modelDatas_.clear();
}

void ModelDataContainer::Load(const std::string& modelName) {
	// 読み込み済みモデルを検索
	if (modelDatas_.contains(modelName)) {
		// 読み込み済みなら早期リターン
		return;
	}
	// モデルを読み込みコンテナに挿入
	modelDatas_.insert(std::make_pair(modelName, LoadModel(modelName)));
}

ModelData ModelDataContainer::FindModelData(const std::string& modelName) const {
	// 読み込み済みモデルを検索
	if (modelDatas_.contains(modelName)) {
		// 読み込みモデルを戻り値としてreturn
		return modelDatas_.at(modelName);
	}
	// 見つからなかった場合止める

	assert(false && "Warning: Not Found Model!!");
	return ModelData{};
}

ModelData ModelDataContainer::LoadModel(const std::string& modelName) {
	// 対応する拡張子のリスト
	std::vector<std::string> supportedExtensions = { ".obj", ".gltf" };

	// ディレクトリ内のファイルを検索
	std::string directoryPath = "Assets/Models";
	// モデルファイルが入っているディレクトリ
	std::string fileDirectoryPath = directoryPath + "/" + modelName;
	// filesystem用
	std::filesystem::path modelDirectoryPath(fileDirectoryPath);

	std::string modelFilePath;

	for (const auto& entry : std::filesystem::directory_iterator(modelDirectoryPath)) {
		if (entry.is_regular_file()) {
			std::string ext = entry.path().extension().string();
			// 対応する拡張子かチェック
			if (std::find(supportedExtensions.begin(), supportedExtensions.end(), ext) != supportedExtensions.end()) {
				if (entry.path().stem().string() == modelName) {
					modelFilePath = entry.path().string();
					break;
				}
			}
		}
	}

	// ファイルが見つからなかった場合はエラー
	if (modelFilePath.empty()) {
		std::cerr << "Error: Model file not found or unsupported format." << std::endl;
	}

	// 今回追加するモデルのデータ
	ModelData newModelData{};

	// 名前を設定
	newModelData.name = modelName;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(
		modelFilePath.c_str(),
		aiProcess_FlipWindingOrder |
		aiProcess_FlipUVs |
		aiProcess_Triangulate |
		aiProcess_CalcTangentSpace |
		aiProcess_JoinIdenticalVertices |  // 重複頂点のマージ（ボーンの影響を統一）
		aiProcess_LimitBoneWeights |       // 最大4ボーンに制限
		aiProcess_PopulateArmatureData |   // アーマチュアデータを整理（Assimp 5.3 以降）
		aiProcess_GenSmoothNormals
	);
	assert(scene && scene->HasMeshes());

	// ノード読み込み
	newModelData.rootNode = ReadNode(scene->mRootNode);

	std::vector<MaterialData> materials(scene->mNumMaterials);

	for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; ++materialIndex) {
		aiMaterial* material = scene->mMaterials[materialIndex];
		MaterialData materialData;

		// Diffuseテクスチャがある場合
		if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {
			// Diffuse テクスチャのパスを取得
			aiString textureFilePath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);
			std::string diffuseTexName = textureFilePath.C_Str();

			// 通常の Diffuse テクスチャ読み込み
			materialData.textureFilePath = fileDirectoryPath + "/" + diffuseTexName;
			textureDataContainer_->Load(materialData.textureFilePath);

			// normalMapのテクスチャが割り当てられている場合
			aiString normalMapPath;
			if (material->GetTexture(aiTextureType_NORMALS, 0, &normalMapPath) == AI_SUCCESS) {
				// アセットにNormal Mapが設定されているので、こちらを使う
				materialData.normalMapTextureFilePath = fileDirectoryPath + "/" + normalMapPath.C_Str();
				textureDataContainer_->LoadNormalMap(materialData.normalMapTextureFilePath);
			} else {

			}

			// UVスケール情報の取得
			aiUVTransform uvTransform;
			if (material->Get(AI_MATKEY_UVTRANSFORM(aiTextureType_DIFFUSE, 0), uvTransform) == AI_SUCCESS) {
				Vector2 scale = { uvTransform.mScaling.x, uvTransform.mScaling.y };
				float rotateZ = uvTransform.mRotation; // Z軸回転
				Vector2 translate = { uvTransform.mTranslation.x, uvTransform.mTranslation.y };

				// UVマトリックスを構築
				materialData.uvMatrix = MakeUVMatrix(scale, rotateZ, translate);
			} else {
				// UV変換が見つからない場合は単位行列を設定
				materialData.uvMatrix = MakeIdentityMatrix4x4();
			}
		}

		aiColor4D baseColor;
		// PBRマテリアルの BaseColorFactor の取得
		if (material->Get(AI_MATKEY_BASE_COLOR, baseColor) == AI_SUCCESS) {
			// BaseColorが存在する場合
			materialData.color = { baseColor.r, baseColor.g, baseColor.b, baseColor.a };
		} else if (material->Get(AI_MATKEY_COLOR_DIFFUSE, baseColor) == AI_SUCCESS) {
			// Fallback: DiffuseColorの取得
			materialData.color = { baseColor.r, baseColor.g, baseColor.b, baseColor.a };
		} else {
			// デフォルト色（白）
			materialData.color = { 1.0f, 1.0f, 1.0f, 1.0f };
		}

		materials[materialIndex] = materialData;
	}

	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		aiMesh* mesh = scene->mMeshes[meshIndex];
		assert(mesh->HasNormals());

		MeshData meshData;
		meshData.material = materials[mesh->mMaterialIndex];


		if (mesh->HasTextureCoords(0)) { // UVあり

			// 最初に頂点数分のメモリを確保
			meshData.vertices.resize(mesh->mNumVertices);

			// 頂点解析
			for (uint32_t vertexIndex = 0; vertexIndex < mesh->mNumVertices; ++vertexIndex) {
				aiVector3D& position = mesh->mVertices[vertexIndex];
				aiVector3D& normal = mesh->mNormals[vertexIndex];
				aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];

				meshData.vertices[vertexIndex].position = { -position.x,position.y,position.z,1.0f };
				meshData.vertices[vertexIndex].normal = { -normal.x,normal.y,normal.z };
				meshData.vertices[vertexIndex].texcoord = { texcoord.x,texcoord.y };

				// Tangentの追加
				if (mesh->HasTangentsAndBitangents()) {
					aiVector3D& tangent = mesh->mTangents[vertexIndex];
					meshData.vertices[vertexIndex].tangent = { -tangent.x, tangent.y, tangent.z };
				} else {
					// Tangentがない場合はデフォルト値を設定
					meshData.vertices[vertexIndex].tangent = { 1.0f, 0.0f, 0.0f };
				}
			}
			// index解析
			for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
				aiFace& face = mesh->mFaces[faceIndex];
				assert(face.mNumIndices == 3);

				for (uint32_t element = 0; element < face.mNumIndices; ++element) {
					uint32_t vertexIndex = face.mIndices[element];
					meshData.indices.push_back(vertexIndex);
				}
			}

			// ボーン解析
			if (mesh->HasBones()) {
				for (uint32_t boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex) {
					aiBone* bone = mesh->mBones[boneIndex];

					std::string jointName = bone->mName.C_Str();

					// 既存 or 新規作成する JointWeightData を取得
					JointWeightData& jointWeightData = newModelData.skinClusterData[jointName];

					aiMatrix4x4 bindPoseMatrixAssimp = bone->mOffsetMatrix;
					aiVector3D scale, translate;
					aiQuaternion rotate;
					bindPoseMatrixAssimp.Decompose(scale, rotate, translate);

					// X反転
					Matrix4x4 bindPoseMatrix = MakeAffineMatrix(
						{ scale.x, scale.y, scale.z },
						{ rotate.x, -rotate.y, -rotate.z, rotate.w },
						{ -translate.x, translate.y, translate.z }
					);

					jointWeightData.inverseBindPoseMatrix = bindPoseMatrix;

					// 頂点ウェイトの登録
					for (uint32_t weightIndex = 0; weightIndex < bone->mNumWeights; ++weightIndex) {
						float w = bone->mWeights[weightIndex].mWeight;
						int32_t localVtxId = bone->mWeights[weightIndex].mVertexId;

						jointWeightData.jointToVertexWeights.push_back({
							w,
							meshIndex,        // このメッシュ番号
							localVtxId        // メッシュ内ローカル頂点インデックス
							});
					}
				}

			}

		} else {
			assert(false && "Warning: Not Found UV");
		}

		newModelData.meshes.push_back(meshData);
	}

	return newModelData;
}

Node ModelDataContainer::ReadNode(aiNode* node) {
	Node result{};

	aiVector3D scale, translate;
	aiQuaternion rotate;

	node->mTransformation.Decompose(scale, rotate, translate);
	result.transform.scale = { scale.x,scale.y,scale.z };
	result.transform.rotate = { rotate.x,-rotate.y,-rotate.z,rotate.w };
	result.transform.translate = { -translate.x,translate.y,translate.z };

	result.localMatrix = MakeAffineMatrix(result.transform.scale, result.transform.rotate, result.transform.translate);

	result.name = node->mName.C_Str(); // node名を格納
	result.children.resize(node->mNumChildren);// 子供の数だけ確保
	for (uint32_t childIndex = 0; childIndex < node->mNumChildren; childIndex++) {
		// 再帰的に読んで階層構造を作っていく
		result.children[childIndex] = ReadNode(node->mChildren[childIndex]);
	}
	return result;
}

void ModelDataContainer::SetTextureDataContainer(TextureDataContainer* textureDataContainer) {
	assert(textureDataContainer);
	textureDataContainer_ = textureDataContainer;
}
