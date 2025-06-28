#pragma once

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

private:
	// ロックオンターゲット
	Transform3D* target_ = nullptr;
};