#include "ModelShadowPipeline.h"

#include <cassert>

#include "Logger/Logger.h"
#include "DirectX/DXGI/DXGI.h"
#include "DirectX/ShaderCompiler/ShaderCompiler.h"

ModelShadowPipeline::ModelShadowPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler)
	:BaseShadowPipeline(dxgi, shaderCompiler) {
}

void ModelShadowPipeline::CreateRootSignature() {
	HRESULT hr;

	// Descriptor Ranges
	D3D12_DESCRIPTOR_RANGE descriptorRangeInstance{};
	descriptorRangeInstance.BaseShaderRegister = 0; // t0
	descriptorRangeInstance.NumDescriptors = 1;
	descriptorRangeInstance.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRangeInstance.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE descriptorRangeVertex{};
	descriptorRangeVertex.BaseShaderRegister = 1; // t1
	descriptorRangeVertex.NumDescriptors = 1;
	descriptorRangeVertex.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRangeVertex.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE descriptorRangeMeshlet{};
	descriptorRangeMeshlet.BaseShaderRegister = 2; // t2
	descriptorRangeMeshlet.NumDescriptors = 1;
	descriptorRangeMeshlet.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRangeMeshlet.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE descriptorRangePrimitive{};
	descriptorRangePrimitive.BaseShaderRegister = 4; // t4
	descriptorRangePrimitive.NumDescriptors = 1;
	descriptorRangePrimitive.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRangePrimitive.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// Root Parameters
	D3D12_ROOT_PARAMETER rootParams[7] = {};

	// b0 : Camera
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_MESH;
	rootParams[0].Descriptor.ShaderRegister = 0;

	// t0 : InstanceData
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_MESH;
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRangeInstance;
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;

	// t1 : VertexBuffer
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_MESH;
	rootParams[2].DescriptorTable.pDescriptorRanges = &descriptorRangeVertex;
	rootParams[2].DescriptorTable.NumDescriptorRanges = 1;

	// t2 : MeshletBuffer
	rootParams[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_MESH;
	rootParams[3].DescriptorTable.pDescriptorRanges = &descriptorRangeMeshlet;
	rootParams[3].DescriptorTable.NumDescriptorRanges = 1;

	// t3 : UniqueVertexIndices
	rootParams[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;
	rootParams[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_MESH;
	rootParams[4].Descriptor.ShaderRegister = 3;

	// t4 : PrimitiveIndices
	rootParams[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_MESH;
	rootParams[5].DescriptorTable.pDescriptorRanges = &descriptorRangePrimitive;
	rootParams[5].DescriptorTable.NumDescriptorRanges = 1;

	// b2 : MeshInfo
	rootParams[6].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	rootParams[6].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParams[6].Constants.Num32BitValues = 4;
	rootParams[6].Constants.ShaderRegister = 2;

	// --- Static Samplerなし ---

	// Root Signature Desc
	D3D12_ROOT_SIGNATURE_DESC rootSigDesc{};
	rootSigDesc.NumParameters = _countof(rootParams);
	rootSigDesc.pParameters = rootParams;
	rootSigDesc.NumStaticSamplers = 0; // ← StaticSamplerなし
	rootSigDesc.pStaticSamplers = nullptr;
	rootSigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

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

void ModelShadowPipeline::CompileShaders() {
	amplificationShaderBlob_ = shaderCompiler_->CompileShader(L"EngineAssets/Shaders/Graphics/Model3D/Model3D.AS.hlsl", L"as_6_5");
	assert(amplificationShaderBlob_);

	meshShaderBlob_ = shaderCompiler_->CompileShader(L"EngineAssets/Shaders/Graphics/Model3D/Model3DShadow.MS.hlsl", L"ms_6_5");
	assert(meshShaderBlob_);

	pixelShaderBlob_ = shaderCompiler_->CompileShader(L"EngineAssets/Shaders/Graphics/Model3D/Model3DShadow.PS.hlsl", L"ps_6_5");
	assert(pixelShaderBlob_);
}
