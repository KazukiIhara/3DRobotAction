#include "Camera3DManager.h"

#include "Logger/Logger.h"
#include <cassert>

Camera3DManager::Camera3DManager() {
	Initialize();
	Logger::Log("Camera3DManager Initialize\n");
}

Camera3DManager::~Camera3DManager() {
	Logger::Log("Camera3DManager Finalize\n");
}

void Camera3DManager::Initialize() {
#if defined(DEBUG) || defined(DEVELOP)
	debugCamera_ = std::make_unique<DebugCamera3D>();
#endif
}

void Camera3DManager::Update() {
	if (currentCamera_) {
		currentCamera_->Update();
		currentCamera_->UpdateData();
	}
#if defined(DEBUG)|| defined(DEVELOP)
	if (isDebugCamera_) {
		debugCamera_->UpdateData();
		currentCamera_->DrawFrustum();
	}
#endif
}

void Camera3DManager::TransferCurrentCamera(uint32_t rootParameterIndex) {
#if defined(DEBUG)|| defined(DEVELOP)
	// デバッグカメラフラグがオンの場合デバッグカメラを転送
	if (isDebugCamera_) {
		debugCamera_->TransferCamera(rootParameterIndex);
		return;
	}
#endif
	// 現在選択中のカメラを見つけて転送
	currentCamera_->TransferCamera(rootParameterIndex);
}

void Camera3DManager::TransferCurrentCameraInverse(uint32_t rootParameterIndex) {
#if defined(DEBUG)|| defined(DEVELOP)
	// デバッグカメラフラグがオンの場合デバッグカメラを転送
	if (isDebugCamera_) {
		debugCamera_->TransferCameraInv(rootParameterIndex);
		return;
	}
#endif // _DEBUG
	// 現在選択中のカメラを見つけて転送
	currentCamera_->TransferCameraInv(rootParameterIndex);
}

void Camera3DManager::TransferCurrentCameraFrustum(uint32_t rootParameterIndex) {
	currentCamera_->TransferCameraFrustum(rootParameterIndex);
}

void Camera3DManager::DrawCurrentCameraFrustum() {
	currentCamera_->DrawFrustum();
}

void Camera3DManager::ShakeCurrentCamera(float duration, float intensity) {
	currentCamera_->Shake(duration, intensity);
}

Camera3D* Camera3DManager::Add(const std::string& name, std::unique_ptr<Camera3D> newCamera3D) {
	// コンテナに登録
	cameras3D_.insert(std::pair(name, std::move(newCamera3D)));
	// 追加したカメラのポインタを返す
	return Find(name);
}

void Camera3DManager::Remove(const std::string& cameraName) {
	// 指定した名前のカメラを検索
	if (cameras3D_.contains(cameraName)) {
		// 見つかったら消す
		cameras3D_.erase(cameraName);
	} else {
		// 指定した名前のカメラがない場合止める
		assert(false && "Camera3D Not Found");
	}
}

Camera3D* Camera3DManager::Find(const std::string& cameraName) {
	// 指定した名前のオブジェクトを検索
	auto it = cameras3D_.find(cameraName);

	// 見つかった場合はポインタを返す
	if (it != cameras3D_.end()) {
		return it->second.get();
	}

	// 見つからなかった場合は nullptr を返す
	return nullptr;
}

void Camera3DManager::SetCurrentCamera(const std::string& cameraName) {
	currentCamera_ = Find(cameraName);
}

Camera3D* Camera3DManager::GetCurrentCamera() {
#if defined(DEBUG) || defined(DEVELOP)
	if (isDebugCamera_) {
		return debugCamera_.get();
	}
#endif // _DEBUG

	return currentCamera_;
}

bool& Camera3DManager::GetIsDebugCamera() {
	return isDebugCamera_;
}

void Camera3DManager::Clear() {
	cameras3D_.clear();
}
