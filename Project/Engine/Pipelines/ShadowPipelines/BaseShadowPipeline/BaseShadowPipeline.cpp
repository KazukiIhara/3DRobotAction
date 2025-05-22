#include "BaseShadowPipeline.h"

#include <cassert>

#include "Logger/Logger.h"
#include "DirectX/DXGI/DXGI.h"
#include "DirectX/ShaderCompiler/ShaderCompiler.h"

BaseShadowPipeline::BaseShadowPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler) {
	SetDXGI(dxgi);
	SetShaderCompiler(shaderCompiler);
}

BaseShadowPipeline::~BaseShadowPipeline() {
}

void BaseShadowPipeline::Initialize() {
	CreateRootSignature();
	CompileShaders();
	CreateGraphicsPipelineObject();
}

ID3D12RootSignature* BaseShadowPipeline::GetRootSignature() {
	return rootSignature_.Get();
}

ID3D12PipelineState* BaseShadowPipeline::GetPipelineState() {
	return pipelineState_.Get();
}

void BaseShadowPipeline::CreateGraphicsPipelineObject() {
	assert(rootSignature_);
	assert(meshShaderBlob_);
	assert(pixelShaderBlob_);

	constexpr DXGI_FORMAT dsvFormat = DXGI_FORMAT_D24_UNORM_S8_UINT; // 深度のみ

	D3D12_SHADER_BYTECODE amplificationShader = { amplificationShaderBlob_->GetBufferPointer(), amplificationShaderBlob_->GetBufferSize() };
	D3D12_SHADER_BYTECODE meshShader = { meshShaderBlob_->GetBufferPointer(), meshShaderBlob_->GetBufferSize() };
	D3D12_SHADER_BYTECODE pixelShader = { pixelShaderBlob_->GetBufferPointer(), pixelShaderBlob_->GetBufferSize() };

	// Rasterizer 設定
	const D3D12_RASTERIZER_DESC rasterizerDesc = RasterizerStateSetting();
	const CD3DX12_RASTERIZER_DESC rast(rasterizerDesc);

	// Blend 設定ShadowMap用なので常に書き込みなし
	D3D12_BLEND_DESC blendDesc = {};
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	D3D12_RENDER_TARGET_BLEND_DESC defaultRenderTargetBlendDesc = {};
	defaultRenderTargetBlendDesc.RenderTargetWriteMask = 0;
	for (int j = 0; j < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; j++) {
		blendDesc.RenderTarget[j] = defaultRenderTargetBlendDesc;
	}
	const CD3DX12_BLEND_DESC blend(blendDesc);

	// DepthStencil 設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc = {};
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	depthStencilDesc.StencilEnable = FALSE;
	const CD3DX12_DEPTH_STENCIL_DESC ds(depthStencilDesc);

	// RenderTargetFormat
	D3D12_RT_FORMAT_ARRAY rtArray = {};
	rtArray.NumRenderTargets = 0;

	// Primitive Pipeline設定
	Primitive3DPipelineStateStream stream = {
		CD3DX12_PIPELINE_STATE_STREAM_ROOT_SIGNATURE(rootSignature_.Get()),
		CD3DX12_PIPELINE_STATE_STREAM_AS(amplificationShader),
		CD3DX12_PIPELINE_STATE_STREAM_MS(meshShader),
		CD3DX12_PIPELINE_STATE_STREAM_PS(pixelShader),
		CD3DX12_PIPELINE_STATE_STREAM_RASTERIZER(rast),
		CD3DX12_PIPELINE_STATE_STREAM_BLEND_DESC(blend),
		CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL(ds),
		CD3DX12_PIPELINE_STATE_STREAM_RENDER_TARGET_FORMATS(rtArray),
		CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL_FORMAT(dsvFormat)
	};

	D3D12_PIPELINE_STATE_STREAM_DESC streamDesc{};
	streamDesc.SizeInBytes = sizeof(stream);
	streamDesc.pPipelineStateSubobjectStream = &stream;

	HRESULT hr = dxgi_->GetDevice10()->CreatePipelineState(&streamDesc, IID_PPV_ARGS(&pipelineState_));
	if (FAILED(hr)) {
		Logger::Log("Failed to create ShadowMap PSO");
		assert(false);
	}
}

D3D12_DEPTH_STENCIL_DESC BaseShadowPipeline::DepthStecilDescSetting() {
	return D3D12_DEPTH_STENCIL_DESC();
}

D3D12_INPUT_LAYOUT_DESC BaseShadowPipeline::InputLayoutSetting() {
	return { nullptr, 0 };
}

D3D12_RASTERIZER_DESC BaseShadowPipeline::RasterizerStateSetting() {
	D3D12_RASTERIZER_DESC desc{};
	desc.FillMode = D3D12_FILL_MODE_SOLID;
	desc.CullMode = D3D12_CULL_MODE_NONE;
	desc.FrontCounterClockwise = FALSE;
	return desc;
}

void BaseShadowPipeline::SetDXGI(DXGI* dxgi) {
	assert(dxgi);
	dxgi_ = dxgi;
}

void BaseShadowPipeline::SetShaderCompiler(ShaderCompiler* shaderCompiler) {
	assert(shaderCompiler);
	shaderCompiler_ = shaderCompiler;
}
