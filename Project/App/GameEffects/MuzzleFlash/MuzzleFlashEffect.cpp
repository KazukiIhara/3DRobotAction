#define NOMINMAX

#include "MuzzleFlashEffect.h"

#include "MAGI.h"

MuzzleFlashEffect::MuzzleFlashEffect() {
	// マテリアルの設定
	mat_.textureName = "muzzleFlash.png";
	mat_.blendMode = BlendMode::Normal;
	mat_.baseColor = Color::Gold;
	wMat_ = MakeIdentityMatrix4x4();
}

void MuzzleFlashEffect::Emit() {
	effectTimer_ = effectTime_;
}

void MuzzleFlashEffect::Update(const Vector3& worldPos) {
	effectTimer_ -= MAGISYSTEM::GetDeltaTime();
	effectTimer_ = std::max(effectTimer_, 0.0f);
	wMat_ = MAGISYSTEM::GetCurrentCamera3D()->MakeBillBoardMat(worldPos, scale_);
}

void MuzzleFlashEffect::Draw() {
	if (effectTimer_ != 0.0f) {
		MAGISYSTEM::DrawPlane3D(wMat_, data_, mat_);
	}
}