#include "Object3DGroupGraphicsPipeline.h"

#include <cassert>

#include "Logger/Logger.h"
#include "DirectX/DXGI/DXGI.h"
#include "DirectX/ShaderCompiler/ShaderCompiler.h"

Object3DGroupGraphicsPipeline::Object3DGroupGraphicsPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler)
	:BaseGraphicsPipeline(dxgi, shaderCompiler) {
}

Object3DGroupGraphicsPipeline::~Object3DGroupGraphicsPipeline() {
}

void Object3DGroupGraphicsPipeline::CreateRootSignature() {
	HRESULT hr = S_FALSE;

	// テクスチャ用
	D3D12_DESCRIPTOR_RANGE descriptorRangeTexture[1] = {};
	descriptorRangeTexture[0].BaseShaderRegister = 0; // t0
	descriptorRangeTexture[0].NumDescriptors = 1;
	descriptorRangeTexture[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRangeTexture[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// ライト用
	D3D12_DESCRIPTOR_RANGE descriptorRangeLights[1] = {};
	descriptorRangeLights[0].BaseShaderRegister = 1; // t1
	descriptorRangeLights[0].NumDescriptors = 1;
	descriptorRangeLights[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRangeLights[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// 法線マップ用
	D3D12_DESCRIPTOR_RANGE descriptorRangeNormal[1] = {};
	descriptorRangeNormal[0].BaseShaderRegister = 2; // t2
	descriptorRangeNormal[0].NumDescriptors = 1;
	descriptorRangeNormal[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRangeNormal[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// インスタンス描画用
	D3D12_DESCRIPTOR_RANGE instanceDataSRVRange[1] = {};
	instanceDataSRVRange[0].BaseShaderRegister = 3;  // t3
	instanceDataSRVRange[0].NumDescriptors = 1;
	instanceDataSRVRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	instanceDataSRVRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;


	// RootSignature作成
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// RootParameter作成。
	D3D12_ROOT_PARAMETER rootParameters[8] = {};
	// オブジェクトのマテリアル
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//CBVを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
	rootParameters[0].Descriptor.ShaderRegister = 0;					//レジスタ番号0とバインド

	// インスタンス描画用
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX; // 頂点シェーダだけ使えばOK
	rootParameters[1].DescriptorTable.pDescriptorRanges = instanceDataSRVRange;
	rootParameters[1].DescriptorTable.NumDescriptorRanges = 1;

	// カメラ
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//CBVを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダーで使える
	rootParameters[2].Descriptor.ShaderRegister = 1;					//レジスタ番号1とバインド

	// テクスチャ
	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[3].DescriptorTable.pDescriptorRanges = descriptorRangeTexture;
	rootParameters[3].DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeTexture);

	// モデルのマテリアル
	rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//CBVを使う
	rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
	rootParameters[4].Descriptor.ShaderRegister = 2;					//レジスタ番号2とバインド


	// 
	// ライトの総数と、ライトのデータを送るルートパラメータのインデックスは並べる。
	// 

	// ライト総数
	rootParameters[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[5].Descriptor.ShaderRegister = 3;  // b3

	// 複数ライト用
	rootParameters[6].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[6].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[6].DescriptorTable.pDescriptorRanges = descriptorRangeLights;
	rootParameters[6].DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeLights);

	// 法線マップ用
	rootParameters[7].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[7].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[7].DescriptorTable.pDescriptorRanges = descriptorRangeNormal;
	rootParameters[7].DescriptorTable.NumDescriptorRanges = 1;

	descriptionRootSignature.pParameters = rootParameters;				//ルートパラメータ配列へのポインタ
	descriptionRootSignature.NumParameters = _countof(rootParameters);	//配列の長さ

	// Samplerの設定
	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;			//バイリニアフィルタ
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;		//0~1の範囲外をリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;		//比較しない
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;						//ありったけのMipmapを使う
	staticSamplers[0].ShaderRegister = 0;								//レジスタ番号0を使う
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);

	// シリアライズしてバイナリにする
	ID3DBlob* signatureBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	hr = D3D12SerializeRootSignature(&descriptionRootSignature,
		D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(hr)) {
		Logger::Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}
	// バイナリをもとに生成
	rootSignature_ = nullptr;
	hr = dxgi_->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature_));
	assert(SUCCEEDED(hr));

}

void Object3DGroupGraphicsPipeline::CompileShaders() {
	vertexShaderBlob_ = nullptr;
	vertexShaderBlob_ = shaderCompiler_->CompileShader(L"EngineAssets/Shaders/Graphics/Object3DGroup/Object3DGroup.VS.hlsl", L"vs_6_0");
	assert(vertexShaderBlob_ != nullptr);

	pixelShaderBlob_ = nullptr;
	pixelShaderBlob_ = shaderCompiler_->CompileShader(L"EngineAssets/Shaders/Graphics/Object3DGroup/Object3DGroup.PS.hlsl", L"ps_6_0");
	assert(pixelShaderBlob_ != nullptr);
}

void Object3DGroupGraphicsPipeline::CreateGraphicsPipelineObject() {
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
	// 書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	// 利用するトポロジ(形状)のタイプ、三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType =
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// どのように画面に色を打ち込むかの設定(気にしなくて良い)
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	// DepthStencilの設定
	graphicsPipelineStateDesc.DepthStencilState = DepthStecilDescSetting();
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 実際に生成
	for (uint32_t i = 0; i < kBlendModeNum; i++) {
		graphicsPipelineStateDesc.BlendState = BlendStateSetting(i);
		pipelineState_[i] = nullptr;
		hr = dxgi_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
			IID_PPV_ARGS(&pipelineState_[i]));
		assert(SUCCEEDED(hr));
	}
}

D3D12_BLEND_DESC Object3DGroupGraphicsPipeline::BlendStateSetting(uint32_t blendModeNum) {
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

D3D12_DEPTH_STENCIL_DESC Object3DGroupGraphicsPipeline::DepthStecilDescSetting() {
	// DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	// Depthの機能を有効化する
	depthStencilDesc.DepthEnable = true;
	// 書き込みします
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	// 比較関数はLessEqual
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	return depthStencilDesc;
}

D3D12_INPUT_LAYOUT_DESC Object3DGroupGraphicsPipeline::InputLayoutSetting() {
	// InputLayout
	static D3D12_INPUT_ELEMENT_DESC inputElementDescs[4] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].InputSlot = 0;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[0].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
	inputElementDescs[0].InstanceDataStepRate = 0;

	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs[2].SemanticName = "NORMAL";
	inputElementDescs[2].SemanticIndex = 0;
	inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs[3].SemanticName = "TANGENT";
	inputElementDescs[3].SemanticIndex = 0;
	inputElementDescs[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescs[3].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);

	return inputLayoutDesc;
}

D3D12_RASTERIZER_DESC Object3DGroupGraphicsPipeline::RasterizerStateSetting() {
	// RasterizerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc_{};
	// 裏側(時計回り)を表示しない
	rasterizerDesc_.CullMode = D3D12_CULL_MODE_BACK;
	// 三角形の中を塗りつぶす
	rasterizerDesc_.FillMode = D3D12_FILL_MODE_SOLID;

	return rasterizerDesc_;
}
