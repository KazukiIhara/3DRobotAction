#include "DataIO.h"

#include "ObjectManagers/Renderer3DManager/Renderer3DManager.h"
#include "ObjectManagers/ColliderManager/ColliderManager.h"
#include "ObjectManagers/GameObject3DManager/GameObject3DManager.h"

#include "Logger/Logger.h"

#include <fstream>
#include <filesystem>
#include <cassert>

// Json
#include <nlohmann/json.hpp>
using json = nlohmann::json;

// Windows環境限定
#ifdef _WIN32
#include <Windows.h>
#endif

DataIO::DataIO(
	Renderer3DManager* renderer3DManager,
	ColliderManager* colliderManager,
	GameObject3DManager* gameObject3DManager
) {
	// 
	// インスタンスのセット
	// 
	SetRenderer3DManager(renderer3DManager);
	SetColliderManager(colliderManager);
	SetGameObject3DManager(gameObject3DManager);

	// 
	// 初期化
	// 
	Initialize();
	Logger::Log("DataIO Initialize\n");
}

DataIO::~DataIO() {
	Logger::Log("DataIO Finalize\n");
}

void DataIO::Initialize() {

}

void DataIO::BeginFrame() {

}

void DataIO::EndFrame() {

}

Renderer3DManager* DataIO::GetRenderer3DManager() {
	return renderer3DManager_;
}

ColliderManager* DataIO::GetColliderManager() {
	return colliderManager_;
}

GameObject3DManager* DataIO::GetGameObject3DManager() {
	return gameObject3DManager_;
}

void DataIO::SetRenderer3DManager(Renderer3DManager* renderer3DManager) {
	assert(renderer3DManager);
	renderer3DManager_ = renderer3DManager;
}

void DataIO::SetColliderManager(ColliderManager* colliderManager) {
	assert(colliderManager);
	colliderManager_ = colliderManager;
}

void DataIO::SetGameObject3DManager(GameObject3DManager* gameObject3DManager) {
	assert(gameObject3DManager);
	gameObject3DManager_ = gameObject3DManager;
}
