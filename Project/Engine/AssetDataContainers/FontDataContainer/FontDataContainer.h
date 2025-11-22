#pragma once

// C++
#include <unordered_map>
#include <string>

// MyHedder
#include "Common/Font/FontCommon.h"

/// <summary>
/// フォントデータコンテナクラス
/// </summary>
class FontDataContainer {
public:
	FontDataContainer();
	~FontDataContainer();

	// フォントのロード
	void LoadFont(const std::string& fontName);



private:
	// フォントデータ
	std::unordered_map<std::string, FontTexture> fontDatas_;
};