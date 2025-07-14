#include "MechAttackComponent.h"

#include "GameObject/Mech/MechCore/MechCore.h"

#include "MAGI.h"
#include "MAGIAssert/MAGIAssert.h"

#include "GameObject/BulletManager/BulletManager.h"

using namespace MAGIMath;

MechAttackComponent::MechAttackComponent(BulletManager* bulletManager) {

}

void MechAttackComponent::AttackLeftHand(MechCore* mechCore) {
	// 左手の武器を見に行く
	const WeaponType type = mechCore->GetLeftHandWeapon()->GetType();

	switch (type) {
	case WeaponType::AssultRifle:
		// 敵味方の識別を取得
		const FriendlyTag tag = FriendlyTag::Player;
		// 腕の向きを取得(弾の発射向きになる)
		const Vector3 armDir = { 0.0f,0.0f,1.0f };
		// 速度を決定
		const float bulletSpeed = 5.0f;
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
		const Vector3 armDir = { 0.0f,0.0f,1.0f };
		// 速度を決定
		const float bulletSpeed = 5.0f;
		// 対象武器の弾発射位置を取得
		const Vector3 fireWPos = mechCore->GetRightHandWeapon()->GetFireWorldPosition();

		// 弾を追加
		bulletManager_->AddBullet(tag, armDir, bulletSpeed, fireWPos);

		break;
	}
}
