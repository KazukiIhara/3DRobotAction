#pragma once
#include "ResourceTextures/RenderTextures/BaseRenderTexture/BaseRenderTexture.h"

class GBufferAlbedoRenderTexture : public BaseRenderTexture {
public:
    GBufferAlbedoRenderTexture();
    ~GBufferAlbedoRenderTexture() override;

    void Initialize();
};