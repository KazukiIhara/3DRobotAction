#include "DebugCamera3D.h"

#include "Framework/MAGI.h"

using namespace MAGIMath;

DebugCamera3D::DebugCamera3D()
	:Camera3D() {

}

void DebugCamera3D::Initialize() {
	Camera3D::Initialize();
}

void DebugCamera3D::UpdateData() {
	// マウス入力の取得
	POINT cursorPos;
	GetCursorPos(&cursorPos);
	ScreenToClient(MAGISYSTEM::GetWindowHandle(), &cursorPos);

	static POINT lastCursorPos = cursorPos;
	POINT delta{};
	delta.x = cursorPos.x - lastCursorPos.x;
	delta.y = cursorPos.y - lastCursorPos.y;

	// マウスホイールの値を取得してズーム操作
	int64_t wheelDelta = MAGISYSTEM::GetMouseWheelDelta();

	// カメラ回転処理
	HandleCameraRotation(delta);
	// カメラ移動処理
	HandleCameraTranslation(delta);
	// カメラズーム処理
	HandleCameraZoom(wheelDelta);

	lastCursorPos = cursorPos;

	Camera3D::UpdateData();
}

void DebugCamera3D::HandleCameraRotation(const POINT& delta) {
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) {
		float rotateSpeed = 0.6f;
		if (MAGISYSTEM::PushKey(DIK_LSHIFT)) {
			rotateSpeed *= 0.3f;
		}

		// Yaw（左右）・Pitch（上下）更新
		yaw_ -= delta.x * rotateSpeed * MAGISYSTEM::GetDeltaTime();
		pitch_ += delta.y * rotateSpeed * MAGISYSTEM::GetDeltaTime();

		// Pitchを±89°に制限（ジンバルロック対策）
		const float kLimit = DegreeToRadian(89.0f);
		pitch_ = std::clamp(pitch_, -kLimit, kLimit);
	}
}

void DebugCamera3D::HandleCameraTranslation(const POINT& delta) {
	if (GetAsyncKeyState(VK_MBUTTON) & 0x8000) {
		Vector3 forward = DirectionFromYawPitch(yaw_, pitch_);
		Vector3 right = Normalize(Cross({ 0, 1, 0 }, forward));
		Vector3 up = Normalize(Cross(forward, right));

		float moveSpeed = 1.0f;
		if (MAGISYSTEM::PushKey(DIK_LSHIFT)) {
			moveSpeed *= 0.3f;
		}

		Vector3 moveDelta =
			(right * static_cast<float>(-delta.x) +
				up * static_cast<float>(delta.y)) * moveSpeed * MAGISYSTEM::GetDeltaTime();

		eye_ += moveDelta;
	}
}

void DebugCamera3D::HandleCameraZoom(int64_t wheelDelta) {
	if (wheelDelta != 0) {
		float zoomSpeed = 0.3f;
		if (MAGISYSTEM::PushKey(DIK_LSHIFT)) {
			zoomSpeed *= 0.3f;
		}
		Vector3 forward = DirectionFromYawPitch(yaw_, pitch_);
		eye_ += forward * (wheelDelta * zoomSpeed * MAGISYSTEM::GetDeltaTime());
	}
}