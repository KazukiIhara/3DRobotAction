#include "MechAttackComponent.h"

#include "GameObject/Mech/MechCore/MechCore.h"

#include "MAGI.h"
#include "MAGIAssert/MAGIAssert.h"

#include "GameObject/AttackObjectManager/AttackObjectManager.h"

using namespace MAGIMath;

MechAttackComponent::MechAttackComponent(AttackObjectManager* attackObjectManager) {
	attackObjectManager_ = attackObjectManager;
}

void MechAttackComponent::Update(MechCore* mechCore) {
	// QB中は攻撃できない(キャンセル可能状態から攻撃可能)
	if (mechCore->GetCurrentState() == MechCoreState::QuickBoost &&
		!mechCore->GetMovementComponent()->QuickBoostEnableCancel()) {
		return;
	}

	// コマンドを取得
	const InputCommand command = mechCore->GetInputCommand();

	// 左手武器
	if (command.leftHandWeapon) {
		mechCore->GetLeftHandWeapon()->Attack(mechCore);
	}

	// 右手武器
	if (command.rightHandWeapon) {
		mechCore->GetRightHandWeapon()->Attack(mechCore);
	}

	// 左肩武器
	if (command.leftShoulderWeapon) {
		AttackLeftShoulder(mechCore);
	}

	// 右肩武器
	if (command.rightShoulderWeapon) {
		AttackRightShoulder(mechCore);
	}
}

void MechAttackComponent::AttackLeftShoulder(MechCore* mechCore) {
	// 左肩の武器を見に行く
	const ShoulderWeaponType type = mechCore->GetLeftShoulderWeapon()->GetType();

	// クールタイム中なら早期リターン
	if (mechCore->GetRightShoulderWeapon()->IsCoolTime()) {
		return;
	}

	// 武騎種類ごとの処理
	switch (type) {
	case ShoulderWeaponType::DualMissileLauncher:
	{
		// 敵味方の識別を取得
		const FriendlyTag tag = mechCore->GetFriendlyTag();
		// 発射座標を取得
		const Vector3 wPos = mechCore->GetLeftShoulderWeapon()->GetFireWorldPosition();
		// ダメージを参照
		const int32_t damage = mechCore->GetLeftShoulderWeapon()->GetDamage();

		// ターゲットを設定 TODO:(ひとまず機体のロックオン機能から直接持ってくるが、ミサイルランチャーから受け取るようにする)
		MechCore* target = mechCore->GetLockOnComponent()->GetLockOnTarget();

		if (auto tgt = target) {
			if (auto tgtBody = tgt->GetMechBody()->GetGameObject().lock()) {
				// ターゲットのワールド座標を取得
				const Vector3 targetWPos = tgtBody->GetTransform()->GetWorldPosition();

				// 発射する方向を計算
				Vector3 forward = Normalize(targetWPos - wPos);
				const Vector3 right = Normalize(Cross(MakeUpVector3(), forward));
				Vector3 up = Normalize(Cross(forward, right));

				// コーン半角
				const float spreadDeg = 45.0f;
				const float spreadRad = spreadDeg * (std::numbers::pi_v<float> / 180.0f);
				const float ringScale = tanf(spreadRad); // 小角での半径
				const float startAngleDeg = 90.0f;
				const float startAngleRad = startAngleDeg * (std::numbers::pi_v<float> / 180.0f);

				// 四発発射
				const uint32_t kCount = 4;
				for (uint32_t i = 0; i < kCount; i++) {

					const float theta = startAngleRad + mechCore->GetLeftShoulderWeapon()->GetDualMissileAngles()[i] * (std::numbers::pi_v<float> / 180.0f);
					const Vector3 ringDir = Normalize(right * cosf(theta) + up * sinf(theta));
					const Vector3 dir = Normalize(forward + ringDir * ringScale);

					// 発射
					attackObjectManager_->AddMissile(tag, MissileType::Dual, wPos, dir, damage, target);
				}
			}
		}

		// クールタイムにする
		mechCore->GetLeftShoulderWeapon()->EnableCoolTime();
	}
	break;

	}

}

void MechAttackComponent::AttackRightShoulder([[maybe_unused]] MechCore* mechCore) {
	// 右肩の武器を見に行く

}