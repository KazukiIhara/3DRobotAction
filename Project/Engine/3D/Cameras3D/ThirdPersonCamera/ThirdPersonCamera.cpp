#include "ThirdPersonCamera.h"

#include "MAGI.h"

ThirdPersonCamera::ThirdPersonCamera()
	:Camera3D() {

}

ThirdPersonCamera::~ThirdPersonCamera() {

}

void ThirdPersonCamera::Initialize() {
	Camera3D::Initialize();
}

void ThirdPersonCamera::Update() {

	const float dt = MAGISYSTEM::GetDeltaTime();
	auto spTarget = target_.lock();
	if (!spTarget) return;

}
