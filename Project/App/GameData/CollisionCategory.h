#pragma once

// C++
#include <iostream>
#include <string>
#include <vector>

/// <summary>
/// 当たり判定のカテゴリ
/// </summary>
enum class CollisionCategory {
	None,

	Count,
};

// TODO: テンプレートを使ってもっとスマートな実装をする

/// <summary>
/// 当たり判定のカテゴリを文字列型に変換する
/// </summary>
/// <param name="category">カテゴリ</param>
/// <returns>カテゴリの文字列型</returns>
inline std::string CollisionCategoryToString(CollisionCategory category) {
	switch (category) {
	case CollisionCategory::None:
		return "None";
	default:
		return "Unknown";
	}
}

/// <summary>
/// 文字列から当たり判定カテゴリに変換する
/// </summary>
/// <param name="categoryName">カテゴリ文字列</param>
/// <returns>カテゴリ</returns>
inline CollisionCategory StringToCollisionCategory(const std::string& categoryName) {
	if (categoryName == "None") {
		return CollisionCategory::None;
	} else {
		return CollisionCategory::None;
	}
}