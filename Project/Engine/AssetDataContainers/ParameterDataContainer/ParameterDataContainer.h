#pragma once

// C++
#include <cstdint>
#include <string>
#include <unordered_map>
#include <variant>
#include <filesystem>

// Json
#include <nlohmann/json.hpp>

// 数学ヘッダ
#include "Math/Types/AllMathTypes.h"

namespace Magi {

	/// <summary>
	/// 扱うパラメータ型
	/// </summary>
	enum class ParamType {
		Int32,
		Float,
		Vec2,
		Vec3,
		Vec4,
	};

	/// <summary>
	/// パラメータ実体
	/// </summary>
	using ParamValue = std::variant<
		int32_t,
		float,
		Vector2,
		Vector3,
		Vector4
	>;

	/// <summary>
	/// パラメータエントリー
	/// </summary>
	struct ParamEntry {
		// パラメータのタイプ
		ParamType Type{};
		// 値
		ParamValue Value{};
	};

	/// <summary>
	/// パラメータデータコンテナクラス
	/// </summary>
	class ParameterDataContainer {
	public:
		ParameterDataContainer();
		~ParameterDataContainer();

	private:
		// すべてのファイルを読んでデータコンテナを作成
		void LoadAllData();
		// コンテナ内すべてのデータをファイルに保存
		void SaveAllData();

		// データの追加

		// データの削除

		// データの取得

	private:
		// パラメータデータマップコンテナ
		std::unordered_map<std::string, ParamEntry> entries_{};

	};

}