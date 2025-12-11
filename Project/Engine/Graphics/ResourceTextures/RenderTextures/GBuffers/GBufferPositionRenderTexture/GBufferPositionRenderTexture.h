#pragma once
#include "ResourceTextures/RenderTextures/BaseRenderTexture/BaseRenderTexture.h"


namespace magi {
class GBufferPositionRenderTexture : public BaseRenderTexture {
public:
    GBufferPositionRenderTexture();
    ~GBufferPositionRenderTexture() override;

    void Initialize();
};
}
