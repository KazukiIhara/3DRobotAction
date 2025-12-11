#pragma once

// C++
#include <string>

// DirectX
#include <d3d12.h>
#include <dxcapi.h>

#include "DirectX/ComPtr/ComPtr.h"
#include "Structs/Primitive3DStruct.h"

// 前方宣言
class DXGI;
class ShaderCompiler;

class BaseShadowPipeline {
public:
	BaseShadowPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler);
	virtual ~BaseShadowPipeline();

	// 初期化
	void Initialize();
	// ルートシグネチャを取得する
	ID3D12RootSignature* GetRootSignature();
	// 指定されたブレンドモードに対応するパイプラインステートを取得する
	ID3D12PipelineState* GetPipelineState();

private:
	// ルートシグネチャを作成する
	virtual void CreateRootSignature() = 0;
	// シェーダーをコンパイルする
	virtual void CompileShaders() = 0;
	// グラフィックスパイプラインオブジェクトを作成する
	virtual void CreateGraphicsPipelineObject();
	// DepthStencilStateの設定を行う
	virtual D3D12_DEPTH_STENCIL_DESC DepthStecilDescSetting();
	// InputLayoutの設定を行う
	virtual D3D12_INPUT_LAYOUT_DESC InputLayoutSetting();
	// RasterizerStateの設定を行う
	virtual D3D12_RASTERIZER_DESC RasterizerStateSetting();

private:
	// DXGIのインスタンスをセット
	void SetDXGI(DXGI* dxgi);
	// ShaderCompilerのインスタンスをセット
	void SetShaderCompiler(ShaderCompiler* shaderCompiler);

protected:
	// ルートシグネイチャ
	ComPtr<ID3D12RootSignature> rootSignature_;
	// パイプラインステート
	ComPtr<ID3D12PipelineState> pipelineState_;

	// 増幅シェーダーのバイナリデータ
	ComPtr<ID3DBlob> amplificationShaderBlob_ = nullptr;
	// ピクセルシェーダーのバイナリデータ
	ComPtr<ID3DBlob> pixelShaderBlob_ = nullptr;
	// メッシュシェーダーのバイナリデータ
	ComPtr<ID3DBlob> meshShaderBlob_ = nullptr;
protected:
	DXGI* dxgi_ = nullptr;
	ShaderCompiler* shaderCompiler_ = nullptr;
};