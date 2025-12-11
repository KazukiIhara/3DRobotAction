#pragma once

// C++
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <variant>

// Json
#include <nlohmann/json.hpp>

#include "Math/Utility/MathUtility.h"


/// <summary>
/// using宣言
/// </summary>
using json = nlohmann::json;

/// <summary>
/// グローバル変数
/// </summary>
class GrobalDataManager {
public:
	// 項目
	struct Item {
		// 項目の値
		std::variant<int32_t, float, Vector3, bool> value;
	};
	// グループ
	struct Group {
		std::map<std::string, Item> items;
	};

public:
	GrobalDataManager();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// グループの作成
	/// </summary>
	void CreateGroup(const std::string& groupName);

	/// <summary>
	/// ファイルに書き出し
	/// </summary>
	void SaveFile(const std::string& groupName);

	/// <summary>
	/// ファイルから読み込む
	/// </summary>
	void LoadFile(const std::string& groupName);

	/// <summary>
	/// ディレクトリの全ファイル読み込み
	/// </summary>
	void LoadFiles();

#pragma region Getter
	// 値の取得
	int32_t GetValueInt(const std::string& groupName, const std::string& key) const;
	float GetValueFloat(const std::string& groupName, const std::string& key) const;
	Vector3 GetValueVector3(const std::string& groupName, const std::string& key) const;
	bool GetValueBool(const std::string& groupName, const std::string& key) const;
#pragma endregion

#pragma region Setter
	// 値のセット(int)
	void SetValue(const std::string& groupName, const std::string& key, int32_t value);
	// 値のセット(float)
	void SetValue(const std::string& groupName, const std::string& key, float value);
	// 値のセット(Vector3)
	void SetValue(const std::string& groupName, const std::string& key, Vector3 value);
	// 値のセット(bool)
	void SetValue(const std::string& groupName, const std::string& key, bool value);
#pragma endregion

#pragma region Adder
	// 項目の追加(int)
	void AddItem(const std::string& groupName, const std::string& key, int32_t value);
	// 項目の追加(float)
	void AddItem(const std::string& groupName, const std::string& key, float value);
	// 項目の追加(Vector3)
	void AddItem(const std::string& groupName, const std::string& key, Vector3 value);
	// 項目の追加(bool)
	void AddItem(const std::string& groupName, const std::string& key, bool value);
#pragma endregion
private:
	// 全データ
	std::map<std::string, Group> datas_;
	// グローバル変数の保存先ファイルパス
	const std::string kDirectoryPath = "Assets/Datas/Global/";

private:
};
