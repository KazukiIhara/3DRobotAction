#include "DXGI.h"

#include <cassert>

#include "Logger/Logger.h"

DXGI::DXGI() {}

DXGI::~DXGI() {
	Logger::Log("DXGI Finalize\n");
}

bool DXGI::Initialize() {
#ifdef _DEBUG
	ComPtr<ID3D12Debug1> debugController = nullptr;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		// デバッグコントローラを有効化する
		debugController->EnableDebugLayer();
		// さらにGPU側でもチェックを行うようにする
		debugController->SetEnableGPUBasedValidation(TRUE);
	}
#endif // _DEBUG

	// DXGIファクトリーの生成
	hr_ = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory_));
	assert(SUCCEEDED(hr_));

	// 使用するアダプタ(GPU)を決定する
	// 良い順にアダプタを積む
	for (UINT i = 0; dxgiFactory_->EnumAdapterByGpuPreference(i,
		DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
		IID_PPV_ARGS(&useAdapter_)) != DXGI_ERROR_NOT_FOUND; ++i) {
		// アダプターの情報を取得する
		DXGI_ADAPTER_DESC3 adapterDesc{};
		hr_ = useAdapter_->GetDesc3(&adapterDesc);
		assert(SUCCEEDED(hr_));

		// ソフトウェアアダプタでなければ採用
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			// 採用したアダプタの情報をログに出力。
			Logger::Log(Logger::ConvertString(std::format(L"Use Adapter : {}\n", adapterDesc.Description)));
			break;
		}
		useAdapter_ = nullptr; // ソフトウェアアダプタの場合は見なかったことにする
	}
	// 適切なアダプタが見つからなかったので起動できない
	assert(useAdapter_ != nullptr);

	// 機能レベルとログ出力用の文字列
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_12_2,D3D_FEATURE_LEVEL_12_1,D3D_FEATURE_LEVEL_12_0
	};
	const char* featureLevelStrings[] = { "12.2","12.1","12.0" };
	// 高い順に生成できるか試していく
	for (size_t i = 0; i < _countof(featureLevels); ++i) {
		// 採用したアダプターでデバイスを生成
		hr_ = D3D12CreateDevice(useAdapter_.Get(), featureLevels[i], IID_PPV_ARGS(&device_));
		// 指定した機能レベルでデバイスが生成出来たかを確認
		if (SUCCEEDED(hr_)) {
			// 生成できたのでログ出力を行ってループを抜ける
			Logger::Log(std::format("featureLevel : {}\n", featureLevelStrings[i]));
			break;
		}
	}
	// デバイスの生成がうまくいかなかったので起動できない
	assert(device_ != nullptr);

	// DirectX12Ultimateに対応しているかどうかをチェック
	bool isSupportDirectX12Ultimate = false;
	{
		D3D12_FEATURE_DATA_D3D12_OPTIONS7 options7{};
		if (SUCCEEDED(device_->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS7, &options7, sizeof(options7)))) {
			if (options7.MeshShaderTier >= D3D12_MESH_SHADER_TIER_1) {
				isSupportDirectX12Ultimate = true;
				Logger::Log("DirectX12 Ultimate (MeshShaderTier1以上) に対応しています。\n");
			} else {
				Logger::Log("MeshShaderTierが非対応、DirectX12 Ultimate 非対応です。\n");

			}
		} else {
			Logger::Log("D3D12_OPTIONS7の取得に失敗しました。\n");
		}
	}

	// サポートしているなら
	if (isSupportDirectX12Ultimate) {
		// MeshShader対応のデバイスを取得
		hr_ = device_->QueryInterface(IID_PPV_ARGS(&device10_));
		if (SUCCEEDED(hr_)) {
			Logger::Log("Device10を取得しました。\n");
		} else {
			// 念のためnullptrに
			device10_ = nullptr;
			isSupportDirectX12Ultimate = false;
			Logger::Log("Device10の取得に失敗しました。MeshShaderはサポートされていますが、ドライバまたはOSが古い可能性があります。\n");

			MessageBox(
				nullptr,
				L"DirectX12 Ultimate 非対応です。\nお使いのハードウェアは本アプリケーションの動作要件を満たしていません。\n"
				L"アプリケーションを終了します。",
				L"サポート外のデバイス",
				MB_OK | MB_ICONERROR
			);
			CoUninitialize();
			Logger::Finalize();
			ExitProcess(EXIT_FAILURE);
		}
	} else {
		Logger::Log("MeshShader未対応GPUです。Device10の取得をスキップしました。\n");

		MessageBox(
			nullptr,
			L"DirectX12 Ultimate 非対応です。\nお使いのハードウェアは本アプリケーションの動作要件を満たしていません。\n"
			L"アプリケーションを終了します。",
			L"サポート外のデバイス",
			MB_OK | MB_ICONERROR
		);
		CoUninitialize();
		Logger::Finalize();
		ExitProcess(EXIT_FAILURE);
	}

	// 初期化完了ログ
	Logger::Log("Complete Create D3D12Device\n");

#ifdef _DEBUG
	ComPtr<ID3D12InfoQueue> infoQueue = nullptr;
	if (SUCCEEDED(device_->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
		// やばいエラー時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		// エラー時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
		// 警告時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);

		// 抑制するメッセージのID
		D3D12_MESSAGE_ID denyIds[] = {
			// Windows11でのDXGiデバッグレイヤーとDX12デバッグレイヤーの相互作用バグによるエラーメッセージ
			// https://stackoverflow.com/questions/69805245/directx-12-application-is-crashing-in-windows11
			D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
		};
		// 抑制するレベル
		D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
		D3D12_INFO_QUEUE_FILTER filter{};
		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;
		// 指定したメッセージの表示を抑制する
		infoQueue->PushStorageFilter(&filter);
	}
#endif // _DEBUG

	// 初期化完了
	Logger::Log("DXGI Initialize\n");

	return isSupportDirectX12Ultimate;
}

ComPtr<ID3D12DescriptorHeap> DXGI::CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible) {
	// ディスクリプタヒープの生成
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc{};
	descriptorHeapDesc.Type = heapType;
	descriptorHeapDesc.NumDescriptors = numDescriptors;
	descriptorHeapDesc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	hr_ = device_->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&descriptorHeap));
	// ディスクリプターヒープが作れなかったので起動できない
	assert(SUCCEEDED(hr_));
	return descriptorHeap;
}

ComPtr<ID3D12Resource> DXGI::CreateBufferResource(size_t sizeInBytes, bool isforUAV) {
	if (!isforUAV) {
		hr_ = S_FALSE;
		// 頂点リソース用のヒープの設定
		D3D12_HEAP_PROPERTIES uplodeHeapProperties{};
		uplodeHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;//UploadHeapを使う

		// マテリアル用のリソースの設定
		D3D12_RESOURCE_DESC resourceDesc{};
		// バッファリソース。テクスチャの場合はまた別の設定をする
		resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resourceDesc.Width = sizeInBytes;
		// バッファの場合はこれらは1にする決まり
		resourceDesc.Height = 1;
		resourceDesc.DepthOrArraySize = 1;
		resourceDesc.MipLevels = 1;
		resourceDesc.SampleDesc.Count = 1;
		// バッファの場合はこれにする決まり
		resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		// バッファリソースを作る
		Microsoft::WRL::ComPtr<ID3D12Resource>resource = nullptr;
		hr_ = device_->CreateCommittedResource(
			&uplodeHeapProperties,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&resource)
		);
		assert(SUCCEEDED(hr_));

		return resource;
	} else {

		hr_ = S_FALSE;

		// UAVリソース用ヒーププロパティ(一般的なGPU上のデフォルトヒープ)
		D3D12_HEAP_PROPERTIES defaultHeapProperties{};
		defaultHeapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

		// UAV用バッファリソース設定
		D3D12_RESOURCE_DESC resourceDesc{};
		resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resourceDesc.Width = sizeInBytes;
		resourceDesc.Height = 1;
		resourceDesc.DepthOrArraySize = 1;
		resourceDesc.MipLevels = 1;
		resourceDesc.SampleDesc.Count = 1;
		resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		// UAVを許可するフラグを設定
		resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

		// UAV用リソースを作成
		Microsoft::WRL::ComPtr<ID3D12Resource> resource = nullptr;
		hr_ = device_->CreateCommittedResource(
			&defaultHeapProperties,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			D3D12_RESOURCE_STATE_COMMON,
			nullptr,
			IID_PPV_ARGS(&resource)
		);
		assert(SUCCEEDED(hr_));

		return resource;
	}
}

ComPtr<ID3D12Resource> DXGI::CreateDepthStencilTextureResource(int32_t width, int32_t height, DXGI_FORMAT format) {
	// 生成するResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = width;
	resourceDesc.Height = height;
	resourceDesc.MipLevels = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.Format = format;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	// 利用するHeapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

	// 深度値のクリア設定
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;
	depthClearValue.Format = format;

	// Resourceの生成
	Microsoft::WRL::ComPtr<ID3D12Resource> resource = nullptr;
	hr_ = device_->CreateCommittedResource(
		&heapProperties,// Heapの設定
		D3D12_HEAP_FLAG_NONE,// Heapの特殊な設定。特になし。
		&resourceDesc,	// Resourceの設定
		D3D12_RESOURCE_STATE_DEPTH_WRITE,// 深度地を書き込む状態にしておく
		&depthClearValue,// Clear最適値
		IID_PPV_ARGS(&resource));// 作成するResourceポインタへのポインタ
	assert(SUCCEEDED(hr_));

	return resource;
}

ComPtr<ID3D12Resource> DXGI::CreateDepthStencilTextureResource(int32_t width, int32_t height, DXGI_FORMAT format, DXGI_FORMAT clearFormat) {
	// 生成するResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = width;
	resourceDesc.Height = height;
	resourceDesc.MipLevels = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.Format = format;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	// 利用するHeapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

	// 深度値のクリア設定
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;
	depthClearValue.DepthStencil.Stencil = 0;
	depthClearValue.Format = clearFormat;

	// Resourceの生成
	Microsoft::WRL::ComPtr<ID3D12Resource> resource = nullptr;
	hr_ = device_->CreateCommittedResource(
		&heapProperties,// Heapの設定
		D3D12_HEAP_FLAG_NONE,// Heapの特殊な設定。特になし。
		&resourceDesc,	// Resourceの設定
		D3D12_RESOURCE_STATE_DEPTH_WRITE,// 深度地を書き込む状態にしておく
		&depthClearValue,// Clear最適値
		IID_PPV_ARGS(&resource));// 作成するResourceポインタへのポインタ
	assert(SUCCEEDED(hr_));

	return resource;
}

ID3D12Device* DXGI::GetDevice() {
	return device_.Get();
}

ID3D12Device10* DXGI::GetDevice10() {
	return device10_.Get();
}

IDXGIFactory7* DXGI::GetFactory() {
	return dxgiFactory_.Get();
}
