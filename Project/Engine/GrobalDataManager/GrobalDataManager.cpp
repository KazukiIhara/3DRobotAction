#include "GrobalDataManager.h"

#include "Framework/MAGI.h"

GrobalDataManager::GrobalDataManager() {
	Initialize();
}

void GrobalDataManager::Initialize() {
	// 配列をクリア
	datas_.clear();
	// ヘッダ内で宣言しているディレクトリ内のjsonFileを読み込み
	LoadFiles();
}

void GrobalDataManager::Finalize() {
	datas_.clear();
}

void GrobalDataManager::Update() {
	//if (!ImGui::Begin("GlobalDatas", nullptr, ImGuiWindowFlags_MenuBar)) {
	//	ImGui::End();
	//	return;
	//}
	//if (!ImGui::BeginMenuBar())
	//	return;

	//// 各グループについて
	//for (std::map<std::string, Group>::iterator itGroup = datas_.begin(); itGroup != datas_.end(); ++itGroup) {
	//	// グループ名を取得
	//	const std::string& groupName = itGroup->first;
	//	// グループの参照を取得
	//	Group& group = itGroup->second;

	//	if (!ImGui::BeginMenu(groupName.c_str()))
	//		continue;

	//	// 各項目について
	//	for (std::map<std::string, Item>::iterator itItem = group.items.begin(); itItem != group.items.end(); ++itItem) {
	//		// 項目名を取得
	//		const std::string& itemName = itItem->first;
	//		// 項目の参照を取得
	//		Item& item = itItem->second;

	//		// int32_t型の値を保持していれば
	//		if (std::holds_alternative<int32_t>(item.value)) {
	//			int32_t* ptr = std::get_if<int32_t>(&item.value);
	//			ImGui::DragInt(itemName.c_str(), ptr, 0, 100);
	//		}
	//		// float型の値を保持していれば
	//		else if (std::holds_alternative<float>(item.value)) {
	//			float* ptr = std::get_if<float>(&item.value);
	//			ImGui::DragFloat(itemName.c_str(), ptr, 0, 100);
	//		}
	//		// Vector3型の値を保持していれば
	//		else if (std::holds_alternative<Vector3>(item.value)) {
	//			Vector3* ptr = std::get_if<Vector3>(&item.value);
	//			ImGui::DragFloat3(itemName.c_str(), reinterpret_cast<float*>(ptr), 0.001f);
	//		}
	//		// Bool型の値を保持していれば
	//		else if (std::holds_alternative<bool>(item.value)) {
	//			bool* ptr = std::get_if<bool>(&item.value);
	//			ImGui::Checkbox(itemName.c_str(), ptr);
	//		}
	//	}

	//	// 改行
	//	ImGui::Text("\n");

	//	if (ImGui::Button("Save")) {
	//		SaveFile(groupName);
	//		std::string message = std::format("{}.json saved.", groupName);
	//		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
	//	}

	//	ImGui::EndMenu();
	//}

	//ImGui::EndMenuBar();
	//ImGui::End();
}

void GrobalDataManager::CreateGroup(const std::string& groupName) {
	// 指定名のオブジェクトがなければ追加する
	datas_[groupName];
}

void GrobalDataManager::SaveFile(const std::string& groupName) {
	// グループを検索
	std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);

	// 未登録チェック
	assert(itGroup != datas_.end());

	json root;
	// コンテナにグループの全データをまとめる
	root = json::object();

	// jsonオブジェクト登録
	root[groupName] = json::object();

	// 各項目について
	for (std::map<std::string, Item>::iterator itItem = itGroup->second.items.begin(); itItem != itGroup->second.items.end(); ++itItem) {
		// 項目名を取得
		const std::string& itemName = itItem->first;
		// 項目の参照を取得
		Item& item = itItem->second;

		// int32_t型の値を保持していれば
		if (std::holds_alternative<int32_t>(item.value)) {
			// int32_t型の値を登録
			root[groupName][itemName] = std::get<int32_t>(item.value);
		}
		// float型の値を保持していれば
		else if (std::holds_alternative<float>(item.value)) {
			// float型の値を登録
			root[groupName][itemName] = std::get<float>(item.value);
		}
		// Vector3型の値を保持していれば
		else if (std::holds_alternative<Vector3>(item.value)) {
			// float型のjson配列登録
			// Vector3型の値を取得し、一旦ローカル変数で受ける
			Vector3 value = std::get<Vector3>(item.value);
			// jsonのデータ構造に合わせるため、json::arryで、float型のjson配列として格納する(厳密にはdouble型に暗黙的に変換されるらしい)
			root[groupName][itemName] = json::array({ value.x, value.y, value.z });
		}
		// Bool型の値を保持していれば
		else if (std::holds_alternative<bool>(item.value)) {
			// bool型のjson配列登録
			root[groupName][itemName] = std::get<bool>(item.value);
		}
	}

	// ディレクトリがなければ作成する
	std::filesystem::path dir(kDirectoryPath);
	if (!std::filesystem::exists(dir)) {
		std::filesystem::create_directory(dir);
	}

	// 書き込むjsonファイルのフルパスを合成する
	std::string filePath = kDirectoryPath + groupName + ".json";
	// 書き込み用ファイルストリーム
	std::ofstream ofs;
	// ファイルを書き込み用に開く
	ofs.open(filePath);

	// ファイルオープン失敗時処理
	if (ofs.fail()) {
		std::string message = "Failed open data file for write.";
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		assert(0);
		return;
	}

	// ファイルにjson文字列を書き込む(インデント幅4)
	ofs << std::setw(4) << root << std::endl;
	// ファイルを閉じる
	ofs.close();
}

void GrobalDataManager::LoadFile(const std::string& groupName) {
	// 読み込むjsonファイルのフルパスを合成する
	std::string filePath = kDirectoryPath + groupName + ".json";
	// 読み込み用のファイルストリーム
	std::ifstream ifs;
	// ファイルを読み込み用に開く
	ifs.open(filePath);

	// ファイルオープン失敗なら
	if (ifs.fail()) {
		std::string message = "Faild file open.";
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		assert(0);
		return;
	}

	json root;
	// json文字列からjsonのデータ構造に展開
	ifs >> root;
	// ファイルを閉じる
	ifs.close();

	// グループを検索
	json::iterator itGroup = root.find(groupName);

	// 未登録チェック
	assert(itGroup != root.end());

	// 各アイテムについて
	for (json::iterator itItem = itGroup->begin(); itItem != itGroup->end(); ++itItem) {
		// アイテム名を取得
		const std::string& itemName = itItem.key();

		// int32_t型の値を保持していれば
		if (itItem->is_number_integer()) {
			// int型の値を登録
			int32_t value = itItem->get<int32_t>();
			SetValue(groupName, itemName, value);
		}

		// float型の値を保持していれば
		else if (itItem->is_number_float()) {
			// float型の値を登録
			double value = itItem->get<double>();
			SetValue(groupName, itemName, static_cast<float>(value));
		}

		// 要素数3の配列であれば(Vector3)
		else if (itItem->is_array() && itItem->size() == 3) {
			// float型のjson配列登録
			Vector3 value = { itItem->at(0), itItem->at(1), itItem->at(2) };
			SetValue(groupName, itemName, value);
		}

		// bool型の値を保持していれば
		else if (itItem->is_boolean()) {
			// bool型の値を登録
			bool value = itItem->get<bool>();
			SetValue(groupName, itemName, value);
		}
	}
}

void GrobalDataManager::LoadFiles() {
	// 保存先ディレクトリのパスをローカル変数で宣言する
	std::filesystem::path dir(kDirectoryPath);
	// ディレクトリがなければスキップする
	if (!std::filesystem::exists(dir)) {
		return;
	}
	std::filesystem::directory_iterator dir_it(dir);
	for (const std::filesystem::directory_entry& entry : dir_it) {
		// ファイルパスを取得
		const std::filesystem::path& filePath = entry.path();

		// ファイル拡張子を取得
		std::string extension = filePath.extension().string();
		// .jsonファイル以外はスキップ
		if (extension.compare(".json") != 0) {
			continue;
		}
		// ファイル読み込み
		LoadFile(filePath.stem().string());
	}
}

int32_t GrobalDataManager::GetValueInt(const std::string& groupName, const std::string& key) const {
	// グループを検索
	std::map<std::string, Group>::const_iterator itGroup = datas_.find(groupName);
	// 未登録チェック
	assert(itGroup != datas_.end());

	// グループの参照を取得
	const Group& group = datas_.at(groupName);

	// 指定グループ内に特定のキーが存在するかチェック
	std::map<std::string, Item>::const_iterator itItem = group.items.find(key);
	assert(itItem != group.items.end());

	// 指定グループから指定のキー値を取得
	return std::get<int32_t>(itItem->second.value);
}

float GrobalDataManager::GetValueFloat(const std::string& groupName, const std::string& key) const {
	// グループを検索
	std::map<std::string, Group>::const_iterator itGroup = datas_.find(groupName);
	// 未登録チェック
	assert(itGroup != datas_.end());

	// グループの参照を取得
	const Group& group = datas_.at(groupName);

	// 指定グループ内に特定のキーが存在するかチェック
	std::map<std::string, Item>::const_iterator itItem = group.items.find(key);
	assert(itItem != group.items.end());

	// 指定グループから指定のキー値を取得
	return std::get<float>(itItem->second.value);
}

Vector3 GrobalDataManager::GetValueVector3(const std::string& groupName, const std::string& key) const {
	// グループを検索
	std::map<std::string, Group>::const_iterator itGroup = datas_.find(groupName);
	// 未登録チェック
	assert(itGroup != datas_.end());

	// グループの参照を取得
	const Group& group = datas_.at(groupName);

	// 指定グループ内に特定のキーが存在するかチェック
	std::map<std::string, Item>::const_iterator itItem = group.items.find(key);
	assert(itItem != group.items.end());

	// 指定グループから指定のキー値を取得
	return std::get<Vector3>(itItem->second.value);
}

bool GrobalDataManager::GetValueBool(const std::string& groupName, const std::string& key) const {
	// グループを検索
	std::map<std::string, Group>::const_iterator itGroup = datas_.find(groupName);
	// 未登録チェック
	assert(itGroup != datas_.end());

	// グループの参照を取得
	const Group& group = datas_.at(groupName);

	// 指定グループ内に特定のキーが存在するかチェック
	std::map<std::string, Item>::const_iterator itItem = group.items.find(key);
	assert(itItem != group.items.end());

	// 指定グループから指定のキー値を取得
	return std::get<bool>(itItem->second.value);
}

void GrobalDataManager::SetValue(const std::string& groupName, const std::string& key, int32_t value) {
	// グループの参照を取得
	Group& group = datas_[groupName];
	// 新しい項目のデータを設定
	Item newItem{};
	newItem.value = value;
	// 設定した項目をstd::mapに追加
	group.items[key] = newItem;
}

void GrobalDataManager::SetValue(const std::string& groupName, const std::string& key, float value) {
	// グループの参照を取得
	Group& group = datas_[groupName];
	// 新しい項目のデータを設定
	Item newItem{};
	newItem.value = value;
	// 設定した項目をstd::mapに追加
	group.items[key] = newItem;
}

void GrobalDataManager::SetValue(const std::string& groupName, const std::string& key, Vector3 value) {
	// グループの参照を取得
	Group& group = datas_[groupName];
	// 新しい項目のデータを設定
	Item newItem{};
	newItem.value = value;
	// 設定した項目をstd::mapに追加
	group.items[key] = newItem;
}

void GrobalDataManager::SetValue(const std::string& groupName, const std::string& key, bool value) {
	// グループの参照を取得
	Group& group = datas_[groupName];
	// 新しい項目のデータを設定
	Item newItem{};
	newItem.value = value;
	// 設定した項目をstd::mapに追加
	group.items[key] = newItem;
}

void GrobalDataManager::AddItem(const std::string& groupName, const std::string& key, int32_t value) {
	// グループの参照を取得
	const Group& group = datas_.at(groupName);
	// 指定グループ内に特定のキーが存在するかチェック
	std::map<std::string, Item>::const_iterator itItem = group.items.find(key);

	// 項目が未登録なら
	if (itItem == group.items.end()) {
		SetValue(groupName, key, value);
	}
}

void GrobalDataManager::AddItem(const std::string& groupName, const std::string& key, float value) {
	// グループの参照を取得
	const Group& group = datas_.at(groupName);
	// 指定グループ内に特定のキーが存在するかチェック
	std::map<std::string, Item>::const_iterator itItem = group.items.find(key);

	// 項目が未登録なら
	if (itItem == group.items.end()) {
		SetValue(groupName, key, value);
	}
}

void GrobalDataManager::AddItem(const std::string& groupName, const std::string& key, Vector3 value) {
	// グループの参照を取得
	const Group& group = datas_.at(groupName);
	// 指定グループ内に特定のキーが存在するかチェック
	std::map<std::string, Item>::const_iterator itItem = group.items.find(key);

	// 項目が未登録なら
	if (itItem == group.items.end()) {
		SetValue(groupName, key, value);
	}
}

void GrobalDataManager::AddItem(const std::string& groupName, const std::string& key, bool value) {
	// グループの参照を取得
	const Group& group = datas_.at(groupName);
	// 指定グループ内に特定のキーが存在するかチェック
	std::map<std::string, Item>::const_iterator itItem = group.items.find(key);

	// 項目が未登録なら
	if (itItem == group.items.end()) {
		SetValue(groupName, key, value);
	}
}

