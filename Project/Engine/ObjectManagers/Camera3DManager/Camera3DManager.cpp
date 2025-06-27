#include "Camera3DManager.h"

#include <memory>

#include "MAGIAssert/MAGIAssert.h"

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

#if defined(DEBUG) || defined(DEVELOP)
	if (isDebugCamera_) {
		debugCamera_->Update();
		debugCamera_->UpdateData();

		if (currentCamera_) {
			currentCamera_->DrawFrustum();
		}
	}
#endif
}

void Camera3DManager::TransferCurrentCamera(uint32_t rootParameterIndex) {
#if defined(DEBUG) || defined(DEVELOP)
	if (isDebugCamera_) {
		debugCamera_->TransferCamera(rootParameterIndex);
		return;
	}
#endif
	if (currentCamera_) {
		currentCamera_->TransferCamera(rootParameterIndex);
	}
}

void Camera3DManager::TransferCurrentCameraInverse(uint32_t rootParameterIndex) {
#if defined(DEBUG) || defined(DEVELOP)
	if (isDebugCamera_) {
		debugCamera_->TransferCameraInv(rootParameterIndex);
		return;
	}
#endif
	if (currentCamera_) {
		currentCamera_->TransferCameraInv(rootParameterIndex);
	}
}

void Camera3DManager::TransferCurrentCameraFrustum(uint32_t rootParameterIndex) {
#if defined(DEBUG) || defined(DEVELOP)
	if (isDebugCamera_) {
		debugCamera_->TransferCameraFrustum(rootParameterIndex);
		return;
	}
#endif
	if (currentCamera_) {
		currentCamera_->TransferCameraFrustum(rootParameterIndex);
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
	if (currentCamera_) {
		currentCamera_->DrawFrustum();
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
	if (currentCamera_) {
		currentCamera_->Shake(duration, intensity);
	}
}

//------------------------------------------------------------------------------
// カメラ登録
//------------------------------------------------------------------------------
std::weak_ptr<Camera3D> Camera3DManager::Add(std::shared_ptr<Camera3D> newCamera3D) {
	MAGIAssert::Assert(newCamera3D.get(), "newCamera3D must not be null");
	cameras3D_.push_back(std::move(newCamera3D));
	return cameras3D_.back();
}

void Camera3DManager::SetCurrentCamera(Camera3D* camera3D) {
	currentCamera_ = camera3D;
	MAGIAssert::Assert(currentCamera_, "CurrentCamera3D must not be null");
}

//------------------------------------------------------------------------------
// 使用カメラ取得
//------------------------------------------------------------------------------
Camera3D* Camera3DManager::GetCurrentCamera() {
#if defined(DEBUG) || defined(DEVELOP)
	if (isDebugCamera_) {
		return debugCamera_.get();
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
	currentCamera_ = nullptr;
}
