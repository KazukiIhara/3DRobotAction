#pragma once
#include "ResourceTextures/RenderTextures/BaseRenderTexture/BaseRenderTexture.h"



namespace magi {
class GBufferNormalRenderTexture : public BaseRenderTexture {
public:
    GBufferNormalRenderTexture();
    ~GBufferNormalRenderTexture() override;

    void Initialize();
};

} // namespace magi
