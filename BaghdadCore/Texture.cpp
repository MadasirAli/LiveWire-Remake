#include "Texture.h"

#include <memory>

using namespace BaghdadCore;

Texture::Texture(Microsoft::WRL::ComPtr<ID3D11Resource>&& pResource, View&& view) :
	Resource(std::move(pResource), std::move(view))
{}