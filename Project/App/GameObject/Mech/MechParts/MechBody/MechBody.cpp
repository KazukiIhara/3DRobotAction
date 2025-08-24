#include "MechBody.h"

#include "MAGI.h"
#include "MAGIAssert/MAGIAssert.h" 

#include "GameObject/PlayerCamera/PlayerCamera.h"
#include "GameObject/Mech/MechCore/MechCore.h"

using namespace MAGIMath;

MechBody::MechBody() {
	// レンダラーとゲームオブジェクトを作成
	std::shared_ptr<ModelRenderer> bodyModel = std::make_shared<ModelRenderer>("MechBody", "MechBody");

	std::shared_ptr<GameObject3D> bodyObject = std::make_shared<GameObject3D>("MechBody", Vector3(0.0f, 1.0f, 0.0f));
	bodyObject->AddModelRenderer(std::move(bodyModel));
	bodyObject->SetIsUnique(true);
	// ゲームオブジェクトマネージャに追加
	body_ = MAGISYSTEM::AddGameObject3D(std::move(bodyObject));
}

void MechBody::Update(MechCore* mechCore) {
	// 方向を指定
	DirectionToLockOnView(mechCore);

	RotateToMoveDirection(mechCore);

}

std::weak_ptr<GameObject3D> MechBody::GetGameObject()const {
	return body_;
}

void MechBody::DirectionToLockOnView(MechCore* mechCore) {
	// ロックオン用のカメラ情報を取得
	const LockOnView view = mechCore->GetLockOnView();
	// カメラの前方向を計算 
	Vector3 camFwd = view.target - view.eye;
	// 平面回転に限定するため Y を無効化
	camFwd.y = 0.0f;
	// 正規化
	camFwd = Normalize(camFwd);
	// ヨー角を取得
	const float yaw = std::atan2(camFwd.x, camFwd.z);
	// 回転を作成、適用
	const Quaternion targetQ = MakeRotateAxisAngleQuaternion({ 0.0f,1.0f,0.0f }, yaw);

	if (auto body = body_.lock()) {
		body->GetTransform()->SetQuaternion(targetQ);
	}
}

void MechBody::RotateToMoveDirection(MechCore* mechCore) {

	MechCoreState currentState = mechCore->GetCurrentState();

	Vector3 dir{};
	float spd{};
	float maxDeg{};
	float maxSpd{};

	switch (currentState) {
	case MechCoreState::AssultBoost:
		// カメラの方向を取得
		if (auto mechCoreObj = mechCore->GetGameObject().lock()) {
			if (auto camera = dynamic_cast<PlayerCamera*>(mechCoreObj->GetCamera3D("MainCamera").lock().get())) {
				const Quaternion q = camera->GetCameraQuaternion();

				dir = Normalize(Transform(MakeForwardVector3(), q));

			}
		}
		// 速度を取得(ひとまず設定)
		spd = 30.0f;
		// 最大速度を設定
		maxSpd = 30.0f;
		// 最大角度を設定
		maxDeg = 60.0f;

		break;
	default:
		// 移動方向を取得
		Vector2 moveDir = mechCore->GetMovementComponent()->GetCurrentMoveDir();
		dir = { moveDir.x,0.0f,moveDir.y };
		if (Length(moveDir) < 1e-4f) { return; }
		// 速度を取得
		spd = mechCore->GetMovementComponent()->GetCurrentSpeed();
		if (spd < 1e-4f) { return; }
		// 最大速度を取得
		maxSpd = mechCore->GetMovementComponent()->GetMaxSpeed();
		// 最大角度を設定
		maxDeg = 10.0f;

		break;
	}

	float mul = spd / maxSpd;

	Vector3 fwd = Normalize(dir);
	Vector3 right = Normalize(Cross(Vector3{ 0,1,0 }, fwd));

	float kMaxRollRad = DegreeToRadian(maxDeg) - dir.y;

	Quaternion dq = MakeRotateAxisAngleQuaternion(right, kMaxRollRad * mul);

	if (auto body = body_.lock()) {
		body->GetTransform()->AddQuaterion(dq);
	}
}

void MechBody::RotateToQuickBoost(MechCore* mechCore) {
	if (mechCore->GetCurrentState() != MechCoreState::QuickBoost) { return; }

	Vector2 dir2 = mechCore->GetMovementComponent()->GetCurrentMoveDir();
	float spd = mechCore->GetMovementComponent()->GetCurrentSpeed();
	float maxSpd = mechCore->GetMovementComponent()->GetMaxSpeed();
	float mul = spd / maxSpd;

	Vector3 fwd = Normalize(Vector3{ dir2.x, 0.0f, dir2.y });
	Vector3 right = Normalize(Cross(Vector3{ 0,1,0 }, fwd));

	float kMaxRollDeg = 20.0f;
	float kMaxRollRad = DegreeToRadian(kMaxRollDeg);

	Quaternion dq = MakeRotateAxisAngleQuaternion(right, kMaxRollRad * mul);

	if (auto body = body_.lock()) {
		body->GetTransform()->AddQuaterion(dq);
	}

}
