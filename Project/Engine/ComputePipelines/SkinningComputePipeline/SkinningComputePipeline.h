#pragma once

// C++
#include <string>

// DirectX
#include <d3d12.h>
#include <dxcapi.h>

// MyHedder
#include "DirectX/ComPtr/ComPtr.h"

class DXGI;
class ShaderCompiler;

class SkinningComputePipeline {
public:
	SkinningComputePipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler);
	~SkinningComputePipeline();

	// 初期化
	void Initialize(DXGI* dxgi, ShaderCompiler* shaderCompiler);
	// ルートシグネチャを取得する
	ID3D12RootSignature* GetRootSignature();
	// パイプラインステートを取得する
	ID3D12PipelineState* GetPipelineState();

private:
	// ルートシグネチャを作成する
	void CreateRootSignature();
	// シェーダーをコンパイルする
	void CompileShaders();
	// グラフィックスパイプラインオブジェクトを作成する
	void CreatePipelineStateObject();

private:
	// DXGIのインスタンスをセット
	void SetDXGI(DXGI* dxgi);
	// ShaderCompilerのインスタンスをセット
	void SetShaderCompiler(ShaderCompiler* shaderCompiler);

private:
	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootSignature_;
	// パイプラインステート
	ComPtr<ID3D12PipelineState> pipelineState_;
	// コンピュートシェーダーのバイナリデータ
	ComPtr<ID3DBlob> computeShaderBlob_;
private:
	DXGI* dxgi_ = nullptr;
	ShaderCompiler* shaderCompiler_ = nullptr;
};

