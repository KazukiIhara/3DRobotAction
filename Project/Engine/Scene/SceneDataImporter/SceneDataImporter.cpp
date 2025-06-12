#include "SceneDataImporter.h"

#include <cassert>
#include "Logger/Logger.h"

#include "SceneDataContainer/SceneDataContainer.h"
#include "GameObject3DManager/GameObject3DManager.h"
#include "Renderer3DManager/Renderer3DManager.h"
#include "TransformManager/TransformManager.h"

SceneDataImporter::SceneDataImporter(SceneDataContainer* sceneDataContainer, GameObject3DManager* gameObject3DManager, Renderer3DManager* renderer3DManager, TransformManager* transformManager) {
	assert(sceneDataContainer);
	assert(gameObject3DManager);
	assert(renderer3DManager);
	assert(transformManager);

	sceneDataContainer_ = sceneDataContainer;
	gameObject3DManager_ = gameObject3DManager;
	renderer3DManager_ = renderer3DManager;
	transformManager_ = transformManager;
	Logger::Log("SceneDataImporter Initialize\n");
}

SceneDataImporter::~SceneDataImporter() {
	Logger::Log("SceneDataImporter Finalize\n");
}

void SceneDataImporter::Import(const std::string& scaneDataName, bool isSceneClear) {
	Logger::Log("Begin import sceneData: " + scaneDataName + "\n");

	if (isSceneClear) {
		gameObject3DManager_->Clear();
		renderer3DManager_->Clear();
		transformManager_->Clear();
	}

	SceneData sceneData = sceneDataContainer_->GetData(scaneDataName);

	for (const auto& object3d : sceneData.objects) {
		std::shared_ptr<ModelRenderer> newModelRenderer = std::make_shared<ModelRenderer>(object3d.modelName, object3d.modelName);

		std::shared_ptr<GameObject3D> newGameObject = std::make_shared<GameObject3D>(object3d.objectName, object3d.scale, object3d.rotate, object3d.translate);
		newGameObject->AddModelRenderer(std::move(newModelRenderer));

		gameObject3DManager_->Add(std::move(newGameObject), true);
	}

	Logger::Log("Complete import sceneData: " + scaneDataName + "\n");
}
