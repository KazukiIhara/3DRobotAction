#pragma once

// C++
#include <cstdint>

/// <summary>
/// ブレンドモードの種類
/// </summary>
enum class BlendMode {
	None,
	Normal,
	Add,
	Subtract,
	Multiply,
	Screen,

	Num,
};

// ブレンドモードの種類の数
inline constexpr uint32_t kBlendModeNum = static_cast<uint32_t>(BlendMode::Num);