#pragma once

#include "Enums/PostEffectPipelineEnum.h"


/// <summary>
/// パラメータ
/// </summary>

namespace magi {
struct PostEffectParamater {
	float param[16];
};

struct PostEffectCommand {
	PostEffectType postEffectType;
	PostEffectParamater param;
	uint32_t index;
};

} // namespace magi
