#include "LightingDefferedRenderringPipeline.h"

#include <cassert>

#include "Logger/Logger.h"
#include "DirectX/DXGI/DXGI.h"
#include "DirectX/ShaderCompiler/ShaderCompiler.h"

LightingDefferedRenderringPipeline::LightingDefferedRenderringPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler)
	:BaseDefferedRenderringPipeline(dxgi, shaderCompiler) {
}

LightingDefferedRenderringPipeline::~LightingDefferedRenderringPipeline() {}

void LightingDefferedRenderringPipeline::CreateRootSignature() {
	HRESULT hr;

	// --- 各SRV（GBufferそれぞれ個別） ---
	D3D12_DESCRIPTOR_RANGE rangeAlbedo{};
	rangeAlbedo.BaseShaderRegister = 0; // t0
	rangeAlbedo.NumDescriptors = 1;
	rangeAlbedo.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	rangeAlbedo.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE rangeNormal{};
	rangeNormal.BaseShaderRegister = 1; // t1
	rangeNormal.NumDescriptors = 1;
	rangeNormal.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	rangeNormal.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE rangeDepthTex{};
	rangeDepthTex.BaseShaderRegister = 2; // t2
	rangeDepthTex.NumDescriptors = 1;
	rangeDepthTex.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	rangeDepthTex.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE rangeShadow{};
	rangeShadow.BaseShaderRegister = 3; // t3
	rangeShadow.NumDescriptors = 1;
	rangeShadow.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	rangeShadow.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE rangeEnvironmentTex{};
	rangeEnvironmentTex.BaseShaderRegister = 4; // t4
	rangeEnvironmentTex.NumDescriptors = 1;
	rangeEnvironmentTex.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	rangeEnvironmentTex.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;


	// --- ルートパラメータ ---
	D3D12_ROOT_PARAMETER rootParams[9]{};

	// b0 : カメラ用CBV
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParams[0].Descriptor.ShaderRegister = 0;

	// b1 : DirectionalLight
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParams[1].Descriptor.ShaderRegister = 1;

	// b2 : ライトカメラVP行列
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParams[2].Descriptor.ShaderRegister = 2;

	// b3 : InvCamera
	rootParams[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParams[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParams[3].Descriptor.ShaderRegister = 3;

	// t0 : AlbedoTexture
	rootParams[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParams[4].DescriptorTable.pDescriptorRanges = &rangeAlbedo;
	rootParams[4].DescriptorTable.NumDescriptorRanges = 1;

	// t1 : NormalTexture
	rootParams[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParams[5].DescriptorTable.pDescriptorRanges = &rangeNormal;
	rootParams[5].DescriptorTable.NumDescriptorRanges = 1;

	// t2 : DepthTex
	rootParams[6].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[6].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParams[6].DescriptorTable.pDescriptorRanges = &rangeDepthTex;
	rootParams[6].DescriptorTable.NumDescriptorRanges = 1;

	// t3 : ShadowMapTex
	rootParams[7].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[7].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParams[7].DescriptorTable.pDescriptorRanges = &rangeShadow;
	rootParams[7].DescriptorTable.NumDescriptorRanges = 1;

	// t4 : EnvironmentTex
	rootParams[8].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[8].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParams[8].DescriptorTable.pDescriptorRanges = &rangeEnvironmentTex;
	rootParams[8].DescriptorTable.NumDescriptorRanges = 1;

	// --- Static Sampler ---
	D3D12_STATIC_SAMPLER_DESC samplers[3]{};

	// s0 : 通常サンプラー
	samplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	samplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplers[0].ShaderRegister = 0;
	samplers[0].RegisterSpace = 0;
	samplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	samplers[0].MaxLOD = D3D12_FLOAT32_MAX;

	// s1 : シャドウマップ用比較サンプラー
	samplers[1].Filter = D3D12_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
	samplers[1].AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	samplers[1].AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	samplers[1].AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	samplers[1].BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE;
	samplers[1].ComparisonFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	samplers[1].ShaderRegister = 1;  // register(s1)
	samplers[1].RegisterSpace = 0;
	samplers[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	samplers[1].MinLOD = 0;
	samplers[1].MaxLOD = D3D12_FLOAT32_MAX;

	// s2 : ポイントフィルター用サンプラー
	samplers[2].Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
	samplers[2].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplers[2].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplers[2].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplers[2].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplers[2].ShaderRegister = 2;
	samplers[2].RegisterSpace = 0;
	samplers[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	samplers[2].MaxLOD = D3D12_FLOAT32_MAX;

	// --- ルートシグネチャ作成 ---
	D3D12_ROOT_SIGNATURE_DESC rootSigDesc{};
	rootSigDesc.NumParameters = _countof(rootParams);
	rootSigDesc.pParameters = rootParams;
	rootSigDesc.NumStaticSamplers = _countof(samplers);
	rootSigDesc.pStaticSamplers = samplers;
	rootSigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	ComPtr<ID3DBlob> sigBlob;
	ComPtr<ID3DBlob> errorBlob;
	hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &sigBlob, &errorBlob);
	if (FAILED(hr)) {
		if (errorBlob) {
			Logger::Log(reinterpret_cast<const char*>(errorBlob->GetBufferPointer()));
		}
		assert(false && "Lighting RootSignature作成失敗");
	}

	hr = dxgi_->GetDevice()->CreateRootSignature(0, sigBlob->GetBufferPointer(), sigBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature_));
	assert(SUCCEEDED(hr));
}


void LightingDefferedRenderringPipeline::CompileShaders() {
	vertexShaderBlob_ = nullptr;
	vertexShaderBlob_ = shaderCompiler_->CompileShader(L"EngineAssets/Shaders/DefferedRenderring/Lighting/Lighting.VS.hlsl", L"vs_6_0");
	assert(vertexShaderBlob_ != nullptr);

	pixelShaderBlob_ = nullptr;
	pixelShaderBlob_ = shaderCompiler_->CompileShader(L"EngineAssets/Shaders/DefferedRenderring/Lighting/Lighting.PS.hlsl", L"ps_6_0");
	assert(pixelShaderBlob_ != nullptr);
}

void LightingDefferedRenderringPipeline::CreateGraphicsPipelineObject() {
	HRESULT hr;

	assert(rootSignature_);
	assert(vertexShaderBlob_);
	assert(pixelShaderBlob_);

	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = rootSignature_.Get();
	graphicsPipelineStateDesc.InputLayout = InputLayoutSetting();
	graphicsPipelineStateDesc.VS = { vertexShaderBlob_->GetBufferPointer(),
	vertexShaderBlob_->GetBufferSize() };
	graphicsPipelineStateDesc.PS = { pixelShaderBlob_->GetBufferPointer(),
	pixelShaderBlob_->GetBufferSize() };
	graphicsPipelineStateDesc.RasterizerState = RasterizerStateSetting();
	//書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	//利用するトポロジ(形状)のタイプ、三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType =
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	//どのように画面に色を打ち込むかの設定(気にしなくて良い)
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	/*DepthStencilの設定*/
	graphicsPipelineStateDesc.DepthStencilState = DepthStecilDescSetting();
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	// ブレンドモードの設定
	graphicsPipelineStateDesc.BlendState = BlendStateSetting();

	// パイプライン生成
	pipelineState_ = nullptr;
	hr = dxgi_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&pipelineState_));
	assert(SUCCEEDED(hr));
}

D3D12_BLEND_DESC LightingDefferedRenderringPipeline::BlendStateSetting() {
	// NONE  
	D3D12_BLEND_DESC blendDesc{};
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	return blendDesc;
}

D3D12_DEPTH_STENCIL_DESC LightingDefferedRenderringPipeline::DepthStecilDescSetting() {
	// DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	// Depthの機能を無効化する
	depthStencilDesc.DepthEnable = false;
	return depthStencilDesc;
}

D3D12_INPUT_LAYOUT_DESC LightingDefferedRenderringPipeline::InputLayoutSetting() {
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = nullptr;
	inputLayoutDesc.NumElements = 0;
	return inputLayoutDesc;
}

D3D12_RASTERIZER_DESC LightingDefferedRenderringPipeline::RasterizerStateSetting() {
	// RasterizerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	// 裏側(時計回り)を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	// 三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	rasterizerDesc.DepthBias = 1000;    // 定数バイアス
	rasterizerDesc.DepthBiasClamp = 0.0f;    // 最大バイアスクランプ（使わないなら 0）
	rasterizerDesc.SlopeScaledDepthBias = 1.5f;    // 法線傾きに応じたスケールバイアス
	return rasterizerDesc;
}
