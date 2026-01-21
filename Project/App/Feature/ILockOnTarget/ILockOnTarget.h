#pragma once


#include "Math/Types/Vector3.h"

/// <summary>
/// ロックオン可能オブジェクトのインターフェース
/// </summary>
class ILockOnTarget {
public:
	virtual ~ILockOnTarget() = default;

	virtual const Vector3& GetCenterPos() = 0;

};