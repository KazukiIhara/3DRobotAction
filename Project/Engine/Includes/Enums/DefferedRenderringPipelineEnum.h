#pragma once

// C++
#include <cstdint>


/// <summary>
/// ディファードレンダリングパイプラインの種類
/// </summary>

namespace magi {
enum class DefferedRenderringType {
	Lighting, // ライティング

	Num,
};

// パイプラインの種類の数
inline constexpr uint32_t kDefferedRenderringPipelineStateNum = static_cast<uint32_t>(DefferedRenderringType::Num);

} // namespace magi
