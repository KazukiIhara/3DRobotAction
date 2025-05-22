#pragma once

#include "RenderTextures/BaseRenderTexture/BaseRenderTexture.h"


class ColorRenderTexture : public BaseRenderTexture {
public:
	ColorRenderTexture();
	~ColorRenderTexture()override;

	void Initialize();

private:

};