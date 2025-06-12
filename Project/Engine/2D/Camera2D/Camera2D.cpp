#include "Camera2D.h"

#include "Framework/MAGI.h"

using namespace MAGIMath;

Camera2D::Camera2D(const std::string& name) {
	name_ = name;
	Initialize();
}

void Camera2D::Initialize() {
	CreateCameraResource();
	MapCameraData();
}

void Camera2D::Update() {
	// カメラ特有の更新処理があれば

}

void Camera2D::UpdateData() {

}

void Camera2D::TransferCamera(uint32_t rootParameterIndex) {
	MAGISYSTEM::GetDirectXCommandList()->SetGraphicsRootConstantBufferView(rootParameterIndex, cameraResource_->GetGPUVirtualAddress());
}

std::string& Camera2D::GetName() {
	return name_;
}

void Camera2D::CreateCameraResource() {
	cameraResource_ = MAGISYSTEM::CreateBufferResource(sizeof(Camera2DForGPU));
}

void Camera2D::MapCameraData() {
	cameraData_ = nullptr;
	cameraResource_->Map(0, nullptr, reinterpret_cast<void**>(&cameraData_));
	Matrix4x4 viewMat = Inverse(MakeIdentityMatrix4x4());
	Matrix4x4 projectionMat = MakeOrthographicMatrix(0.0f, 0.0f, WindowApp::kClientWidth, WindowApp::kClientHeight, nearClip_, farClip_);
	cameraData_->viewProjection = viewMat * projectionMat;
}