#pragma once
#include "ResourceTextures/RenderTextures/BaseRenderTexture/BaseRenderTexture.h"

class GBufferPositionRenderTexture : public BaseRenderTexture {
public:
    GBufferPositionRenderTexture();
    ~GBufferPositionRenderTexture() override;

    void Initialize();
};
