#define NOMINMAX

#include "FontAtlas.h"

#include <vector>
#include <fstream>
#include <cassert>
#include <algorithm>

#include <wrl.h>

#include "DirectXTex/DirectXTex.h"

#define STB_TRUETYPE_STATIC
#include "stb/stb_truetype.h"

using Microsoft::WRL::ComPtr;
using namespace Magi;

namespace {
	bool LoadFileBinary(const std::string& path, std::vector<uint8_t>& outData) {
		std::ifstream ifs(path, std::ios::binary);
		if (!ifs) {
			return false;
		}

		ifs.seekg(0, std::ios::end);
		const std::streamoff size = ifs.tellg();
		if (size <= 0) {
			return false;
		}

		outData.resize(static_cast<size_t>(size));
		ifs.seekg(0, std::ios::beg);
		ifs.read(reinterpret_cast<char*>(outData.data()), size);

		return true;
	}

	int32_t CeilDiv(int32_t a, int32_t b) {
		assert(b != 0);
		return (a + b - 1) / b;
	}

	void WriteGlyphToRgba(
		uint8_t* dst,
		int32_t dstW,
		int32_t dstH,
		int32_t dstStrideBytes,
		int32_t dstX,
		int32_t dstY,
		const uint8_t* src,
		int32_t srcW,
		int32_t srcH
	) {
		for (int32_t y = 0; y < srcH; ++y) {
			const int32_t py = dstY + y;
			if (py < 0 || py >= dstH) {
				continue;
			}

			for (int32_t x = 0; x < srcW; ++x) {
				const int32_t px = dstX + x;
				if (px < 0 || px >= dstW) {
					continue;
				}

				const uint8_t a = src[y * srcW + x];

				uint8_t* p = dst + py * dstStrideBytes + px * 4;
				p[0] = 255;
				p[1] = 255;
				p[2] = 255;
				p[3] = a;
			}
		}
	}
}

bool FontAtlas::BuildAsciiAtlasPng(
	const std::string& fontFilePath,
	const std::string& outPngPath,
	int32_t pixelSize,
	int32_t cellSize,
	int32_t padding
) {
	Clear();

	std::vector<uint8_t> fontData;
	// フォント読み込み
	if (!LoadFileBinary(fontFilePath, fontData)) {
		return false;
	}

	stbtt_fontinfo font{};
	// フォント初期化
	if (!stbtt_InitFont(&font, fontData.data(), 0)) {
		return false;
	}

	pixelSize_ = pixelSize;

	const float scale = stbtt_ScaleForPixelHeight(&font, static_cast<float>(pixelSize));
	int32_t ascent = 0;
	int32_t descent = 0;
	int32_t lineGap = 0;
	// 行メトリクス取得
	stbtt_GetFontVMetrics(&font, &ascent, &descent, &lineGap);

	const int32_t ascentPx = static_cast<int32_t>(std::round(ascent * scale));

	const int32_t first = 32;
	const int32_t last = 126;
	const int32_t count = last - first + 1;

	const int32_t cols = 16;
	const int32_t rows = CeilDiv(count, cols);

	atlasWidth_ = cols * cellSize;
	atlasHeight_ = rows * cellSize;

	std::vector<uint8_t> rgba;
	// RGBAバッファ確保
	rgba.resize(static_cast<size_t>(atlasWidth_) * static_cast<size_t>(atlasHeight_) * 4);

	const int32_t stride = atlasWidth_ * 4;

	for (int32_t c = first; c <= last; ++c) {
		const int32_t idx = c - first;
		const int32_t col = idx % cols;
		const int32_t row = idx / cols;

		const int32_t cellX = col * cellSize;
		const int32_t cellY = row * cellSize;

		int32_t advance = 0;
		int32_t lsb = 0;
		// 横メトリクス取得
		stbtt_GetCodepointHMetrics(&font, c, &advance, &lsb);

		int32_t x0 = 0, y0 = 0, x1 = 0, y1 = 0;
		// ビットマップ範囲取得
		stbtt_GetCodepointBitmapBox(&font, c, scale, scale, &x0, &y0, &x1, &y1);

		const int32_t gw = x1 - x0;
		const int32_t gh = y1 - y0;

		std::vector<uint8_t> mono;
		// グリフビットマップ生成
		mono.resize(static_cast<size_t>(std::max(gw, 0)) * static_cast<size_t>(std::max(gh, 0)));

		if (gw > 0 && gh > 0) {
			stbtt_MakeCodepointBitmap(
				&font,
				mono.data(),
				gw,
				gh,
				gw,
				scale,
				scale,
				c
			);
		}

		const int32_t baseX = cellX + padding;
		const int32_t baseY = cellY + padding + ascentPx;

		const int32_t dstX = baseX + x0;
		const int32_t dstY = baseY + y0;

		// アトラスへ転写
		if (gw > 0 && gh > 0) {
			WriteGlyphToRgba(
				rgba.data(),
				atlasWidth_,
				atlasHeight_,
				stride,
				dstX,
				dstY,
				mono.data(),
				gw,
				gh
			);
		}

		GlyphInfo info{};
		info.codepoint = c;

		info.x = dstX;
		info.y = dstY;
		info.w = gw;
		info.h = gh;

		info.bearingX = x0;
		info.bearingY = -y0;

		info.advance = static_cast<int32_t>(std::round(advance * scale));

		glyphs_.emplace(c, info);
	}

	DirectX::ScratchImage img;
	// 画像作成
	if (FAILED(img.Initialize2D(DXGI_FORMAT_R8G8B8A8_UNORM, atlasWidth_, atlasHeight_, 1, 1))) {
		Clear();
		return false;
	}

	const DirectX::Image* dstImg = img.GetImage(0, 0, 0);
	// ピクセル転送
	for (int32_t y = 0; y < atlasHeight_; ++y) {
		uint8_t* dstRow = dstImg->pixels + dstImg->rowPitch * y;
		const uint8_t* srcRow = rgba.data() + static_cast<size_t>(stride) * y;
		std::memcpy(dstRow, srcRow, static_cast<size_t>(stride));
	}

	std::wstring outW(outPngPath.begin(), outPngPath.end());
	// PNG保存
	if (FAILED(DirectX::SaveToWICFile(
		*dstImg,
		DirectX::WIC_FLAGS_NONE,
		DirectX::GetWICCodec(DirectX::WIC_CODEC_PNG),
		outW.c_str()
	))) {
		Clear();
		return false;
	}

	return true;
}

const GlyphInfo* FontAtlas::GetGlyph(char c) const {
	const int32_t cp = static_cast<unsigned char>(c);
	auto it = glyphs_.find(cp);
	if (it == glyphs_.end()) {
		return nullptr;
	}
	return &it->second;
}

int32_t FontAtlas::GetAtlasWidth() const {
	return atlasWidth_;
}

int32_t FontAtlas::GetAtlasHeight() const {
	return atlasHeight_;
}

int32_t FontAtlas::GetPixelSize() const {
	return pixelSize_;
}

void FontAtlas::Clear() {
	glyphs_.clear();
	atlasWidth_ = 0;
	atlasHeight_ = 0;
	pixelSize_ = 0;
}
