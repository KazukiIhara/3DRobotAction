#pragma once

#include <string>

#include "Math/Utility/MathUtility.h"
#include "FontAtlas/FontAtlas.h"

namespace Magi {
	// Forward
	class SpriteDrawer;
	class FontAtlas;

	/// <summary>
	/// SpriteDrawer を使ってフォントを描画するクラス
	/// </summary>
	class FontDrawer {
	public:
		explicit FontDrawer(SpriteDrawer* spriteDrawer, FontAtlas* fontAtlas);
		~FontDrawer() = default;

		// フォントを設定
		bool LoadFont(const std::string& atlasTextureName, const std::string& glyphJsonPath);

		// 描画（1文字=1スプライト）
		void DrawFont(const std::string& text, const Vector2& pos, const Vector4& color, float scale = 1.0f, float lineHeightScale = 1.0f);

		// 背景フラグ
		void SetBack(bool isBack);

	private:
		// 1文字分のスプライトを積む
		void AddGlyph(char c, float& ioPenX, float& ioPenY, const Vector4& color, float scale);

	private:
		SpriteDrawer* spriteDrawer_ = nullptr;
		FontAtlas* atlas_ = nullptr;

		std::string atlasTextureName_{};

		bool isBack_ = false;
		bool isLoaded_ = false;
	};

}