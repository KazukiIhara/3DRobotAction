#include "Ground.h"

#include <numbers>


Ground::Ground() {
	transform_ = std::make_unique<Transform3D>();

	transform_->GetRotate().x = std::numbers::pi_v<float> * 0.5f;

	planeData_.verticesOffsets[Plane3DVertices::LeftTop] = { -20.0f,20.0f,0.0f };
	planeData_.verticesOffsets[Plane3DVertices::RightTop] = { 20.0f,20.0f,0.0f };
	planeData_.verticesOffsets[Plane3DVertices::LeftBottom] = { -20.0f,-20.0f,0.0f };
	planeData_.verticesOffsets[Plane3DVertices::RightBottom] = { 20.0f,-20.0f,0.0f };


}

void Ground::Update() {
	transform_->Update();
}

void Ground::Draw() {
	MAGISYSTEM::DrawPlane3D(transform_->GetWorldMatrix(), planeData_, PrimitiveMaterialData3D{});
}
