#include "MechAttackComponent.h"

#include "GameObject/Mech/MechCore/MechCore.h"

#include "MAGI.h"
#include "MAGIAssert/MAGIAssert.h"

#include "GameObject/BulletManager/BulletManager.h"

using namespace MAGIMath;

MechAttackComponent::MechAttackComponent(BulletManager* bulletManager) {
	bulletManager_ = bulletManager;
}

void MechAttackComponent::Update(MechCore* mechCore) {
	// コマンドを取得
	const InputCommand command = mechCore->GetInputCommand();

	if (command.leftHandWeapon) {
		AttackLeftHand(mechCore);
	}

	if (command.rightHandWeapon) {
		AttackRightHand(mechCore);
	}

}

void MechAttackComponent::AttackLeftHand(MechCore* mechCore) {
	// 左手の武器を見に行く
	const WeaponType type = mechCore->GetLeftHandWeapon()->GetType();

	switch (type) {
	case WeaponType::AssultRifle:
		// 敵味方の識別を取得
		const FriendlyTag tag = FriendlyTag::Player;
		// 腕の向きを取得(弾の発射向きになる)
		const Vector3 armDir = mechCore->GetMechArmLeft()->GetForward();
		// 速度を決定
		const float bulletSpeed = 100.0f;
		// 対象武器の弾発射位置を取得
		const Vector3 fireWPos = mechCore->GetLeftHandWeapon()->GetFireWorldPosition();

		// 弾を追加
		bulletManager_->AddBullet(tag, armDir, bulletSpeed, fireWPos);

		break;
	}

}

void MechAttackComponent::AttackRightHand(MechCore* mechCore) {
	// 右手の武器を見に行く
	const WeaponType type = mechCore->GetRightHandWeapon()->GetType();

	switch (type) {
	case WeaponType::AssultRifle:
		// 敵味方の識別を取得
		const FriendlyTag tag = FriendlyTag::Player;
		// 腕の向きを取得(弾の発射向きになる)
		const Vector3 armFwd = mechCore->GetMechArmRight()->GetForward();
		// 速度を決定
		const float bulletSpeed = 100.0f;
		// 対象武器の弾発射位置を取得
		const Vector3 fireWPos = mechCore->GetRightHandWeapon()->GetFireWorldPosition();

		// 弾を追加
		bulletManager_->AddBullet(tag, armFwd, bulletSpeed, fireWPos);

		break;
	}
}
