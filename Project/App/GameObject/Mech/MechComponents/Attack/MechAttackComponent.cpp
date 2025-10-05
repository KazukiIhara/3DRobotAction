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
	// コマンドを取得
	const InputCommand command = mechCore->GetInputCommand();

	// 左手武器
	if (command.leftHandWeapon) {
		AttackLeftHand(mechCore);
	}

	// 右手武器
	if (command.rightHandWeapon) {
		AttackRightHand(mechCore);
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

void MechAttackComponent::AttackLeftHand(MechCore* mechCore) {
	// 左手の武器を見に行く
	const HandWeaponType type = mechCore->GetLeftHandWeapon()->GetType();

	// クールタイム中なら早期リターン
	if (mechCore->GetLeftHandWeapon()->IsCoolTime()) {
		return;
	}

	switch (type) {
	case HandWeaponType::AssultRifle:

		// 敵味方の識別を取得
		const FriendlyTag tag = mechCore->GetFriendlyTag();
		// 腕の向きを取得(弾の発射向きになる)
		const Vector3 armDir = mechCore->GetMechArmLeft()->GetForward();
		// 速度を決定
		const float bulletSpeed = mechCore->GetLeftHandWeapon()->GetBulletSpeed();
		// 対象武器の弾発射位置を取得
		const Vector3 fireWPos = mechCore->GetLeftHandWeapon()->GetFireWorldPosition();
		// ダメージを参照
		const int32_t damage = mechCore->GetLeftHandWeapon()->GetDamage();

		// 弾を追加
		attackObjectManager_->AddBullet(tag, armDir, bulletSpeed, fireWPos, damage);

		// クールタイムにする
		mechCore->GetLeftHandWeapon()->SetCoolTime();

		break;
	}

}

void MechAttackComponent::AttackRightHand(MechCore* mechCore) {
	// 右手の武器を見に行く
	const HandWeaponType type = mechCore->GetRightHandWeapon()->GetType();

	// クールタイム中なら早期リターン
	if (mechCore->GetRightHandWeapon()->IsCoolTime()) {
		return;
	}

	// 武騎種類ごとの処理
	switch (type) {
	case HandWeaponType::AssultRifle:
	{
		// 敵味方の識別を取得
		const FriendlyTag tag = mechCore->GetFriendlyTag();
		// 腕の向きを取得(弾の発射向きになる)
		const Vector3 armFwd = mechCore->GetMechArmRight()->GetForward();
		// 速度を決定
		const float bulletSpeed = mechCore->GetRightHandWeapon()->GetBulletSpeed();
		// 対象武器の弾発射位置を取得
		const Vector3 fireWPos = mechCore->GetRightHandWeapon()->GetFireWorldPosition();
		// ダメージを参照
		const int32_t damage = mechCore->GetRightHandWeapon()->GetDamage();

		// 弾を追加
		attackObjectManager_->AddBullet(tag, armFwd, bulletSpeed, fireWPos, damage);

		// クールタイムにする
		mechCore->GetRightHandWeapon()->SetCoolTime();
	}
	break;

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
		// 初期速度取得
		const float firstSpeed = mechCore->GetLeftShoulderWeapon()->GetFirstSpeed();
		// 加速度取得
		const float acc = mechCore->GetLeftShoulderWeapon()->GetAcc();
		// 最大速度取得
		const float maxSpeed = mechCore->GetLeftShoulderWeapon()->GetMaxSpeed();
		// ダメージを参照
		const int32_t damage = mechCore->GetLeftShoulderWeapon()->GetDamage();

		// ターゲットを設定 TODO:(ひとまず機体のロックオン機能から直接持ってくるが、ミサイルランチャーから受け取るようにする)
		std::weak_ptr<MechCore> target = mechCore->GetLockOnComponent()->GetLockOnTarget();

		if (auto tgt = target.lock()) {
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
					attackObjectManager_->AddMissile(tag, MissileType::Dual, wPos, firstSpeed, acc, maxSpeed, dir, damage, target);
				}
			}
		}

		// クールタイムにする
		mechCore->GetLeftShoulderWeapon()->EnableCoolTime();
	}
	break;

	}

}

void MechAttackComponent::AttackRightShoulder(MechCore* mechCore) {
	// 右肩の武器を見に行く

}