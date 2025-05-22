#pragma once
#include "RenderTextures/BaseRenderTexture/BaseRenderTexture.h"

class GBufferAlbedoRenderTexture : public BaseRenderTexture {
public:
    GBufferAlbedoRenderTexture();
    ~GBufferAlbedoRenderTexture() override;

    void Initialize();
};