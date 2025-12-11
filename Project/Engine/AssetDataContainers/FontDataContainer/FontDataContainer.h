#pragma once

// C++
#include <unordered_map>
#include <string>

// MyHedder
#include "Common/Font/FontCommon.h"


namespace magi {
/// <summary>
/// フォントデータコンテナクラス
/// </summary>
class FontDataContainer {
public:
	FontDataContainer();
	~FontDataContainer();

private:
	// フォントデータ
	std::unordered_map<std::string, FontTexture> fontDatas_;
};
}
