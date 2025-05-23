#include "Ground.h"

#include <numbers>


Ground::Ground() {
	transform_ = std::make_unique<Transform3D>();

	transform_->GetRotate().x = std::numbers::pi_v<float> *0.5f;

	groundSize_ = 40.0f;

	// 板ポリの形状を設定
	planeData_.verticesOffsets[Plane3DVertices::LeftTop] = { -groundSize_,groundSize_,0.0f };
	planeData_.verticesOffsets[Plane3DVertices::RightTop] = { groundSize_,groundSize_,0.0f };
	planeData_.verticesOffsets[Plane3DVertices::LeftBottom] = { -groundSize_,-groundSize_,0.0f };
	planeData_.verticesOffsets[Plane3DVertices::RightBottom] = { groundSize_,-groundSize_,0.0f };

}

void Ground::Update() {
	transform_->Update();
}

void Ground::Draw() {
	MAGISYSTEM::DrawPlane3D(transform_->GetWorldMatrix(), planeData_, PrimitiveMaterialData3D{});
}
