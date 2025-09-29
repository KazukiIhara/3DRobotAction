#pragma once

class MechCore;
class AttackObjectManager;

/// <summary>
/// 攻撃コンポーネント
/// </summary>
class MechAttackComponent {
public:
	MechAttackComponent(AttackObjectManager* bulletManager);
	~MechAttackComponent() = default;

	void Update(MechCore* mechCore);

	// 攻撃
	void AttackLeftHand(MechCore* mechCore);
	void AttackRightHand(MechCore* mechCore);
	void AttackLeftShoulder(MechCore* mechCore);
	void AttackRightShoulder(MechCore* mechCore);

private:


	// 弾マネージャ
	AttackObjectManager* attackObjectManager_ = nullptr;
};