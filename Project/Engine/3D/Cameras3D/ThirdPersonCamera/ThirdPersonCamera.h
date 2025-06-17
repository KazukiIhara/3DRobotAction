#pragma once

#include <memory>

#include "Cameras3D/Camera3D/Camera3D.h"

#include "Transform3D/Transform3D.h"

class ThirdPersonCamera :public Camera3D {
public:
	ThirdPersonCamera();
	~ThirdPersonCamera()override;

	void Initialize()override;
	void Update()override;

private:
	std::weak_ptr<Transform3D> targetTransform_;

};