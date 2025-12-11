#include "Logger.h"

#include <Windows.h>

#include <filesystem>
#include <chrono>
#include <iomanip>
#include <sstream>

std::ofstream Logger::logFile_;

void Logger::Initialize() {

	bool runningUnderVS = IsDebuggerPresent();
	bool runningUnderPix = IsRunningUnderPIX();

	// logsフォルダ作成
	if (runningUnderVS || runningUnderPix) {
		std::filesystem::create_directories("../generated/logs");
	} else {
		std::filesystem::create_directories("logs");
	}

	// 現在時刻取得
	auto now = std::chrono::system_clock::now();
	auto timeT = std::chrono::system_clock::to_time_t(now);
	std::tm localTime{};
#ifdef _WIN32
	localtime_s(&localTime, &timeT);
#else
	localtime_r(&timeT, &localTime);
#endif
	// ファイル名の構築
	std::ostringstream fileNameStream;

	// VSから実行
	if (runningUnderVS || runningUnderPix) {
		fileNameStream << "../generated/logs/"
			<< std::put_time(&localTime, "%Y%m%d_%H%M%S")
			<< ".txt";
	} else {
		fileNameStream << "logs/"
			<< std::put_time(&localTime, "%Y%m%d_%H%M%S")
			<< ".txt";
	}

	// ファイルオープン
	logFile_.open(fileNameStream.str(), std::ios::out | std::ios::trunc);
	Log("[Logger Initialized]");
}

void Logger::Finalize() {
	Log("[Logger Finalized]");
	if (logFile_.is_open()) {
		logFile_.close();
	}
}

void Logger::Log(const std::string& message) {
	// 時刻を取得
	auto now = std::chrono::system_clock::now();
	auto timeT = std::chrono::system_clock::to_time_t(now);
	std::tm localTime{};
#ifdef _WIN32
	localtime_s(&localTime, &timeT);
#else
	localtime_r(&timeT, &localTime);
#endif

	// 時刻プレフィックス生成
	std::ostringstream timePrefix;
	timePrefix << "[" << std::put_time(&localTime, "%H:%M:%S") << "] ";

	// コンソール出力（デバッグ用）
	std::string fullMessage = timePrefix.str() + message;
	OutputDebugStringA(fullMessage.c_str());
	OutputDebugStringA("\n");

	// ファイル出力
	if (logFile_.is_open()) {
		logFile_ << fullMessage << std::endl;
	}
}

std::wstring Logger::ConvertString(const std::string& str) {
	if (str.empty()) {
		return std::wstring();
	}

	auto sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&str[0]), static_cast<int>(str.size()), NULL, 0);
	if (sizeNeeded == 0) {
		return std::wstring();
	}
	std::wstring result(sizeNeeded, 0);
	MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&str[0]), static_cast<int>(str.size()), &result[0], sizeNeeded);
	return result;
}

std::string Logger::ConvertString(const std::wstring& str) {
	if (str.empty()) {
		return std::string();
	}

	auto sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), NULL, 0, NULL, NULL);
	if (sizeNeeded == 0) {
		return std::string();
	}
	std::string result(sizeNeeded, 0);
	WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), result.data(), sizeNeeded, NULL, NULL);
	return result;
}

bool Logger::IsRunningUnderPIX() {
	char buf[2];         // 値は不要なので1バイトあれば良い
	size_t len = 0;
	getenv_s(&len, buf, sizeof(buf), "PIX_PROCESS");
	return len > 0;      // 文字列長が0より大きければ存在
}
