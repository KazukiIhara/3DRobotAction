#pragma once

#include <fstream>
#include <string>
#include <format>

// 
// デバッグログ表示用クラス
// 
class Logger {
public:
	// 初期化
	static void Initialize();
	// 終了
	static void Finalize();

	// デバッグログの出力
	static void Log(const std::string& message);
	// string型をwstring型に変換する関数
	static std::wstring ConvertString(const std::string& str);
	// wstring型をstring型に変換する関数
	static std::string ConvertString(const std::wstring& str);
private:
	static std::ofstream logFile_;
};