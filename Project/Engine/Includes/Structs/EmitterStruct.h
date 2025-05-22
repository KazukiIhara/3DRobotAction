#pragma once

#include <cstdint>

#include "Math/Types/Vector3.h"

#include "Structs/ColorStruct.h"
#include "Enums/EmitterEnum.h" 

/// <summary>
/// エミッターの設定構造体
/// </summary>
struct EmitterSetting {
	// 発生のタイプ
	EmitType emitType = EmitType::Default;
	// 発生場所
	Vector3 minTranslate = { 0.0f,0.0f,0.0f };
	Vector3 maxTranslate = { 0.0f,0.0f,0.0f };
	// 移動量
	Vector3	minVelocity = { 0.0f,0.0f,0.0f };
	Vector3 maxVelocity = { 0.0f,0.0f,0.0f };
	// サイズ
	float minScale = 1.0f;
	float maxScale = 1.0f;
	// 色
	Vector4 minColor = Color::White;
	Vector4 maxColor = Color::White;
	// 生存時間
	float minLifeTime = 1.0f;
	float maxLifeTime = 1.0f;
	// 一度の発生個数
	uint32_t count = 1;
	// 繰り返し発生するかどうか
	bool isRepeat = false;
	// 発生頻度
	float frequency = 1.0f;
};