#include "DebugCamera3D.h"

#include "Framework/MAGI.h"

using namespace MAGIMath;

DebugCamera3D::DebugCamera3D(const std::string& cameraName)
	:Camera3D(cameraName) {

}

void DebugCamera3D::Initialize() {
	Camera3D::Initialize();
	// クオータニオン角を使用しない
	worldTransform_.isUseQuaternion_ = false;
}

void DebugCamera3D::UpdateData() {
	// マウス入力の取得
	POINT cursorPos;
	GetCursorPos(&cursorPos);
	// ウィンドウ座標系へ変換
	ScreenToClient(MAGISYSTEM::GetWindowHandle(), &cursorPos);

	static POINT lastCursorPos = cursorPos;
	POINT delta{};
	delta.x = cursorPos.x - lastCursorPos.x;
	delta.y = cursorPos.y - lastCursorPos.y;

	// マウスホイールの値を取得してズーム操作
	int64_t wheelDelta = MAGISYSTEM::GetMouseWheelDelta();

	// カメラ回転処理
	HandleCameraRotation(worldTransform_.rotate_, delta);
	// カメラ移動処理
	HandleCameraTranslation(worldTransform_.translate_, worldTransform_.rotate_, delta);
	// カメラズーム処理
	HandleCameraZoom(worldTransform_.translate_, worldTransform_.rotate_, wheelDelta);

	// 現在のカーソル位置を保存
	lastCursorPos = cursorPos;

	// 既定クラスの更新処理
	Camera3D::UpdateData();
}

void DebugCamera3D::HandleCameraRotation(Vector3& cameraRotate, const POINT& delta) {
	// マウスの右ボタンが押されているか確認
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) {
		float rotateSpeed = 0.2f;
		if (MAGISYSTEM::PushKey(DIK_LSHIFT)) {
			rotateSpeed *= 0.3f;
		}
		// カメラの回転を更新
		cameraRotate.x -= delta.y * rotateSpeed * MAGISYSTEM::GetDeltaTime(); // 縦方向
		cameraRotate.y -= delta.x * rotateSpeed * MAGISYSTEM::GetDeltaTime(); // 横方向
	}
}

void DebugCamera3D::HandleCameraTranslation(Vector3& cameraTranslate, Vector3& cameraRotate, const POINT& delta) {
	// 中ボタンドラッグで移動
	if (GetAsyncKeyState(VK_MBUTTON) & 0x8000) {
		// 回転からカメラの右方向ベクトルを計算
		Vector3 right{};
		right.x = std::cosf(cameraRotate.y);
		right.y = 0.0f;
		right.z = -std::sinf(cameraRotate.y);

		// 回転からカメラの上方向ベクトルを計算
		Vector3 up{};
		up.x = std::sinf(cameraRotate.x) * std::sinf(cameraRotate.y);
		up.y = std::cosf(cameraRotate.x);
		up.z = std::sinf(cameraRotate.x) * std::cosf(cameraRotate.y);

		// 移動量をローカル座標系で計算
		float moveSpeed = 0.3f;
		if (MAGISYSTEM::PushKey(DIK_LSHIFT)) {
			moveSpeed *= 0.3f;
		}

		Vector3 moveDelta = (right * static_cast<float> (-delta.x) + up * static_cast<float> (delta.y)) * moveSpeed * MAGISYSTEM::GetDeltaTime();

		// カメラ位置を更新
		cameraTranslate += moveDelta;
	}
}

void DebugCamera3D::HandleCameraZoom(Vector3& cameraTranslate, Vector3& cameraRotate, int64_t wheelDelta) {
	if (wheelDelta != 0) {
		float zoomSpeed = 0.3f; // ズーム速度スケール
		if (MAGISYSTEM::PushKey(DIK_LSHIFT)) {
			zoomSpeed *= 0.3f;
		}
		// カメラの forward ベクトルを取得
		Vector3 forward = Forward(cameraRotate);

		// forward に沿ってカメラの位置を更新
		cameraTranslate += forward * (wheelDelta * zoomSpeed * MAGISYSTEM::GetDeltaTime());
	}
}

