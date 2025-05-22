#include "Plane.h"

#include "Framework/MAGI.h"

using namespace MAGIMath;

Plane::Plane()
	:BasePrimitiveShape3D() {
	BasePrimitiveShape3D::Initialize();
}

void Plane::SetShape() {
	primitiveData_ = MAGISYSTEM::GetPrimitiveShape(Primitive3DType::Plane);
}

void Plane::Update() {
	BasePrimitiveShape3D::Update();
}
