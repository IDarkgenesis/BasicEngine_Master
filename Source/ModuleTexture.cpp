#include "ModuleTexture.h"
#include "DirectXTex/DirectXTex.h"
#include "glew-2.1.0/include/GL/glew.h"

ModuleTexture::ModuleTexture()
{
}

ModuleTexture::~ModuleTexture()
{
}

bool ModuleTexture::LoadTexture(const wchar_t* texturePath, DirectX::TexMetadata& outMetadata, DirectX::ScratchImage& outImage)
{

	HRESULT hr = LoadFromDDSFile(texturePath, DirectX::DDS_FLAGS_NONE, &outMetadata, outImage);
	
	if (SUCCEEDED(hr)) return true;

	hr = LoadFromTGAFile(texturePath, DirectX::TGA_FLAGS_NONE, &outMetadata, outImage);

	if (SUCCEEDED(hr)) return true;

	hr = DirectX::LoadFromWICFile(texturePath, DirectX::WIC_FLAGS_NONE, &outMetadata, outImage);

	if (SUCCEEDED(hr)) return true;

	return false;
}

void ModuleTexture::ConvertMetadata(const DirectX::TexMetadata& metadata, OpenGLMetadata& outMetadata)
{
	switch (metadata.format)
	{
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
	case DXGI_FORMAT_R8G8B8A8_UNORM:
		outMetadata.internalFormat = GL_RGBA8;
		outMetadata.format = GL_RGBA;
		outMetadata.type = GL_UNSIGNED_BYTE;
		break;
	case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
	case DXGI_FORMAT_B8G8R8A8_UNORM:
		outMetadata.internalFormat = GL_RGBA8;
		outMetadata.format = GL_BGRA;
		outMetadata.type = GL_UNSIGNED_BYTE;
		break;
	case DXGI_FORMAT_B5G6R5_UNORM:
		outMetadata.internalFormat = GL_RGB8;
		outMetadata.format = GL_BGR;
		outMetadata.type = GL_UNSIGNED_BYTE;
		break;
	default:
		assert(false && "Unsupported format");
	}
}
