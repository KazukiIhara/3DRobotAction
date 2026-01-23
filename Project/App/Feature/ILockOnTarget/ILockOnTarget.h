#pragma once


#include "Math/Types/Vector3.h"

/// <summary>
/// ロックオン可能オブジェクトのインターフェース
/// </summary>
class ILockOnTarget {
public:
	virtual ~ILockOnTarget() = default;

	virtual Vector3 GetCenterPos() = 0;

};