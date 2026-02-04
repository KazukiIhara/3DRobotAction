#include "FontDrawer.h"

#include <cassert>

#include "2D/Drawer2D/SpriteDrawer/SpriteDrawer.h"
#include "FontAtlas/FontAtlas.h"

using namespace Magi;

FontDrawer::FontDrawer(SpriteDrawer* spriteDrawer, FontAtlas* fontAtlas) {
	spriteDrawer_ = spriteDrawer;
	atlas_ = fontAtlas;
}

bool FontDrawer::LoadFont(const std::string& atlasTextureName, const std::string& glyphJsonPath) {
	atlasTextureName_ = atlasTextureName;

	// glyph情報ロード
	const bool ok = atlas_->LoadGlyphJson(glyphJsonPath);
	if (!ok) {
		isLoaded_ = false;
		return false;
	}

	isLoaded_ = true;
	return true;
}

void FontDrawer::DrawFont(const std::string& text, const Vector2& pos, const Vector4& color, float scale, float lineHeightScale) {
	if (!isLoaded_) {
		return;
	}

	float penX = pos.x;
	float penY = pos.y;

	const Vector2 origin = pos;

	for (char c : text) {
		// 改行
		if (c == '\n') {
			penX = origin.x;
			penY += static_cast<float>(atlas_->GetPixelSize()) * lineHeightScale * scale;
			continue;
		}

		AddGlyph(c, penX, penY, color, scale);
	}
}

void FontDrawer::SetBack(bool isBack) {
	isBack_ = isBack;
}

void FontDrawer::AddGlyph(char c, float& ioPenX, float& ioPenY, const Vector4& color, float scale) {
	const Magi::GlyphInfo* g = atlas_->GetGlyph(c);
	if (!g) {
		return;
	}

	// 空白など幅0の扱い
	if (g->w <= 0 || g->h <= 0) {
		ioPenX += static_cast<float>(g->advance) * scale;
		return;
	}

	SpriteData sd{};
	SpriteMaterialData mat{};

	// 前後
	sd.isBack = isBack_;

	// 文字位置
	sd.position = Vector2(
		ioPenX + static_cast<float>(g->bearingX) * scale,
		ioPenY - static_cast<float>(g->bearingY) * scale
	);

	// 文字サイズ
	sd.size = Vector2(
		static_cast<float>(g->w) * scale,
		static_cast<float>(g->h) * scale
	);

	// 回転なし
	sd.rotate = 0.0f;

	// テクスチャ
	mat.textureName = atlasTextureName_;

	// 切り抜き矩形
	mat.textureLeftTop = Vector2(static_cast<float>(g->x), static_cast<float>(g->y));
	mat.textureCutOutSize = Vector2(static_cast<float>(g->w), static_cast<float>(g->h));

	// 色
	mat.color = color;

	// 左上基準
	mat.anchorPoint = Vector2(0.0f, 0.0f);

	// スプライト追加
	spriteDrawer_->AddSprite(sd, mat);

	// 次の文字へ
	ioPenX += static_cast<float>(g->advance) * scale;
}
