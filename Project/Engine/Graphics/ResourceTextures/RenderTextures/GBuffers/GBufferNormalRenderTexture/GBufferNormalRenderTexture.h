#pragma once
#include "ResourceTextures/RenderTextures/BaseRenderTexture/BaseRenderTexture.h"

class GBufferNormalRenderTexture : public BaseRenderTexture {
public:
    GBufferNormalRenderTexture();
    ~GBufferNormalRenderTexture() override;

    void Initialize();
};

