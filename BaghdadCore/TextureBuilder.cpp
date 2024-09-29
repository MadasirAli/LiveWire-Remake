#include "TextureBuilder.h"

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

	if (_fromFile)
	{
		const auto file = _ppmLoader.Load(_name);
		const auto header = file.GetHeader();

		format = header.depth == 1 ?
			DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UINT :
			DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_UINT;

		width = header.width;
		height = header.height;
	}

	unsigned int sliceSize = DirectXUtil::BitsPerPixel(format) / 8;

	// creating texture
	D3D11_TEXTURE2D_DESC desc = { 0 };
	desc.Format = format;
	desc.BindFlags = _renderTexture ? 
		D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET :
		D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = _readWrite ?
		D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE : 0u;
	desc.Usage = _readWrite ?
		D3D11_USAGE::D3D11_USAGE_DYNAMIC : D3D11_USAGE_IMMUTABLE;
	desc.ArraySize = 1u;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Width = width;
	desc.Height = height;

	D3D11_SUBRESOURCE_DATA data = { 0 };
	data.pSysMem = pData;
	data.SysMemPitch = sliceSize;

	ComPtr<ID3D11Texture2D> pTexture{};
	D3D_CALL(
	_device.GetComPtr()->CreateTexture2D(
		&desc, &data, pTexture.ReleaseAndGetAddressOf()));

	_logger.WriteLine("Texture Created: " + _name +
		"\nWidth: " + std::to_string(width) +
		"\nHeight: " + std::to_string(height));

	return Texture2D(std::move(pTexture));
}

TextureBuilder& TextureBuilder::Clear() noexcept
{
	_name = "";
	_readWrite = false;
	_renderTexture = false;
	_fromFile = false;
	_width = 0u;
	_height = 0u;
	_pData = nullptr;
	_format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;

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

TextureBuilder& TextureBuilder::RenderTexture() noexcept
{
	_renderTexture = true;

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

	if (magic != 8054)	// P6
	{
		THROW_BERROR("Given file is not a PPM File or is corrupted.");
	}

	std::vector<char> buffer{};
	auto counter = 4;

	// width
	while (pFile[counter] != 32)	// SPACE
	{
		buffer.push_back(pFile[counter]);
		++counter;
	}

	auto width = 0;
	for (auto i = 0; i < buffer.size(); i++)
	{
		width += (buffer[i] - 48)* i;
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
		width += (buffer[i] - 48) * i;
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
		width += (buffer[i] - 48) * i;
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

const std::unique_ptr<char[]>& 
TextureBuilder::PPMLoader::PPMFile::GetBufferpPtr() const noexcept
{
	return _pPixels;
}

TextureBuilder::PPMLoader::PPMFile::PPMFile(
	const Header& header, std::unique_ptr<char[]>&& pFile, const unsigned int pixelOffset) :
	_header(header),
	_pFile(std::move(pFile)),
	_pPixels(pFile.get() + pixelOffset)
{}