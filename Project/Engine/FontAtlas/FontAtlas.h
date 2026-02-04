#pragma once
#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

#include "Math/Utility/MathUtility.h"

namespace Magi {

	struct GlyphInfo {
		int32_t codepoint = 0;

		int32_t x = 0;
		int32_t y = 0;
		int32_t w = 0;
		int32_t h = 0;

		int32_t bearingX = 0;
		int32_t bearingY = 0;
		int32_t advance = 0;
	};

	/// <summary>
	/// フォント読み込みクラス
	/// </summary>
	class FontAtlas {
	public:
		bool BuildAsciiAtlasPng(
			const std::string& fontFilePath,
			const std::string& outPngPath,
			int32_t pixelSize,
			int32_t cellSize,
			int32_t padding
		);

		const GlyphInfo* GetGlyph(char c) const;

		int32_t GetAtlasWidth() const;
		int32_t GetAtlasHeight() const;
		int32_t GetPixelSize() const;

	private:
		void Clear();

	private:
		std::unordered_map<int32_t, GlyphInfo> glyphs_;

		int32_t atlasWidth_ = 0;
		int32_t atlasHeight_ = 0;
		int32_t pixelSize_ = 0;
	};

}