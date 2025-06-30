#pragma once

// C++
#include <vector>
#include <memory>

// MyHedder
#include "Math/Utility/MathUtility.h"

class MechCore;
class Transform3D;

/// <summary>
/// ロックオンコンポーネント
/// </summary>
class MechLockOnComponent {
public:
	MechLockOnComponent() = default;
	~MechLockOnComponent() = default;

	// 更新
	void Update(MechCore* mechCore);

	// ロックオン対象を追加する
	void AddMech(std::weak_ptr<MechCore> mech);

	// ロックオン対象の機体を取得
	std::weak_ptr<MechCore> GetLockOnTarget();

	// ロックオンモードの取得
	bool GetEnableHardLockOn()const;

private:
	// ロックオン処理
	void LockOnTarget(MechCore* mechCore);

private:
	// ハードロックオンフラグ
	bool enableHardLockOn_ = false;

	// ロックオン対象のリスト
	std::vector<std::weak_ptr<MechCore>> targetList_;

	// ロックオン対象の機体
	std::weak_ptr<MechCore> lockOnTarget_;

};