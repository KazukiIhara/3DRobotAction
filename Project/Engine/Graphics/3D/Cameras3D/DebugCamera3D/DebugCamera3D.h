#pragma once

#include "3D/Cameras3D/Camera3D/Camera3D.h"

class DebugCamera3D:public Camera3D {
public:

	DebugCamera3D(const std::string& cameraName);
	~DebugCamera3D()override = default;

	void Initialize()override;

	void UpdateData()override;

	void HandleCameraRotation(Vector3& cameraRotate, const POINT& delta);

	void HandleCameraTranslation(Vector3& cameraTranslate, Vector3& cameraRotate, const POINT& delta);

	void HandleCameraZoom(Vector3& cameraTranslate, Vector3& cameraRotate, int64_t wheelDelta);
};