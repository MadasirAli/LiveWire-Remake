#include "TextureBuilder.h"

#include <numeric>
#include <vector>
#include <fstream>
#include <wrl/client.h>

#include "Globals.h"
#include "BaghdadError.h"
#include "GraphicsError.h"
#include "ComUtility.h"

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
	
	PPMLoader::PPMFile file{};
	if (_fromFile)
	{
		file = std::move(_ppmLoader.Load(_name));
		const auto header = file.GetHeader();

		format = header.depth == 1 ?
			DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM :
			DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_UNORM;

		width = header.width;
		height = header.height;

		pData = file.GetBufferpPtr();
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

		samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_ANISOTROPIC;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
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

TextureBuilder::PPMLoader::PPMFile 
TextureBuilder::PPMLoader::Load(const std::string& name) const
{
	std::ifstream stream{ name, std::ios::binary};

	if (stream.is_open() == false)
	{
		THROW_BERROR("Failed to Open File.\n\n" + name)
	}

	// loading whole file in memory
	stream.seekg(0, std::ios::end);
	const auto size = stream.tellg();
	stream.seekg(0, std::ios::beg);

	std::unique_ptr<char[]> pFile{};
	try
	{
		pFile = std::make_unique<char[]>(size);
	}
	catch(...)
	{
		stream.close();
		THROW_BERROR("Failed to allocate memory");
	}

	stream.read(pFile.get(), size);
	stream.close();

	// reading header
	// magic
	const auto magic = ((wchar_t*)pFile.get())[0];

	if (pFile[0] != 80 && pFile[1] != 54)	// P6
	{
		THROW_BERROR("Given file is not a PPM File or is corrupted.");
	}

	std::vector<char> buffer{};
	auto counter = 3;

	// width
	while (pFile[counter] != 32)	// SPACE
	{
		buffer.push_back(pFile[counter]);
		++counter;
	}

	auto width = 0;
	for (auto i = 0; i < buffer.size(); i++)
	{
		width += (buffer[i] - 48) * std::pow<int, int>(10, ((int)buffer.size() - (i + 1)));
	}
	buffer.clear();

	// height
	counter += 1;
	while (pFile[counter] != 10)	// NEW LINE
	{
		buffer.push_back(pFile[counter]);
		++counter;
	}

	auto height = 0;
	for (auto i = 0; i < buffer.size(); i++)
	{
		height += (buffer[i] - 48) * std::pow<int, int>(10, ((int)buffer.size() - (i + 1)));
	}
	buffer.clear();

	// depth
	counter += 1;
	while (pFile[counter] != 10)	// NEW LINE
	{
		buffer.push_back(pFile[counter]);
		++counter;
	}

	auto depth = 0;
	for (auto i = 0; i < buffer.size(); i++)
	{
		depth += (buffer[i] - 48) * std::pow<int, int>(10, ((int)buffer.size() - (i + 1)));
	}
	buffer.clear();

	if (depth < 256)
		depth = 1;
	else
		depth = 2;

	Header header = { 0 };
	header.magic = magic;
	header.width = width;
	header.height = height;
	header.depth = depth;

	// reading pixels
	counter += 1;

	return PPMFile(header, std::move(pFile), counter);
}

const TextureBuilder::PPMLoader::Header& 
TextureBuilder::PPMLoader::PPMFile::GetHeader() const noexcept
{
	return _header;
}

const char*
TextureBuilder::PPMLoader::PPMFile::GetBufferpPtr() const noexcept
{
	return _pPixels;
}

TextureBuilder::PPMLoader::PPMFile::PPMFile(
	const Header& header, std::unique_ptr<char[]>&& pFile, const unsigned int pixelOffset) :
	_header(header),
	_pPixels(pFile.get() + pixelOffset),
	_pFile(std::move(pFile))
{}