#include "HandWeaponDataIO.h"

#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>

#include "MAGIAssert/MAGIAssert.h"

using nlohmann::json;

HandWeaponDataIO::HandWeaponDataIO() {
	// 全データロード
	LoadAll();
}

MechHandWeapon::Param HandWeaponDataIO::GetParam(const std::string& name) {
	name;
	return MechHandWeapon::Param();
}

void HandWeaponDataIO::AddParam(const MechHandWeapon::Param& param) {
	std::pair<std::string, MechHandWeapon::Param> pair = std::make_pair(param.name, param);
	params_.insert(pair);
}

void HandWeaponDataIO::LoadFile(const std::string& name) {
	name;
}

void HandWeaponDataIO::SaveFile(const MechHandWeapon::Param& param) {

	// Jsonを組み立て
	json j;
	j["name"] = param.name;
	j["type"] = MechHandWeapon::ComvertTypeToString(param.type);
	j["damage"] = param.damage;
	j["reloadTime"] = param.reloadTime;
	j["coolTime"] = param.coolTime;
	j["capacity"] = param.ammoCapacity;
	j["speed"] = param.speed;
	j["modelName"] = param.modelName;
	j["fireOffsetLocalPos"] = {
		{ "x", param.fireOffsetLocalPos.x },
		{ "y", param.fireOffsetLocalPos.y },
		{ "z", param.fireOffsetLocalPos.z }
	};

	// ディレクトリを作成
	std::filesystem::create_directories(kDirectoryPath_);

	// ファイルパスを作成
	const std::string filePath = kDirectoryPath_ + param.name + ".json";
	// 書き込み用ファイルストリーム
	std::ofstream ofs;
	// ファイルを書き込み用に開く
	ofs.open(filePath);

	// 失敗時処理
	MAGIAssert::Assert(!ofs.fail(),"Faild open jsonFile");

	// ファイルにjson文字列を書き込む(インデント幅4)
	ofs << std::setw(4) << j << std::endl;
	// ファイルを閉じる
	ofs.close();

}

void HandWeaponDataIO::LoadAll() {

}

void HandWeaponDataIO::SaveAll() {

}

void HandWeaponDataIO::ClearDataFiles() {

}

void HandWeaponDataIO::ClearContainerData() {

}
