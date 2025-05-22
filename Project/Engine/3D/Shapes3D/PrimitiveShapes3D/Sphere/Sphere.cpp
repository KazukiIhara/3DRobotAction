#include "Sphere.h"

#include "Framework/MAGI.h"

using namespace MAGIMath;

Sphere::Sphere()
	:BasePrimitiveShape3D() {
	BasePrimitiveShape3D::Initialize();
}

void Sphere::SetShape() {
	primitiveData_ = MAGISYSTEM::GetPrimitiveShape(Primitive3DType::Sphere);
}

void Sphere::Update() {
	BasePrimitiveShape3D::Update();
}
