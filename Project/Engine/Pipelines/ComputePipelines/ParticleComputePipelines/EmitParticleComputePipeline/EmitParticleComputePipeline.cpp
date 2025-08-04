#include "EmitParticleComputePipeline.h"

#include <cassert>

#include "Logger/Logger.h"

#include "DirectX/DXGI/DXGI.h"
#include "DirectX/ShaderCompiler/ShaderCompiler.h"

EmitParticleComputePipeline::EmitParticleComputePipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler) {
	Initialize(dxgi, shaderCompiler);
}

void EmitParticleComputePipeline::Initialize(DXGI* dxgi, ShaderCompiler* shaderCompiler) {
	SetDXGI(dxgi);
	SetShaderCompiler(shaderCompiler);

	CreateRootSignature();
	CompileShaders();
	CreatePipelineStateObject();
}

ID3D12RootSignature* EmitParticleComputePipeline::GetRootSignature() {
	return rootSignature_.Get();
}

ID3D12PipelineState* EmitParticleComputePipeline::GetPipelineState() {
	return pipelineState_.Get();
}

void EmitParticleComputePipeline::CreateRootSignature() {
	HRESULT hr = S_FALSE;

	// SRV Range
	D3D12_DESCRIPTOR_RANGE emitParticleSrv[1]{};
	emitParticleSrv[0].NumDescriptors = 1;
	emitParticleSrv[0].BaseShaderRegister = 0;
	emitParticleSrv[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	emitParticleSrv[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// UAV Range
	D3D12_DESCRIPTOR_RANGE particleUav[1] = {};
	particleUav[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
	particleUav[0].NumDescriptors = 1;
	particleUav[0].BaseShaderRegister = 0;
	particleUav[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// Root Parameters
	D3D12_ROOT_PARAMETER rootParams[3] = {};

	// Particle用
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[0].DescriptorTable.NumDescriptorRanges = _countof(particleUav);
	rootParams[0].DescriptorTable.pDescriptorRanges = particleUav;
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	// EmitParticle用
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[1].DescriptorTable.NumDescriptorRanges = _countof(emitParticleSrv);
	rootParams[1].DescriptorTable.pDescriptorRanges = emitParticleSrv;
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	// EmitCount用
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParams[2].Descriptor.ShaderRegister = 0;

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

void EmitParticleComputePipeline::CompileShaders() {
	computeShaderBlob_ = nullptr;
	computeShaderBlob_ = shaderCompiler_->CompileShader(L"EngineAssets/Shaders/Compute/ParticleEffect/EmitParticleEffect.CS.hlsl", L"cs_6_0");
	assert(computeShaderBlob_ != nullptr);
}

void EmitParticleComputePipeline::CreatePipelineStateObject() {
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

void EmitParticleComputePipeline::SetDXGI(DXGI* dxgi) {
	assert(dxgi);
	dxgi_ = dxgi;
}

void EmitParticleComputePipeline::SetShaderCompiler(ShaderCompiler* shaderCompiler) {
	assert(shaderCompiler);
	shaderCompiler_ = shaderCompiler;
}
