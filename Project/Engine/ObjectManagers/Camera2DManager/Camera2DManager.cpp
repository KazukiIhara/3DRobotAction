#include "Camera2DManager.h"

#include "Logger/Logger.h"
#include <cassert>

Camera2DManager::Camera2DManager() {
	Initialize();
	Logger::Log("Camera2DManager Initialize \n");
}

Camera2DManager::~Camera2DManager() {
	Logger::Log("Camera2DManager Finalize\n");
}

void Camera2DManager::Initialize() {
	Clear();
}

void Camera2DManager::Update() {

}

void Camera2DManager::TransferCurrentCamera(uint32_t rootParameterIndex) {
	currentCamera_->TransferCamera(rootParameterIndex);
}

void Camera2DManager::Add(std::unique_ptr<Camera2D> newCamera2D) {
	const std::string newCameraName = newCamera2D->GetName();
	cameras2D_.insert(std::make_pair(newCameraName, std::move(newCamera2D)));
}

Camera2D* Camera2DManager::Find(const std::string& cameraName) {
	// 指定した名前のオブジェクトを検索
	auto it = cameras2D_.find(cameraName);

	// 見つかった場合はポインタを返す
	if (it != cameras2D_.end()) {
		return it->second.get();
	}

	// 見つからなかった場合は nullptr を返す
	return nullptr;
}

void Camera2DManager::SetCurrentCamera(const std::string& cameraName) {
	currentCamera_ = Find(cameraName);
}

void Camera2DManager::Clear() {
	currentCamera_ = nullptr;
	cameras2D_.clear();
}
