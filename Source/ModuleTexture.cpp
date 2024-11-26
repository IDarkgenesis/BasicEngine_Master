#include "ModuleTexture.h"
#include "DirectXTex/DirectXTex.h"

ModuleTexture::ModuleTexture()
{
}

ModuleTexture::~ModuleTexture()
{
}

bool ModuleTexture::LoadTexture(const wchar_t* texturePath, DirectX::TexMetadata& metadata ,DirectX::ScratchImage& image)
{

	HRESULT hr = LoadFromDDSFile(texturePath, DirectX::DDS_FLAGS_NONE, &metadata, image);
	
	if (SUCCEEDED(hr)) return true;

	hr = LoadFromTGAFile(texturePath, DirectX::TGA_FLAGS_NONE, &metadata, image);

	if (SUCCEEDED(hr)) return true;

	hr = DirectX::LoadFromWICFile(texturePath, DirectX::WIC_FLAGS_NONE, &metadata, image);

	if (SUCCEEDED(hr)) return true;

	return false;
}
