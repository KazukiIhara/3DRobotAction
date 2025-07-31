#include "InitParticleComputePipeline.h"

#include <cassert>

#include "Logger/Logger.h"

#include "DirectX/DXGI/DXGI.h"
#include "DirectX/ShaderCompiler/ShaderCompiler.h"

InitParticleComputePipeline::InitParticleComputePipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler) {
	Initialize(dxgi, shaderCompiler);
}

void InitParticleComputePipeline::Initialize(DXGI* dxgi, ShaderCompiler* shaderCompiler) {
	SetDXGI(dxgi);
	SetShaderCompiler(shaderCompiler);

	CreateRootSignature();
	CompileShaders();
	CreatePipelineStateObject();
}

ID3D12RootSignature* InitParticleComputePipeline::GetRootSignature() {
	return rootSignature_.Get();
}

ID3D12PipelineState* InitParticleComputePipeline::GetPipelineState() {
	return pipelineState_.Get();
}

void InitParticleComputePipeline::CreateRootSignature() {
	HRESULT hr = S_FALSE;

	// SRV Descriptor Ranges
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
	D3D12_ROOT_PARAMETER rootParams[1] = {};



	
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

void InitParticleComputePipeline::CompileShaders() {
	computeShaderBlob_ = nullptr;
	computeShaderBlob_ = shaderCompiler_->CompileShader(L"EngineAssets/Shaders/Compute/Skinning/Skinning.CS.hlsl", L"cs_6_0");
	assert(computeShaderBlob_ != nullptr);
}

void InitParticleComputePipeline::CreatePipelineStateObject() {
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

void InitParticleComputePipeline::SetDXGI(DXGI* dxgi) {
	assert(dxgi);
	dxgi_ = dxgi;
}

void InitParticleComputePipeline::SetShaderCompiler(ShaderCompiler* shaderCompiler) {
	assert(shaderCompiler);
	shaderCompiler_ = shaderCompiler;
}
