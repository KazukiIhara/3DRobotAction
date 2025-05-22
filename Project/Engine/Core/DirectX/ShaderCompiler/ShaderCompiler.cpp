#include "ShaderCompiler.h"

#include <cassert>
#include "Logger/Logger.h"

ShaderCompiler::ShaderCompiler() {
	Initialize();
	Logger::Log("ShaderCompiler Initialize\n");
}

ShaderCompiler::~ShaderCompiler() {
	Logger::Log("ShaderCompiler Finalize\n");
}

ComPtr<ID3DBlob> ShaderCompiler::CompileShader(const std::wstring& filePath, const wchar_t* profile) {
	// ログ出力
	Logger::Log(Logger::ConvertString(std::format(L"Begin CompileShader, path:{}, profile:{}\n", filePath, profile)));

	// HLSLファイルの読み込み
	ComPtr<IDxcBlobEncoding> shaderSource = nullptr;
	HRESULT hr = dxcUtils_->LoadFile(filePath.c_str(), nullptr, &shaderSource);
	assert(SUCCEEDED(hr));

	DxcBuffer shaderSourceBuffer{};
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8;

	// コンパイル引数の指定（Shader Model 6.6 + HLSL2021 + Bindless 対応）
	std::vector<LPCWSTR> arguments = {
		filePath.c_str(),               // 入力ファイル
		L"-E", L"main",                 // エントリポイント
		L"-T", profile,                 // ターゲットプロファイル（例: vs_6_6）
		L"-HV", L"2021",                // HLSL言語バージョン（2021=SM6.6）
		L"-Zi", L"-Qembed_debug",       // デバッグ情報
		L"-Zpr",                        // 行優先メモリレイアウト
		L"-D", L"USE_BINDLESS_TEXTURE", // 必要に応じて define 追加
	};

	// 実際のコンパイル処理
	ComPtr<IDxcResult> shaderResult = nullptr;
	hr = dxcCompiler_->Compile(
		&shaderSourceBuffer,
		arguments.data(),
		static_cast<UINT>(arguments.size()),
		includeHandler_.Get(),
		IID_PPV_ARGS(&shaderResult));
	assert(SUCCEEDED(hr));

	// コンパイルエラーをログ出力
	ComPtr<IDxcBlobUtf8> shaderError = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
	if (shaderError && shaderError->GetStringLength() > 0) {
		Logger::Log(shaderError->GetStringPointer());
		assert(false); // エラーがあれば止める
	}

	// バイナリを取得
	ComPtr<ID3DBlob> shaderBlob = nullptr;
	hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
	assert(SUCCEEDED(hr));

	Logger::Log(Logger::ConvertString(std::format(L"Compile Succeeded, path:{}, profile:{}\n", filePath, profile)));

	return shaderBlob;
}

void ShaderCompiler::Initialize() {
	HRESULT hr = S_FALSE;
	hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils_));
	assert(SUCCEEDED(hr));

	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler_));
	assert(SUCCEEDED(hr));

	hr = dxcUtils_->CreateDefaultIncludeHandler(&includeHandler_);
	assert(SUCCEEDED(hr));

	LogDxcVersion();
}

void ShaderCompiler::LogDxcVersion() {
    ComPtr<IDxcVersionInfo> verInfo;
    if (SUCCEEDED(dxcCompiler_->QueryInterface(IID_PPV_ARGS(&verInfo)))) {
        UINT32 major = 0, minor = 0;
        verInfo->GetVersion(&major, &minor);

        std::string msg;

        // commit 情報が取れる場合
        ComPtr<IDxcVersionInfo2> verInfo2;
        if (SUCCEEDED(dxcCompiler_->QueryInterface(IID_PPV_ARGS(&verInfo2)))) {
            UINT32 commitCount = 0;
            char*  commitHash  = nullptr;                 // ← char* (UTF‑8)
            verInfo2->GetCommitInfo(&commitCount, &commitHash);

            msg = std::format(
                     "DXC Version {}.{} ({} commits, hash: {})\n",
                     major, minor, commitCount,
                     commitHash ? commitHash : "");
        }
        else {
            msg = std::format(
                     "DXC Version {}.{}\n", major, minor);
        }

        Logger::Log(msg);                                 // Logger が char* 系 OK ならこれで完了
    }
}