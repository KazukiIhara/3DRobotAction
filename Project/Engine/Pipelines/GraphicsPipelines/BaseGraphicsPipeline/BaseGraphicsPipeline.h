#pragma once

// C++
#include <string>

// DirectX
#include <d3d12.h>
#include <dxcapi.h>

#include "DirectX/ComPtr/ComPtr.h"
#include "Includes/Enums/BlendModeEnum.h"
#include "Structs/Primitive3DStruct.h"

// 前方宣言
class DXGI;
class ShaderCompiler;

class BaseGraphicsPipeline {
public:
	BaseGraphicsPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler);
	virtual ~BaseGraphicsPipeline();

	// 初期化
	virtual void Initialize();
	// ルートシグネチャを取得する
	ID3D12RootSignature* GetRootSignature();
	// 指定されたブレンドモードに対応するパイプラインステートを取得する
	ID3D12PipelineState* GetPipelineState(BlendMode blendMode);

protected:
	// ルートシグネチャを作成する
	virtual void CreateRootSignature() = 0;
	// シェーダーをコンパイルする
	virtual void CompileShaders() = 0;
	// グラフィックスパイプラインオブジェクトを作成する
	virtual void CreateGraphicsPipelineObject();
	// BlendStateの設定を行う
	virtual D3D12_BLEND_DESC BlendStateSetting(uint32_t blendModeNum) = 0;
	// DepthStencilStateの設定を行う
	virtual D3D12_DEPTH_STENCIL_DESC DepthStecilDescSetting();
	// ブレンドモードごとにDepthStencilStateの設定を行う
	D3D12_DEPTH_STENCIL_DESC DepthStecilDescSettingBlend(uint32_t blendModeNum);
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
	ComPtr<ID3DBlob> vertexShaderBlob_ = nullptr;
	// ピクセルシェーダーのバイナリデータ
	ComPtr<ID3DBlob> pixelShaderBlob_ = nullptr;
	// メッシュシェーダーのバイナリデータ
	ComPtr<ID3DBlob> meshShaderBlob_ = nullptr;
	// 増幅シェーダーのバイナリデータ
	ComPtr<ID3DBlob> amplificationShaderBlob_ = nullptr;


	// αありのピクセルシェーダーのバイナリデータ
	ComPtr<ID3DBlob> pixelShaderBlobWithAlpha_ = nullptr;
	// αありのメッシュシェーダーのバイナリデータ
	ComPtr<ID3DBlob> meshShaderBlobWithAlpha_ = nullptr;

protected:
	DXGI* dxgi_ = nullptr;
	ShaderCompiler* shaderCompiler_ = nullptr;
};