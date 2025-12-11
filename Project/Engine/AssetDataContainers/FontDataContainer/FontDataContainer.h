#pragma once

// C++
#include <unordered_map>
#include <string>


// MyHedder
#include "Common/Font/FontCommon.h"

/// <summary>
/// フォントデータコンテナクラス
/// </summary>

namespace magi {
class FontDataContainer {
public:
	FontDataContainer();
	~FontDataContainer();

private:
	// フォントデータ
	std::unordered_map<std::string, FontTexture> fontDatas_;
};

} // namespace magi
