#pragma once

#include "ResourceTextures/RenderTextures/BaseRenderTexture/BaseRenderTexture.h"




namespace magi {
class ColorRenderTexture : public BaseRenderTexture {
public:
	ColorRenderTexture();
	~ColorRenderTexture()override;

	void Initialize();

private:

};

} // namespace magi
