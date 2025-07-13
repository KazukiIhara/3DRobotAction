#pragma once

class MechCore;

/// <summary>
/// 攻撃コンポーネント
/// </summary>
class MechAttackComponent {
public:
	MechAttackComponent();
	~MechAttackComponent() = default;

	// 攻撃
	void AttackLeftHand(MechCore* mechCore);
	void AttackRightHand(MechCore* mechCore);

private:

};