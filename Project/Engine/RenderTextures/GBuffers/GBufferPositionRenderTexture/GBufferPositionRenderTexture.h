#pragma once
#include "RenderTextures/BaseRenderTexture/BaseRenderTexture.h"

class GBufferPositionRenderTexture : public BaseRenderTexture {
public:
    GBufferPositionRenderTexture();
    ~GBufferPositionRenderTexture() override;

    void Initialize();
};
