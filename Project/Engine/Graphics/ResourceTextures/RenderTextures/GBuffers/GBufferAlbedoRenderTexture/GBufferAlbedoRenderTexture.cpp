#include "GBufferAlbedoRenderTexture.h"

GBufferAlbedoRenderTexture::GBufferAlbedoRenderTexture() {
}

GBufferAlbedoRenderTexture::~GBufferAlbedoRenderTexture() {
}

void GBufferAlbedoRenderTexture::Initialize() {
	BaseRenderTexture::Create(
		DXGI_FORMAT_R8G8B8A8_UNORM,                        // アルベドは普通の色なのでこれでOK
		D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET,
		Vector4(0.0f, 0.0f, 0.0f, 1.0f)                     // 黒クリア
	);
}
