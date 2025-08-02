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
	defaultCamera_ = std::make_unique<Camera3D>("DefaultCamera", false);
	defaultCamera_->SetIsUnique(true);

	// デフォルトカメラをセット
	currentCamera_ = debugCamera_.get();
}

void Camera3DManager::Update() {

	for (auto& camera : cameras3D_) {
		camera->Update();
		camera->PlayAnimation();
		camera->UpdateData();
	}

#if defined(DEBUG) || defined(DEVELOP)
	if (isDebugCamera_) {
		debugCamera_->Update();
		debugCamera_->UpdateData();

		for (auto& camera : cameras3D_) {
			camera->DrawFrustum();
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

void Camera3DManager::TransferCurrentCameraVector(uint32_t rootParameterIndex) {
#if defined(DEBUG) || defined(DEVELOP)
	if (isDebugCamera_) {
		debugCamera_->TransferCameraVector(rootParameterIndex);
		return;
	}
#endif
	if (currentCamera_) {
		currentCamera_->TransferCameraVector(rootParameterIndex);
	}
}


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

void Camera3DManager::ShakeCurrentCamera(float duration, const Vector3& intensity) {
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

std::weak_ptr<Camera3D> Camera3DManager::Add(std::shared_ptr<Camera3D> newCamera3D) {
	MAGIAssert::Assert(newCamera3D.get(), "newCamera3D must not be null");
	cameras3D_.push_back(std::move(newCamera3D));
	return cameras3D_.back();
}

void Camera3DManager::SetCurrentCamera(Camera3D* camera3D) {
	currentCamera_ = camera3D;
	MAGIAssert::Assert(currentCamera_, "CurrentCamera3D must not be null");
}

Camera3D* Camera3DManager::GetCurrentCamera() {
#if defined(DEBUG) || defined(DEVELOP)
	if (isDebugCamera_) {
		return debugCamera_.get();
	}
#endif
	return currentCamera_;
}

bool& Camera3DManager::GetIsDebugCamera() {
	return isDebugCamera_;
}

void Camera3DManager::Clear() {
	cameras3D_.clear();
	currentCamera_ = defaultCamera_.get();
}

void Camera3DManager::ClearWithOutUnique() {

}
