#include "SkinningComputePipeline.h"

#include <cassert>

#include "Logger/Logger.h"

#include "DirectX/DXGI/DXGI.h"
#include "DirectX/ShaderCompiler/ShaderCompiler.h"

SkinningComputePipeline::SkinningComputePipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler) {
	Initialize(dxgi, shaderCompiler);
}

SkinningComputePipeline::~SkinningComputePipeline() {}

void SkinningComputePipeline::Initialize(DXGI* dxgi, ShaderCompiler* shaderCompiler) {
	SetDXGI(dxgi);
	SetShaderCompiler(shaderCompiler);

	CreateRootSignature();
	CompileShaders();
	CreatePipelineStateObject();
}

ID3D12RootSignature* SkinningComputePipeline::GetRootSignature() {
	return rootSignature_.Get();
}

ID3D12PipelineState* SkinningComputePipeline::GetPipelineState() {
	return pipelineState_.Get();
}

void SkinningComputePipeline::CreateRootSignature() {
	HRESULT hr = S_FALSE;

	// SRV Descriptor Ranges (それぞれt0, t1, t2を割り当てる)
	D3D12_DESCRIPTOR_RANGE srvRanges0[1] = {};
	srvRanges0[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	srvRanges0[0].NumDescriptors = 1;              // t0
	srvRanges0[0].BaseShaderRegister = 0;          // t0から始まる
	srvRanges0[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE srvRanges1[1] = {};
	srvRanges1[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	srvRanges1[0].NumDescriptors = 1;              // t1
	srvRanges1[0].BaseShaderRegister = 1;          // t1から始まる
	srvRanges1[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE srvRanges2[1] = {};
	srvRanges2[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	srvRanges2[0].NumDescriptors = 1;              // t2
	srvRanges2[0].BaseShaderRegister = 2;          // t2から始まる
	srvRanges2[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// UAV Range
	D3D12_DESCRIPTOR_RANGE uavRanges[1] = {};
	uavRanges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
	uavRanges[0].NumDescriptors = 1;                // u0のみ
	uavRanges[0].BaseShaderRegister = 0;            // u0から始まる
	uavRanges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// Root Parameters
	D3D12_ROOT_PARAMETER rootParams[5] = {};

	// SRV Descriptor Table用パラメータ (t0)
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[0].DescriptorTable.NumDescriptorRanges = _countof(srvRanges0);
	rootParams[0].DescriptorTable.pDescriptorRanges = srvRanges0;
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	// SRV Descriptor Table用パラメータ (t1)
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[1].DescriptorTable.NumDescriptorRanges = _countof(srvRanges1);
	rootParams[1].DescriptorTable.pDescriptorRanges = srvRanges1;
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	// SRV Descriptor Table用パラメータ (t2)
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[2].DescriptorTable.NumDescriptorRanges = _countof(srvRanges2);
	rootParams[2].DescriptorTable.pDescriptorRanges = srvRanges2;
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	// UAV Descriptor Table用パラメータ (u0)
	rootParams[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[3].DescriptorTable.NumDescriptorRanges = _countof(uavRanges);
	rootParams[3].DescriptorTable.pDescriptorRanges = uavRanges;
	rootParams[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	// CBV用パラメータ (b0)
	rootParams[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParams[4].Descriptor.ShaderRegister = 0; // b0
	rootParams[4].Descriptor.RegisterSpace = 0;
	rootParams[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	// Root Signature
	D3D12_ROOT_SIGNATURE_DESC rootSigDesc = {};
	rootSigDesc.NumParameters = _countof(rootParams);
	rootSigDesc.pParameters = rootParams;
	rootSigDesc.NumStaticSamplers = 0;
	rootSigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_NONE;

	// シリアライズ
	ID3DBlob* signatureBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	hr = D3D12SerializeRootSignature(&rootSigDesc,
		D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(hr)) {
		Logger::Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}

	// Root Signature作成
	hr = dxgi_->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature_));
	assert(SUCCEEDED(hr));
}

void SkinningComputePipeline::CompileShaders() {
	computeShaderBlob_ = nullptr;
	computeShaderBlob_ = shaderCompiler_->CompileShader(L"EngineAssets/Shaders/Compute/Skinning/Skinning.CS.hlsl", L"cs_6_0");
	assert(computeShaderBlob_ != nullptr);
}

void SkinningComputePipeline::CreatePipelineStateObject() {
	HRESULT hr;

	assert(rootSignature_);
	assert(computeShaderBlob_);

	D3D12_COMPUTE_PIPELINE_STATE_DESC computePipelineStateDesc{};
	computePipelineStateDesc.CS = {
		.pShaderBytecode = computeShaderBlob_->GetBufferPointer(),
		.BytecodeLength = computeShaderBlob_->GetBufferSize(),
	};
	computePipelineStateDesc.pRootSignature = rootSignature_.Get();
	pipelineState_ = nullptr;
	hr = dxgi_->GetDevice()->CreateComputePipelineState(&computePipelineStateDesc, IID_PPV_ARGS(&pipelineState_));
	assert(SUCCEEDED(hr));
}

void SkinningComputePipeline::SetDXGI(DXGI* dxgi) {
	assert(dxgi);
	dxgi_ = dxgi;
}

void SkinningComputePipeline::SetShaderCompiler(ShaderCompiler* shaderCompiler) {
	assert(shaderCompiler);
	shaderCompiler_ = shaderCompiler;
}
