#pragma once
#include "RenderTextures/BaseRenderTexture/BaseRenderTexture.h"

class GBufferNormalRenderTexture : public BaseRenderTexture {
public:
    GBufferNormalRenderTexture();
    ~GBufferNormalRenderTexture() override;

    void Initialize();
};

