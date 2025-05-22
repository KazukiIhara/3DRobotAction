#pragma once

// MyHedder
#include "Math/Types/Vector4.h"

/// <summary>
/// 色定数
/// </summary>
struct Color {
    // 赤系
    static constexpr Vector4 Red = { 1.0f, 0.0f, 0.0f, 1.0f }; // 赤
    static constexpr Vector4 Crimson = { 0.86f, 0.08f, 0.24f, 1.0f }; // 深紅
    static constexpr Vector4 Tomato = { 1.0f, 0.39f, 0.28f, 1.0f }; // トマト
    static constexpr Vector4 Coral = { 1.0f, 0.5f, 0.31f, 1.0f }; // 珊瑚

    // 青系
    static constexpr Vector4 Blue = { 0.0f, 0.0f, 1.0f, 1.0f }; // 青
    static constexpr Vector4 SkyBlue = { 0.53f, 0.81f, 0.92f, 1.0f }; // 空色
    static constexpr Vector4 RoyalBlue = { 0.25f, 0.41f, 0.88f, 1.0f }; // ロイヤルブルー
    static constexpr Vector4 Navy = { 0.0f, 0.0f, 0.5f, 1.0f }; // ネイビー

    // 緑系
    static constexpr Vector4 Green = { 0.0f, 1.0f, 0.0f, 1.0f }; // 緑
    static constexpr Vector4 Lime = { 0.0f, 1.0f, 0.0f, 1.0f }; // ライム
    static constexpr Vector4 ForestGreen = { 0.13f, 0.55f, 0.13f, 1.0f }; // 森林緑
    static constexpr Vector4 Olive = { 0.5f, 0.5f, 0.0f, 1.0f }; // オリーブ

    // 黄系
    static constexpr Vector4 Yellow = { 1.0f, 1.0f, 0.0f, 1.0f }; // 黄
    static constexpr Vector4 Gold = { 1.0f, 0.84f, 0.0f, 1.0f }; // ゴールド
    static constexpr Vector4 Khaki = { 0.94f, 0.9f, 0.55f, 1.0f }; // カーキ
    static constexpr Vector4 LemonChiffon = { 1.0f, 0.98f, 0.8f, 1.0f }; // レモンシフォン

    // オレンジ系
    static constexpr Vector4 Orange = { 1.0f, 0.65f, 0.0f, 1.0f }; // オレンジ
    static constexpr Vector4 DarkOrange = { 1.0f, 0.55f, 0.0f, 1.0f }; // 濃いオレンジ

    // 紫系
    static constexpr Vector4 Purple = { 0.5f, 0.0f, 0.5f, 1.0f }; // 紫
    static constexpr Vector4 Violet = { 0.93f, 0.51f, 0.93f, 1.0f }; // 菫色
    static constexpr Vector4 Magenta = { 1.0f, 0.0f, 1.0f, 1.0f }; // マゼンタ
    static constexpr Vector4 Indigo = { 0.29f, 0.0f, 0.51f, 1.0f }; // インディゴ

    // 白・黒系
    static constexpr Vector4 White = { 1.0f, 1.0f, 1.0f, 1.0f }; // 白
    static constexpr Vector4 Black = { 0.0f, 0.0f, 0.0f, 1.0f }; // 黒
    static constexpr Vector4 Gray = { 0.5f, 0.5f, 0.5f, 1.0f }; // 灰色
    static constexpr Vector4 LightGray = { 0.83f, 0.83f, 0.83f, 1.0f }; // 明るい灰色
    static constexpr Vector4 DarkGray = { 0.33f, 0.33f, 0.33f, 1.0f }; // 暗い灰色
};