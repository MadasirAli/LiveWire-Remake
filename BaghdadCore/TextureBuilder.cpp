#include "TextureBuilder.h"

#include <numeric>
#include <vector>
#include <fstream>
#include <wrl/client.h>

#include "Globals.h"
#include "BaghdadError.h"
#include "GraphicsError.h"
#include "ComUtility.h"

#include "BitmapPlusPlus.hpp"

using namespace BaghdadCore;

Texture2D BaghdadCore::TextureBuilder::Build()
{
	using namespace Microsoft::WRL;

	unsigned int width = _width;
	unsigned int height = _height;
	DXGI_FORMAT format = _format;

	const char* pData = _pData;
	const unsigned int viewFlags = _viewFlags;

	unsigned int bindFlags = 0u;
	if ((viewFlags & (unsigned int)Resource::View::Type::SRV) != 0u)
	{
		bindFlags |= D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
	}
	if ((viewFlags & (unsigned int)Resource::View::Type::RTV) != 0u)
	{
		bindFlags |= D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET;
	}
	if ((viewFlags & (unsigned int)Resource::View::Type::DSV) != 0u)
	{
		bindFlags |= D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
	}

	std::unique_ptr<float[]> pImageData{};

	if (_fromFile)
	{
		bmp::Bitmap image{};
		image.load(_name);

		if (!image)
		{
			THROW_BERROR("Failed to load image: " + _name);
		}

		format = DXGI_FORMAT_R32G32B32A32_FLOAT;

		width = image.width();
		height = image.height();

		pImageData = std::make_unique<float[]>(width * height * 4);	// 32 bits

		// 24 bits to 32 bit conversion
		for (auto y = 0; y < height; ++y)
		{
			for (auto x = 0; x < width; ++x)
			{
				auto pixel = image.get(x, y);
				((float*)(pImageData.get() + (y * 4 * width) + (x * 4)))[0] = pixel.r;
				((float*)(pImageData.get() + (y * 4 * width) + (x * 4)))[1] = pixel.g;
				((float*)(pImageData.get() + (y * 4 * width) + (x * 4)))[2] = pixel.b;
				((float*)(pImageData.get() + (y * 4 * width) + (x * 4)))[3] = 1;
			}
		}

		pData = (char*)(pImageData.get());
	}

	unsigned int sliceSize = ((unsigned int)DirectXUtil::BitsPerPixel(format) / 8) * width;

	// creating texture
	D3D11_TEXTURE2D_DESC desc = { 0 };
	desc.Format = format;
	desc.BindFlags = bindFlags;
	desc.CPUAccessFlags = _readWrite ?
		D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE : 0u;
	desc.Usage = _readWrite ?
		D3D11_USAGE::D3D11_USAGE_DYNAMIC : 
		pData == nullptr? D3D11_USAGE_DEFAULT : D3D11_USAGE_IMMUTABLE;
	desc.ArraySize = 1u;
	desc.SampleDesc.Count = 1;
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1u;

	D3D11_SUBRESOURCE_DATA data = { 0 };
	data.pSysMem = pData;
	data.SysMemPitch = sliceSize;

	D3D11_SUBRESOURCE_DATA* pSubResourceData = pData == nullptr ? nullptr : &data;

	ComPtr<ID3D11Texture2D> pTexture{};
	D3D_CALL(
	_device.GetComPtr()->CreateTexture2D(
		&desc, pSubResourceData, pTexture.ReleaseAndGetAddressOf()));

	// creating views
	const auto pResource = ComUtility::As<ID3D11Texture2D, ID3D11Resource>(pTexture);

	// shader resource view
	ComPtr<ID3D11ShaderResourceView> pSRV {};
	ComPtr<ID3D11SamplerState> pSampler{};
	if ((viewFlags & (unsigned int)Resource::View::Type::SRV) != 0)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
		ZeroMemory(&desc, sizeof(desc));

		desc.Format = format;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipLevels = -1;
		desc.Texture2D.MostDetailedMip = 0;

		D3D_CALL(
		_device.GetComPtr()->CreateShaderResourceView(pResource.Get(),
			&desc,
			pSRV.ReleaseAndGetAddressOf())
		);

		D3D11_SAMPLER_DESC samplerDesc = { };
		ZeroMemory(&samplerDesc, sizeof(samplerDesc));

		samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
		samplerDesc.MaxAnisotropy = 2u;

		D3D_CALL(
			_device.GetComPtr()->CreateSamplerState(&samplerDesc, pSampler.ReleaseAndGetAddressOf())
		);
	}

	// render target view
	ComPtr<ID3D11RenderTargetView> pRTV{};
	if ((viewFlags & (unsigned int)Resource::View::Type::RTV) != 0)
	{
		D3D11_RENDER_TARGET_VIEW_DESC desc = {};
		ZeroMemory(&desc, sizeof(desc));

		desc.Format = format;
		desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

		D3D_CALL(
			_device.GetComPtr()->CreateRenderTargetView(pResource.Get(),
				&desc, pRTV.ReleaseAndGetAddressOf())
		);
	}

	/// depth stencil view
	ComPtr<ID3D11DepthStencilView> pDSV{};
	if ((viewFlags & (unsigned int)Resource::View::Type::DSV) != 0)
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC desc = { };
		ZeroMemory(&desc, sizeof(desc));

		desc.Format = format;
		desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

		D3D_CALL(
			_device.GetComPtr()->CreateDepthStencilView(pResource.Get(),
				&desc, pDSV.ReleaseAndGetAddressOf())
		);
	}

	Resource::View view{ _viewFlags,
	std::move(pSRV), std::move(pRTV), std::move(pDSV),
	std::move(pSampler)};

	_logger.WriteLine("Texture Created: " + _name +
		"\nWidth: " + std::to_string(width) +
		"\nHeight: " + std::to_string(height));

	return Texture2D(std::move(pTexture), std::move(view));
}

TextureBuilder& TextureBuilder::Clear() noexcept
{
	_name = "";
	_readWrite = false;
	_fromFile = false;
	_width = 0u;
	_height = 0u;
	_pData = nullptr;
	_format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	_viewFlags = 0u;

	return *this;
}

TextureBuilder& TextureBuilder::ViewFlag(Resource::View::Type viewFlag) noexcept
{
	_viewFlags |= (unsigned int)viewFlag;

	return *this;
}

TextureBuilder& TextureBuilder::Size(const unsigned int width, const unsigned int height) noexcept
{
	_width = width;
	_height = height;

	return *this;
}

TextureBuilder& TextureBuilder::InitialData(const char* const pData)
{
	_pData = pData;

	return *this;
}

TextureBuilder& TextureBuilder::Format(const DXGI_FORMAT format)
{
	_format = format;

	return *this;
}

TextureBuilder& TextureBuilder::ReadWrite() noexcept
{
	_readWrite = true;

	return *this;
}

TextureBuilder& TextureBuilder::FromFile(const std::string& name) noexcept
{
	_name = std::string(name);
	_fromFile = true;

	return *this;
}

TextureBuilder::TextureBuilder(const Device& device) :
	_device(device),
	_logger(Globals::GetLogger())
{}