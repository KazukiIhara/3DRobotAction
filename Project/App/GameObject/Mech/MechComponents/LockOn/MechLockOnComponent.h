#pragma once

// C++
#include <vector>
#include <memory>

// MyHedder
#include "Math/Utility/MathUtility.h"

// 前方宣言
class MechCore;
class Transform3D;

/// <summary>
/// ロックオンコンポーネント
/// </summary>
class MechLockOnComponent {
public:
	MechLockOnComponent(bool enableHardLockOn);
	~MechLockOnComponent() = default;

	// 更新
	void Update(MechCore* mechCore);

	// ロックオン対象を追加する
	void AddMech(MechCore* mech);

	// ロックオン対象の機体を取得
	MechCore* GetLockOnTarget();

	// ロックオンモードの取得
	bool GetEnableHardLockOn()const;

private:
	// ロックオン処理
	void LockOnTarget(MechCore* mechCore);

private:
	// ハードロックオンフラグ
	bool enableHardLockOn_ = false;

	// ロックオン対象のリスト
	std::vector<MechCore*> targetList_;
	// ロックオン対象の機体
	MechCore* lockOnTarget_;


	// ロックオン距離
	float lockOnRange_ = 300.0f;

};