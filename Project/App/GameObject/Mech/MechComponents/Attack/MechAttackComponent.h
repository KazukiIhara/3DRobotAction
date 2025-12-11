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
	void AttackLeftShoulder(MechCore* mechCore);
	void AttackRightShoulder(MechCore* mechCore);

private:
	// 攻撃オブジェクトマネージャ
	AttackObjectManager* attackObjectManager_ = nullptr;
};