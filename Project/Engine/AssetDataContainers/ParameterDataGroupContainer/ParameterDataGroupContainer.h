#pragma once

// C++
#include <cstdint>
#include <string>
#include <unordered_map>
#include <variant>
#include <filesystem>
#include <vector>

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
	/// データ名と紐づけるマップ
	/// </summary>
	struct ParamEntry {
		// <データ名、データ>
		std::unordered_map<std::string, ParamData> datas_;
	};

	/// <summary>
	/// タグ単位のデータ
	/// </summary>
	struct ParamTag {
		std::unordered_map<std::string, ParamEntry> tags_;
	};

	/// <summary>
	/// パラメータデータコンテナクラス
	/// </summary>
	class ParameterDataGroupContainer {
	public:
		ParameterDataGroupContainer();
		~ParameterDataGroupContainer();

		// すべてのファイルを読んでデータコンテナを作成
		void LoadAllData();
		// コンテナ内すべてのデータをファイルに保存
		void SaveAllData();
		
		// グループの追加
		void AddGroup(const std::string &groupName);
		// データの追加

		// データの削除

		// データの取得

	private:
		// グループのリスト
		std::vector<std::string> groupList_{};

		// グループ単位のデータ
		std::unordered_map<std::string, ParamData> datas_{};

	};

}