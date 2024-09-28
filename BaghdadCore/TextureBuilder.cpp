#include "TextureBuilder.h"

using namespace BaghdadCore;

Texture BaghdadCore::TextureBuilder::Build()
{
	// TODO: TEXTURE LOADING
	// TODO: TEXTURE CREATION

	return Texture();
}

TextureBuilder& TextureBuilder::FromFile(const std::string& name) noexcept
{
	_name = std::string(name);

	return *this;
}

TextureBuilder::TextureBuilder(const Device& device) :
	_device(device)
{}
