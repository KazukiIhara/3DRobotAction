#include "MechLockOnComponent.h"

#include "GameObject/Mech/MechParts/MechCore/MechCore.h"

#include "MAGI.h"
#include "MAGIAssert/MAGIAssert.h"

using namespace MAGIMath;

void MechLockOnComponent::Update(MechCore* mechCore) {
	// コマンドを取得
	const InputCommand command = mechCore->GetInputCommand();
	// ロックオンモード切替
	if (command.switchHardLock) {
		enableHardLockOn_ = !enableHardLockOn_;
	}

	// ロックオン対象の捜索してロックオン
	LockOnTarget(mechCore);

}

void MechLockOnComponent::AddMech(std::weak_ptr<MechCore> mech) {
	targetList_.push_back(mech);
}

std::weak_ptr<MechCore> MechLockOnComponent::GetLockOnTarget() {
	return lockOnTarget_;
}

bool MechLockOnComponent::GetEnableHardLockOn() const {
	return enableHardLockOn_;
}

void MechLockOnComponent::LockOnTarget(MechCore* mechCore) {
	// lockOnView
	LockOnView view = mechCore->GetLockOnView();
	const float fovY = 60.f * (std::numbers::pi_v<float> / 180.0f);
	const float aspect = 16.0f / 9.0f;

	// View, Projection, VP
	const Matrix4x4 V = MakeLookAtMatrix(view.eye, view.target, view.up);
	const Matrix4x4 P = MakePerspectiveFovMatrix(fovY, aspect, view.nearClipRange, view.farClipRange);
	Matrix4x4 VP = V * P;

	// 最も中央に近いターゲット探索
	float bestScore = std::numeric_limits<float>::max();
	std::weak_ptr<MechCore> target;
	Vector3 targetPos{};

	for (auto& wpTarget : targetList_) {
		if (auto tgtCore = wpTarget.lock()) {
			Vector3 posW = tgtCore->GetMechBody()->GetGameObject().lock()->GetTransform()->GetWorldPosition();

			// ワールド→クリップ
			Vector4 posH = { posW.x, posW.y, posW.z, 1.0f };
			Vector4 clip = Transform(posH, VP);

			// 視錐台外 or 背面
			if (clip.w <= 0.0f) continue;                // 背面

			Vector3 ndc = { clip.x, clip.y, clip.z };
			ndc /= clip.w;
			if (std::abs(ndc.x) > 1.f || std::abs(ndc.y) > 1.f || ndc.z < 0.f || ndc.z > 1.f)              // NDC 外
				continue;

			// 画面中心からの距離をスコアに
			float score = Length(Vector2{ ndc.x, ndc.y });

			// 距離によるブースト
			float dist = Length(posW - view.eye);
			score *= (1.0f + 0.1f * dist);

			if (score < bestScore) {
				bestScore = score;
				targetPos = posW;
				target = tgtCore;
			}
		}
	}

	// ターゲットのポインタが有効なら
	if (target.lock()) {
		lockOnTarget_ = target;
		view.target = targetPos;
		mechCore->SetLockOnView(view);
	}
}
