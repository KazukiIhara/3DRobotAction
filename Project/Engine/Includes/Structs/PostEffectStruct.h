#pragma once

#include "Enums/PostEffectPipelineEnum.h"


namespace magi {
/// <summary>
/// パラメータ
/// </summary>
struct PostEffectParamater {
	float param[16];
};

struct PostEffectCommand {
	PostEffectType postEffectType;
	PostEffectParamater param;
	uint32_t index;
};
}
