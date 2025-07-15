#pragma once

class MechCore;
class BulletManager;

/// <summary>
/// 攻撃コンポーネント
/// </summary>
class MechAttackComponent {
public:
	MechAttackComponent(BulletManager* bulletManager);
	~MechAttackComponent() = default;

	void Update(MechCore* mechCore);

	// 攻撃
	void AttackLeftHand(MechCore* mechCore);
	void AttackRightHand(MechCore* mechCore);

private:


	// 弾マネージャ
	BulletManager* bulletManager_ = nullptr;
};