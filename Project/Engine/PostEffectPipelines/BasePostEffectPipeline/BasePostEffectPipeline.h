#pragma once

// C++
#include <string>

// DirectX
#include <d3d12.h>
#include <dxcapi.h>

#include "DirectX/ComPtr/ComPtr.h"
#include "Includes/Enums/BlendModeEnum.h"

// 前方宣言
class DXGI;
class ShaderCompiler;

/// <summary>
/// ポストエフェクトパイプラインの基底クラス
/// </summary>
class BasePostEffectPipeline {
public:
	BasePostEffectPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler);
	virtual ~BasePostEffectPipeline();

	// 初期化
	void Initialize();
	// ルートシグネチャを取得する
	ID3D12RootSignature* GetRootSignature();
	// 指定されたブレンドモードに対応するパイプラインステートを取得する
	ID3D12PipelineState* GetPipelineState(BlendMode blendMode);

private:
	// ルートシグネチャを作成する
	virtual void CreateRootSignature() = 0;
	// シェーダーをコンパイルする
	virtual void CompileShaders() = 0;
	// グラフィックスパイプラインオブジェクトを作成する
	virtual void CreateGraphicsPipelineObject() = 0;
	// BlendStateの設定を行う
	virtual D3D12_BLEND_DESC BlendStateSetting(uint32_t blendModeNum) = 0;
	// DepthStencilStateの設定を行う
	virtual D3D12_DEPTH_STENCIL_DESC DepthStecilDescSetting() = 0;
	// InputLayoutの設定を行う
	virtual D3D12_INPUT_LAYOUT_DESC InputLayoutSetting() = 0;
	// RasterizerStateの設定を行う
	virtual D3D12_RASTERIZER_DESC RasterizerStateSetting() = 0;
private:
	// DXGIのインスタンスをセット
	void SetDXGI(DXGI* dxgi);
	// ShaderCompilerのインスタンスをセット
	void SetShaderCompiler(ShaderCompiler* shaderCompiler);
protected:
	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootSignature_;
	// グラフィックスパイプラインステート (ブレンドモードごとに設定)
	ComPtr<ID3D12PipelineState> pipelineState_[kBlendModeNum];
	// 頂点シェーダーのバイナリデータ
	ComPtr<ID3DBlob> vertexShaderBlob_;
	// ピクセルシェーダーのバイナリデータ
	ComPtr<ID3DBlob> pixelShaderBlob_;
protected:
	DXGI* dxgi_ = nullptr;
	ShaderCompiler* shaderCompiler_ = nullptr;
};