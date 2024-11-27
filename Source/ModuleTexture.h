#pragma once
#include "Module.h"
#include "DirectXTex/DirectXTex.h"

struct OpenGLMetadata
{
	unsigned internalFormat;
	unsigned format;
	unsigned type;
};

class ModuleTexture : public Module
{
public:
	ModuleTexture();
	~ModuleTexture();

	bool LoadTexture(const wchar_t* texturePath, DirectX::TexMetadata& outMetadata, DirectX::ScratchImage& outImage);
	static void ConvertMetadata(const DirectX::TexMetadata& metadata, OpenGLMetadata& outMetadata);
};

