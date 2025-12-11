
#pragma once

/// <summary>
/// 計算パイプラインのタイプ
/// </summary>

namespace magi {
enum class ComputePipelineStateType {
	Skinning,
	ParticleInit,
	ParticleEmit,
	ParticleUpdate,

	Num,
};

// パイプラインの種類の数 
inline constexpr uint32_t kComputePipelineStateNum = static_cast<uint32_t>(ComputePipelineStateType::Num);

} // namespace magi
