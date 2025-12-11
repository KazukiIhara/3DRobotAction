#define NOMINMAX

#include "MuzzleFlashEffect.h"

#include "MAGI.h"

using namespace MAGI;

MuzzleFlashEffect::MuzzleFlashEffect() {
	// マテリアルの設定
	mat_.textureName = "muzzleFlash.png";
	mat_.blendMode = BlendMode::Normal;
	mat_.baseColor = Color::Gold;
}

void MuzzleFlashEffect::Emit() {
	effectTimer_ = effectTime_;
}

void MuzzleFlashEffect::Update(const Vector3& worldPos) {
	effectTimer_ -= MAGISYSTEM::GetDeltaTime();
	effectTimer_ = std::max(effectTimer_, 0.0f);

 const CameraVector camVec = MAGISYSTEM::GetCurrentCamera3D()->GetCameraVector();

	// ワールド行列を作成
	// カメラの軸
    Vector3 right   = Normalize(camVec.right);
    Vector3 up      = Normalize(camVec.up);
    Vector3 forward = Normalize(camVec.forward);

    // Zを反転
    forward = -forward;

    // スケール設定
    const float sx = scale_.x;  // 横幅スケール
    const float sy = scale_.y;  // 縦幅スケール
    const float sz = 1.0f;      // 奥行き

    // X軸
    wMat_.m[0][0] = right.x * sx;
    wMat_.m[0][1] = right.y * sx;
    wMat_.m[0][2] = right.z * sx;
    wMat_.m[0][3] = 0.0f;

    // Y軸
    wMat_.m[1][0] = up.x * sy;
    wMat_.m[1][1] = up.y * sy;
    wMat_.m[1][2] = up.z * sy;
    wMat_.m[1][3] = 0.0f;

    // Z軸
    wMat_.m[2][0] = forward.x * sz;
    wMat_.m[2][1] = forward.y * sz;
    wMat_.m[2][2] = forward.z * sz;
    wMat_.m[2][3] = 0.0f;

    // 平行移動
    wMat_.m[3][0] = worldPos.x;
    wMat_.m[3][1] = worldPos.y;
    wMat_.m[3][2] = worldPos.z;
    wMat_.m[3][3] = 1.0f;

}

void MuzzleFlashEffect::Draw() {
	if (effectTimer_ != 0.0f) {
		MAGISYSTEM::DrawPlane3D(wMat_, data_, mat_);
	}
}
