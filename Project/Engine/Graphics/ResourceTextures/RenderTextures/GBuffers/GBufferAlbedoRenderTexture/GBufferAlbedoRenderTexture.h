#pragma once
#include "ResourceTextures/RenderTextures/BaseRenderTexture/BaseRenderTexture.h"


namespace magi {
class GBufferAlbedoRenderTexture : public BaseRenderTexture {
public:
    GBufferAlbedoRenderTexture();
    ~GBufferAlbedoRenderTexture() override;

    void Initialize();
};
}
