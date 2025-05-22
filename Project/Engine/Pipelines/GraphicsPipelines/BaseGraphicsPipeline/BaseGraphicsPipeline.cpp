#include "BaseGraphicsPipeline.h"

#include <cassert>

#include "Logger/Logger.h"
#include "DirectX/DXGI/DXGI.h"
#include "DirectX/ShaderCompiler/ShaderCompiler.h"

BaseGraphicsPipeline::BaseGraphicsPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler) {
	SetDXGI(dxgi);
	SetShaderCompiler(shaderCompiler);
}

BaseGraphicsPipeline::~BaseGraphicsPipeline() {
}

void BaseGraphicsPipeline::Initialize() {
	CreateRootSignature();
	CompileShaders();
	CreateGraphicsPipelineObject();
}

ID3D12RootSignature* BaseGraphicsPipeline::GetRootSignature() {
	return rootSignature_.Get();
}

ID3D12PipelineState* BaseGraphicsPipeline::GetPipelineState(BlendMode blendMode) {
	return pipelineState_[static_cast<uint32_t>(blendMode)].Get();
}

void BaseGraphicsPipeline::CreateGraphicsPipelineObject() {
	assert(rootSignature_);
	assert(meshShaderBlob_);
	assert(pixelShaderBlob_);
	assert(amplificationShaderBlob_);

	assert(meshShaderBlobWithAlpha_);
	assert(pixelShaderBlobWithAlpha_);


	const DXGI_FORMAT dsvFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	for (uint32_t i = 0; i < kBlendModeNum; ++i) {
		const BlendMode mode = static_cast<BlendMode>(i);

		D3D12_SHADER_BYTECODE meshShader;
		D3D12_SHADER_BYTECODE pixelShader;
		D3D12_RT_FORMAT_ARRAY rtArray{};

		switch (mode) {
		case BlendMode::Normal:
		case BlendMode::Add:
		case BlendMode::Subtract:
		case BlendMode::Multiply:
		case BlendMode::Screen:
			meshShader = { meshShaderBlobWithAlpha_->GetBufferPointer(),  meshShaderBlobWithAlpha_->GetBufferSize() };
			pixelShader = { pixelShaderBlobWithAlpha_->GetBufferPointer(), pixelShaderBlobWithAlpha_->GetBufferSize() };

			rtArray.NumRenderTargets = 1;
			rtArray.RTFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;       // Albedo
			break;
		default:
			meshShader = { meshShaderBlob_->GetBufferPointer(), meshShaderBlob_->GetBufferSize() };
			pixelShader = { pixelShaderBlob_->GetBufferPointer(), pixelShaderBlob_->GetBufferSize() };

			rtArray.NumRenderTargets = 3;
			rtArray.RTFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;       // Albedo
			rtArray.RTFormats[1] = DXGI_FORMAT_R16G16B16A16_FLOAT;        // Normal
			rtArray.RTFormats[2] = DXGI_FORMAT_R16G16B16A16_FLOAT;        // Position
			break;
		}

		const D3D12_SHADER_BYTECODE amplificationShader = { amplificationShaderBlob_->GetBufferPointer(), amplificationShaderBlob_->GetBufferSize() };

		const D3D12_RASTERIZER_DESC rasterizerDesc = RasterizerStateSetting();
		const D3D12_BLEND_DESC blendDesc = BlendStateSetting(i);
		const D3D12_DEPTH_STENCIL_DESC depthStencilDesc = DepthStecilDescSettingBlend(i);

		const CD3DX12_RASTERIZER_DESC rast(rasterizerDesc);
		const CD3DX12_BLEND_DESC blend(blendDesc);
		const CD3DX12_DEPTH_STENCIL_DESC ds(depthStencilDesc);


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

		HRESULT hr = dxgi_->GetDevice10()->CreatePipelineState(&streamDesc, IID_PPV_ARGS(&pipelineState_[i]));
		if (FAILED(hr)) {
			Logger::Log("Failed to create PSO for blendMode " + std::to_string(i));
			assert(false);
		}
	}

}

D3D12_DEPTH_STENCIL_DESC BaseGraphicsPipeline::DepthStecilDescSetting() {
	return D3D12_DEPTH_STENCIL_DESC();
}

D3D12_DEPTH_STENCIL_DESC BaseGraphicsPipeline::DepthStecilDescSettingBlend(uint32_t blendModeNum) {
	D3D12_DEPTH_STENCIL_DESC desc{};
	desc.DepthEnable = TRUE;

	const auto mode = static_cast<BlendMode>(blendModeNum);
	switch (mode) {
	case BlendMode::Normal:
	case BlendMode::Add:
	case BlendMode::Subtract:
	case BlendMode::Multiply:
	case BlendMode::Screen:
		desc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO; // ブレンド系は書き込まない
		break;
	default:
		desc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		break;
	}

	desc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	return desc;
}

void BaseGraphicsPipeline::SetDXGI(DXGI* dxgi) {
	assert(dxgi);
	dxgi_ = dxgi;
}

void BaseGraphicsPipeline::SetShaderCompiler(ShaderCompiler* shaderCompiler) {
	assert(shaderCompiler);
	shaderCompiler_ = shaderCompiler;
}
