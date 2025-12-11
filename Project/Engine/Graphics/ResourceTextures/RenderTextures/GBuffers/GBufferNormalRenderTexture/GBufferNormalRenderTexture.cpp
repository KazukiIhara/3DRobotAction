#include "GBufferNormalRenderTexture.h"

GBufferNormalRenderTexture::GBufferNormalRenderTexture() {
}

GBufferNormalRenderTexture::~GBufferNormalRenderTexture() {
}

void GBufferNormalRenderTexture::Initialize() {
	BaseRenderTexture::Create(
		DXGI_FORMAT_R16G16B16A16_FLOAT,                    // 法線は高精度なfloat4にする
		D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET,
		Vector4(0.5f, 0.5f, 1.0f, 1.0f)                     // 法線マップは通常 Z+方向(0.5,0.5,1)がデフォ
	);
}
