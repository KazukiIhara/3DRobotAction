//------------------------------------------------------------------------------
// Camera3DManager.cpp
//------------------------------------------------------------------------------
#include "Camera3DManager.h"

#include "Logger/Logger.h"
#include <cassert>
#include <memory>

//------------------------------------------------------------------------------
// ctor / dtor
//------------------------------------------------------------------------------
Camera3DManager::Camera3DManager() {
	Initialize();
	Logger::Log("Camera3DManager Initialize\n");
}

Camera3DManager::~Camera3DManager() {
	Logger::Log("Camera3DManager Finalize\n");
}

//------------------------------------------------------------------------------
// 初期化
//------------------------------------------------------------------------------
void Camera3DManager::Initialize() {
#if defined(DEBUG) || defined(DEVELOP)
	debugCamera_ = std::make_shared<DebugCamera3D>();
#endif
}

//------------------------------------------------------------------------------
// 更新
//------------------------------------------------------------------------------
void Camera3DManager::Update() {
	if (auto sp = currentCamera_.lock()) {
		sp->Update();
		sp->UpdateData();
	}

#if defined(DEBUG) || defined(DEVELOP)
	if (isDebugCamera_) {
		debugCamera_->Update();
		debugCamera_->UpdateData();

		// デバッグカメラ中は、実際に描画対象となっているカメラの
		if (auto sp = currentCamera_.lock()) {
			sp->DrawFrustum();
		}
	}
#endif
}

//------------------------------------------------------------------------------
// カメラ定数バッファ転送
//------------------------------------------------------------------------------
void Camera3DManager::TransferCurrentCamera(uint32_t rootParameterIndex) {
#if defined(DEBUG) || defined(DEVELOP)
	if (isDebugCamera_) {
		debugCamera_->TransferCamera(rootParameterIndex);
		return;
	}
#endif
	if (auto sp = currentCamera_.lock()) {
		sp->TransferCamera(rootParameterIndex);
	}
}

void Camera3DManager::TransferCurrentCameraInverse(uint32_t rootParameterIndex) {
#if defined(DEBUG) || defined(DEVELOP)
	if (isDebugCamera_) {
		debugCamera_->TransferCameraInv(rootParameterIndex);
		return;
	}
#endif
	if (auto sp = currentCamera_.lock()) {
		sp->TransferCameraInv(rootParameterIndex);
	}
}

void Camera3DManager::TransferCurrentCameraFrustum(uint32_t rootParameterIndex) {
#if defined(DEBUG) || defined(DEVELOP)
	if (isDebugCamera_) {
		debugCamera_->TransferCameraFrustum(rootParameterIndex);
		return;
	}
#endif
	if (auto sp = currentCamera_.lock()) {
		sp->TransferCameraFrustum(rootParameterIndex);
	}
}

//------------------------------------------------------------------------------
// フラスタム可視化
//------------------------------------------------------------------------------
void Camera3DManager::DrawCurrentCameraFrustum() {
#if defined(DEBUG) || defined(DEVELOP)
	if (isDebugCamera_) {
		debugCamera_->DrawFrustum();
		return;
	}
#endif
	if (auto sp = currentCamera_.lock()) {
		sp->DrawFrustum();
	}
}

//------------------------------------------------------------------------------
// カメラシェイク
//------------------------------------------------------------------------------
void Camera3DManager::ShakeCurrentCamera(float duration, float intensity) {
#if defined(DEBUG) || defined(DEVELOP)
	if (isDebugCamera_) {
		debugCamera_->Shake(duration, intensity);
		return;
	}
#endif
	if (auto sp = currentCamera_.lock()) {
		sp->Shake(duration, intensity);
	}
}

//------------------------------------------------------------------------------
// カメラ登録
//------------------------------------------------------------------------------
std::weak_ptr<Camera3D> Camera3DManager::Add(const std::string& name,
	std::shared_ptr<Camera3D> newCamera3D) {
	assert(newCamera3D && "newCamera3D must not be null");
	assert(!cameras3D_.contains(name) && "Camera3D name already exists!");

	auto [it, success] = cameras3D_.insert({ name, std::move(newCamera3D) });
	assert(success && "Failed to insert Camera3D");

	return std::weak_ptr<Camera3D>(it->second);
}

//------------------------------------------------------------------------------
// カメラ削除
//------------------------------------------------------------------------------
void Camera3DManager::Remove(const std::string& cameraName) {
	if (auto it = cameras3D_.find(cameraName); it != cameras3D_.end()) {
		// 現在のカメラを消す場合は currentCamera_ も無効化
		if (!currentCamera_.expired() && currentCamera_.lock() == it->second) {
			currentCamera_.reset();
		}
		cameras3D_.erase(it);
	} else {
		assert(false && "Camera3D Not Found");
	}
}

//------------------------------------------------------------------------------
// カメラ検索
//------------------------------------------------------------------------------
std::weak_ptr<Camera3D> Camera3DManager::Find(const std::string& cameraName) {
	if (auto it = cameras3D_.find(cameraName); it != cameras3D_.end()) {
		return std::weak_ptr<Camera3D>(it->second);
	}
	return {};   // empty weak_ptr
}

//------------------------------------------------------------------------------
// 使用カメラ指定
//------------------------------------------------------------------------------
void Camera3DManager::SetCurrentCamera(const std::string& cameraName) {
	currentCamera_ = Find(cameraName);
	assert(!currentCamera_.expired() && "Camera3D Not Found");
}

//------------------------------------------------------------------------------
// 使用カメラ取得
//------------------------------------------------------------------------------
std::weak_ptr<Camera3D> Camera3DManager::GetCurrentCamera() {
#if defined(DEBUG) || defined(DEVELOP)
	if (isDebugCamera_) {
		return std::weak_ptr<Camera3D>(debugCamera_);
	}
#endif
	return currentCamera_;
}

//------------------------------------------------------------------------------
// デバッグカメラフラグ
//------------------------------------------------------------------------------
bool& Camera3DManager::GetIsDebugCamera() {
	return isDebugCamera_;
}

//------------------------------------------------------------------------------
// クリア
//------------------------------------------------------------------------------
void Camera3DManager::Clear() {
	cameras3D_.clear();
	currentCamera_.reset();
}
