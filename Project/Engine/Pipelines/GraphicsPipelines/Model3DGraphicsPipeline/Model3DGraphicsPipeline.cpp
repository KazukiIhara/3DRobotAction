#include "Model3DGraphicsPipeline.h"

#include <cassert>

#include "Logger/Logger.h"
#include "DirectX/DXGI/DXGI.h"
#include "DirectX/ShaderCompiler/ShaderCompiler.h"

Model3DGraphicsPipeline::Model3DGraphicsPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler)
	: BaseGraphicsPipeline(dxgi, shaderCompiler) {

}

void Model3DGraphicsPipeline::CreateRootSignature() {
	HRESULT hr;

	// Descriptor Ranges
	D3D12_DESCRIPTOR_RANGE descriptorRangeInstance{};
	descriptorRangeInstance.BaseShaderRegister = 0;
	descriptorRangeInstance.NumDescriptors = 1;
	descriptorRangeInstance.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRangeInstance.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE descriptorRangeTextures{};
	descriptorRangeTextures.BaseShaderRegister = 1000;
	descriptorRangeTextures.NumDescriptors = 1024;
	descriptorRangeTextures.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRangeTextures.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE descriptorRangeVertex{};
	descriptorRangeVertex.BaseShaderRegister = 1;
	descriptorRangeVertex.NumDescriptors = 1;
	descriptorRangeVertex.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRangeVertex.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE descriptorRangeMeshlet{};
	descriptorRangeMeshlet.BaseShaderRegister = 2;
	descriptorRangeMeshlet.NumDescriptors = 1;
	descriptorRangeMeshlet.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRangeMeshlet.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE rangePrim{};
	rangePrim.BaseShaderRegister = 4;
	rangePrim.NumDescriptors = 1;
	rangePrim.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	rangePrim.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE rangeCullData{};
	rangeCullData.BaseShaderRegister = 5;
	rangeCullData.NumDescriptors = 1;
	rangeCullData.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	rangeCullData.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// Root Parameters
	D3D12_ROOT_PARAMETER rootParams[11] = {};

	// b0 : Camera
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParams[0].Descriptor.ShaderRegister = 0;

	// t0 : InstanceData
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRangeInstance;
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;

	// b1 : MaterialData
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParams[2].Descriptor.ShaderRegister = 1;

	// t1000 : Textures
	rootParams[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParams[3].DescriptorTable.pDescriptorRanges = &descriptorRangeTextures;
	rootParams[3].DescriptorTable.NumDescriptorRanges = 1;

	// t1 : VertexBuffer
	rootParams[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_MESH;
	rootParams[4].DescriptorTable.pDescriptorRanges = &descriptorRangeVertex;
	rootParams[4].DescriptorTable.NumDescriptorRanges = 1;

	// t2 : MeshletBuffer
	rootParams[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParams[5].DescriptorTable.pDescriptorRanges = &descriptorRangeMeshlet;
	rootParams[5].DescriptorTable.NumDescriptorRanges = 1;

	// t3 unique
	rootParams[6].ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;
	rootParams[6].ShaderVisibility = D3D12_SHADER_VISIBILITY_MESH;
	rootParams[6].Descriptor.ShaderRegister = 3;

	// t4 prim
	rootParams[7].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[7].ShaderVisibility = D3D12_SHADER_VISIBILITY_MESH;
	rootParams[7].DescriptorTable.pDescriptorRanges = &rangePrim;
	rootParams[7].DescriptorTable.NumDescriptorRanges = 1;

	// t5 CullData
	rootParams[8].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[8].ShaderVisibility = D3D12_SHADER_VISIBILITY_AMPLIFICATION;
	rootParams[8].DescriptorTable.pDescriptorRanges = &rangeCullData;
	rootParams[8].DescriptorTable.NumDescriptorRanges = 1;

	// b2 MeshInfo
	rootParams[9].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	rootParams[9].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParams[9].Constants.Num32BitValues = 4;
	rootParams[9].Constants.ShaderRegister = 2;

	// b3 Frustum
	rootParams[10].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParams[10].ShaderVisibility = D3D12_SHADER_VISIBILITY_AMPLIFICATION;
	rootParams[10].Descriptor.ShaderRegister = 3;

	// Static Sampler
	D3D12_STATIC_SAMPLER_DESC staticSampler{};
	staticSampler.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	staticSampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	staticSampler.MaxLOD = D3D12_FLOAT32_MAX;
	staticSampler.ShaderRegister = 0;
	staticSampler.RegisterSpace = 0;
	staticSampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	// Root Signature
	D3D12_ROOT_SIGNATURE_DESC rootSigDesc{};
	rootSigDesc.NumParameters = _countof(rootParams);
	rootSigDesc.pParameters = rootParams;
	rootSigDesc.NumStaticSamplers = 1;
	rootSigDesc.pStaticSamplers = &staticSampler;
	rootSigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_NONE;

	ComPtr<ID3DBlob> sigBlob;
	ComPtr<ID3DBlob> errorBlob;
	hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &sigBlob, &errorBlob);
	if (FAILED(hr)) {
		if (errorBlob) {
			Logger::Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		}
		assert(false);
	}

	hr = dxgi_->GetDevice()->CreateRootSignature(0, sigBlob->GetBufferPointer(), sigBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature_));
	assert(SUCCEEDED(hr));
}

void Model3DGraphicsPipeline::CompileShaders() {
	amplificationShaderBlob_ = shaderCompiler_->CompileShader(L"EngineAssets/Shaders/Graphics/Model3D/Model3D.AS.hlsl", L"as_6_5");
	assert(amplificationShaderBlob_);

	meshShaderBlob_ = shaderCompiler_->CompileShader(L"EngineAssets/Shaders/Graphics/Model3D/Model3D.MS.hlsl", L"ms_6_5");
	assert(meshShaderBlob_);

	pixelShaderBlob_ = shaderCompiler_->CompileShader(L"EngineAssets/Shaders/Graphics/Model3D/Model3D.PS.hlsl", L"ps_6_5");
	assert(pixelShaderBlob_);

	meshShaderBlobWithAlpha_ = shaderCompiler_->CompileShader(L"EngineAssets/Shaders/Graphics/Model3D/Model3DWithAlpha.MS.hlsl", L"ms_6_5");
	assert(meshShaderBlobWithAlpha_);

	pixelShaderBlobWithAlpha_ = shaderCompiler_->CompileShader(L"EngineAssets/Shaders/Graphics/Model3D/Model3DWithAlpha.PS.hlsl", L"ps_6_5");
	assert(pixelShaderBlobWithAlpha_);
}

D3D12_BLEND_DESC Model3DGraphicsPipeline::BlendStateSetting(uint32_t blendModeNum) {
	D3D12_BLEND_DESC blendDesc{};
	switch (blendModeNum) {
		case 0:// kBlendModeNone
			blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			break;
		case 1:// kBlendModeNormal
			blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			blendDesc.RenderTarget[0].BlendEnable = TRUE;
			blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
			blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
			blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ZERO;
			blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ONE;
			break;

		case 2:// kBlendModeAdd
			blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			blendDesc.RenderTarget[0].BlendEnable = TRUE;
			blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
			blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
			blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ZERO;
			blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ONE;
			break;
		case 3:// kBlendModeSubtract
			blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			blendDesc.RenderTarget[0].BlendEnable = TRUE;
			blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
			blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
			blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
			blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ZERO;
			blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ONE;
			break;

		case 4:// kBlendModeMultiply
			blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			blendDesc.RenderTarget[0].BlendEnable = TRUE;
			blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ZERO;
			blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_SRC_COLOR;
			blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ZERO;
			blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ONE;
			break;

		case 5:// kBlendModeScreen
			blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			blendDesc.RenderTarget[0].BlendEnable = TRUE;
			blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
			blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
			blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ZERO;
			blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ONE;
			break;
	}
	// 全ての色要素を書き込む
	// ブレンドモードNone D3D12_COLOR_WRITE_ENABLE_ALLだけ

	return blendDesc;
}

D3D12_DEPTH_STENCIL_DESC Model3DGraphicsPipeline::DepthStecilDescSetting() {
	return D3D12_DEPTH_STENCIL_DESC();
}

D3D12_INPUT_LAYOUT_DESC Model3DGraphicsPipeline::InputLayoutSetting() {
	return { nullptr, 0 };
}

D3D12_RASTERIZER_DESC Model3DGraphicsPipeline::RasterizerStateSetting() {
	D3D12_RASTERIZER_DESC desc{};
	desc.FillMode = D3D12_FILL_MODE_SOLID;
	desc.CullMode = D3D12_CULL_MODE_BACK;
	desc.FrontCounterClockwise = FALSE;
	return desc;
}