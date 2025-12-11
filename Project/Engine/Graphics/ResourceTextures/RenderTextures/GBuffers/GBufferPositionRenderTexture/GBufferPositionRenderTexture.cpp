#include "GBufferPositionRenderTexture.h"

GBufferPositionRenderTexture::GBufferPositionRenderTexture() {
}

GBufferPositionRenderTexture::~GBufferPositionRenderTexture() {
}

void GBufferPositionRenderTexture::Initialize() {
	BaseRenderTexture::Create(
		DXGI_FORMAT_R32G32B32A32_FLOAT,                    // 位置は超高精度が必要
		D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET,
		Vector4(0.0f, 0.0f, 0.0f, 1.0f)                     // 空の位置（ゼロ初期化）
	);
}
