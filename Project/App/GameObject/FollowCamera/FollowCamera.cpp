#include "FollowCamera.h"

FollowCamera::FollowCamera(const std::string& cameraName)
	:Camera3D(cameraName) {}

void FollowCamera::Update() {

}

void FollowCamera::MoveCamera() {

}

void FollowCamera::FollowTarget() {

}

void FollowCamera::SetTarget(const Transform3D* followTarget) {
	followTarget;
}
