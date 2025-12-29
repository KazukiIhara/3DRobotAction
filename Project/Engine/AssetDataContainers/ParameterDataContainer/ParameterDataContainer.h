#pragma once

// C++
#include <cstdint>
#include <string>
#include <unordered_map>
#include <variant>
#include <filesystem>
#include <vector>
#include <optional>

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
	/// パラメータデータ
	/// </summary>
	struct ParamData {
		// パラメータのタイプ
		ParamType Type{};
		// 値
		ParamValue Value{};
	};

	/// <summary>
	/// パラメータのノード
	/// </summary>
	struct ParamNode {
		std::unordered_map<std::string, ParamNode> children;
		std::optional<ParamData> value;
	};

	/// <summary>
	/// パラメータデータコンテナクラス
	/// </summary>
	class ParameterDataContainer {
	public:
		ParameterDataContainer();
		~ParameterDataContainer();

		// すべてのファイルを読んでデータコンテナを作成
		void LoadAllData();
		// コンテナ内すべてのデータをファイルに保存
		void SaveAllData();

		// グループの追加
		void AddGroup(const std::string& groupName);

		// タグの追加
		void AddTag(const std::vector<std::string>& path);

		// データの追加
		void AddData(const std::vector<std::string>& path, const ParamData& data);

		// 値の取得
		ParamValue GetValue(std::vector<std::string>& path);

	private:
		// グループコンテナ <グループ名、デバッグ描画フラグ>
		std::vector<std::pair<std::string, bool>> groups_{};

		// パラメータコンテナ <グループ名、
		std::unordered_map<std::string, ParamNode> paramDatas_{};

	};

}