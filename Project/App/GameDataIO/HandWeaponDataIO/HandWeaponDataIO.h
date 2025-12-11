#pragma once

// C++
#include <string>
#include <unordered_map>

// MyHedder
#include "GameObject/MechWeapon/Hand/MechHandWeapon.h"

/// <summary>
/// 手武器データ入出力クラス
/// </summary>
class HandWeaponDataIO {
public:
	HandWeaponDataIO();
	~HandWeaponDataIO() = default;

	// パラメータ取得
	MechHandWeapon::Param GetParam(const std::string& paramName);
	// パラメータ追加
	void AddParam(const MechHandWeapon::Param& param);

	// ファイルをロード
	void LoadFile(const std::string& fileName);
	// ファイルをセーブ
	void SaveFile(const MechHandWeapon::Param& param);

private:
	// ディレクトリ内のすべてのデータをロード
	void LoadAll();
	// コンテナ内のデータをすべてセーブ
	void SaveAll();

	// ディレクトリ内のファイルをすべて削除
	void ClearDataFiles();

	// コンテナ内のデータをすべて削除
	void ClearContainerData();

private:
	// 手武器データのディレクトリ
	const std::string kDirectoryPath_ = "Assets/GameData/Weapon/Hand/";
	
	// データコンテナ
	std::unordered_map<std::string, MechHandWeapon::Param> params_;

};