#pragma once

// C++
#include <string>
#include <vector>

// DirectX
#include <d3d12.h>
#include <dxcapi.h>

// MAGI
#include "DirectX/ComPtr/ComPtr.h"

// シェーダーコンパイラ
class ShaderCompiler {
public:
	ShaderCompiler();
	~ShaderCompiler();

	// シェーダーコンパイル関数
	ComPtr<ID3DBlob> CompileShader(const std::wstring& filePath, const wchar_t* profile);

private:
	// DXC初期化
	void Initialize();

	void LogDxcVersion();

private:
	// DXC関連
	ComPtr<IDxcUtils> dxcUtils_ = nullptr;
	ComPtr<IDxcCompiler3> dxcCompiler_ = nullptr;
	ComPtr<IDxcIncludeHandler> includeHandler_ = nullptr;
};
