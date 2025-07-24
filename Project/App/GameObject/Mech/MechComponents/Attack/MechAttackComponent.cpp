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

	// 左手武器
	if (command.leftHandWeapon) {
		AttackLeftHand(mechCore);
	}

	// 右手武器
	if (command.rightHandWeapon) {
		AttackRightHand(mechCore);
	}

}

void MechAttackComponent::AttackLeftHand(MechCore* mechCore) {
	// 左手の武器を見に行く
	const WeaponType type = mechCore->GetLeftHandWeapon()->GetType();

	// クールタイム中なら早期リターン
	if (mechCore->GetLeftHandWeapon()->IsCoolTime()) {
		return;
	}

	switch (type) {
	case WeaponType::AssultRifle:

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
		bulletManager_->AddBullet(tag, armDir, bulletSpeed, fireWPos, damage);

		// クールタイムにする
		mechCore->GetLeftHandWeapon()->SetCoolTime();

		break;
	}

}

void MechAttackComponent::AttackRightHand(MechCore* mechCore) {
	// 右手の武器を見に行く
	const WeaponType type = mechCore->GetRightHandWeapon()->GetType();

	// クールタイム中なら早期リターン
	if (mechCore->GetRightHandWeapon()->IsCoolTime()) {
		return;
	}

	switch (type) {
	case WeaponType::AssultRifle:

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
		bulletManager_->AddBullet(tag, armFwd, bulletSpeed, fireWPos, damage);

		// クールタイムにする
		mechCore->GetRightHandWeapon()->SetCoolTime();

		break;
	}
}
