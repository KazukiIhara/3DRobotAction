#pragma once

// C++
#include <string>

#include "Math/Types/Vector3.h"

namespace CombatStage {
	struct AABB {
		Vector3 min;
		Vector3 max;
		bool isActive_;

		std::string name;
	};
}