#include "TextureBuilder.h"

#include <vector>
#include <fstream>

#include "Globals.h"
#include "BaghdadError.h"

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
	_device(device),
	_logger(Globals::GetLogger())
{}

TextureBuilder::PPMLoader::PPMFile 
TextureBuilder::PPMLoader::Load(const std::string& name) const
{
	std::ifstream stream{ name, std::ios::binary};

	if (stream.good() == false)
	{
		stream.close();
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
		THROW_BERROR("Failed to allocate memory");
	}

	if (pFile == nullptr)
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