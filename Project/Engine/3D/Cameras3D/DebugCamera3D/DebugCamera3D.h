#pragma once

#include "3D/Cameras3D/Camera3D/Camera3D.h"

class DebugCamera3D:public Camera3D {
public:

	DebugCamera3D();
	~DebugCamera3D()override = default;

	void Initialize()override;

	void Update()override;

	void HandleCameraRotation(const POINT& delta);

	void HandleCameraTranslation(const POINT& delta);

	void HandleCameraZoom(int64_t wheelDelta);
};