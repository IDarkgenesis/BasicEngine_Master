#pragma once
#include "Module.h"
#include "DirectXTex/DirectXTex.h"

class ModuleTexture : public Module
{
public:
	ModuleTexture();
	~ModuleTexture();

	bool LoadTexture(const wchar_t* texturePath, DirectX::TexMetadata& metadata, DirectX::ScratchImage& image);
};

