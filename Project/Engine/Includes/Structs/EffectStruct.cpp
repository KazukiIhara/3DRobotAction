#include "EffectStruct.h"

PlaneData3D Lerp(const PlaneData3D& p0, const PlaneData3D& p1, float t) {
	PlaneData3D result{};
	for (int i = 0; i < 4; ++i) {
		result.verticesOffsets[i] = MAGIMath::Lerp(p0.verticesOffsets[i], p1.verticesOffsets[i], t);
	}
	return result;
}

void PlaneEffectParam::Initialize() {
	scale.animation = SimpleAnimation{
		Vector3{1.0f,1.0f,1.0f},
		Vector3{1.0f,1.0f,1.0f},
	};
	rotate.animation = SimpleAnimation{
		Vector3{0.0f,0.0f,0.0f},
		Vector3{0.0f,0.0f,0.0f},
	};
	translate.animation = SimpleAnimation{
		Vector3{0.0f,0.0f,0.0f},
		Vector3{0.0f,0.0f,0.0f},
	};

	uvScale.animation = SimpleAnimation{
		Vector2{1.0f,1.0f},
		Vector2{1.0f,1.0f},
	};
	uvRotate.animation = SimpleAnimation{
		float{0.0f},
		float{0.0f},
	};
	uvTranslate.animation = SimpleAnimation{
		Vector2{0.0f,0.0f},
		Vector2{0.0f,0.0f},
	};
	color.animation = SimpleAnimation{
		Vector4{1.0f,1.0f,1.0f,1.0f},
		Vector4{1.0f,1.0f,1.0f,1.0f},
	};
	shape.animation = SimpleAnimation{
		PlaneData3D{},
		PlaneData3D{},
	};
}
