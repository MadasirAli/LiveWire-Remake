#include "BufferBuilder.h"

#include <wrl/client.h>

#include "Globals.h"
#include "GraphicsError.h"

using namespace BaghdadCore;

ConstantBuffer BaghdadCore::BufferBuilder::BuildCBuffer()
{
	using namespace Microsoft::WRL;

	// creating constant buffer
	D3D11_BUFFER_DESC desc = { 0 };
	desc.ByteWidth = _size;
	desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0u;
	desc.StructureByteStride = 0u;

	D3D11_SUBRESOURCE_DATA data = { 0 };
	data.pSysMem = (void*)_pData;

	ComPtr<ID3D11Buffer> pBuffer{};
	D3D_CALL(
		_device.GetComPtr()->CreateBuffer(
			&desc, &data, pBuffer.ReleaseAndGetAddressOf()));

	_logger.WriteLine("Buffer Created." +
		std::string("\nWidth: ") + std::to_string(desc.ByteWidth));

	return ConstantBuffer(
		std::move(Buffer(
			std::move(pBuffer))));
}

BufferBuilder& BufferBuilder::Clear() noexcept
{
	_write_able = true;
	_pData = nullptr;
	_size = 0u;

	return *this;
}

BufferBuilder& BufferBuilder::InitialData(const char* pData, const unsigned int size)
{
	_pData = pData;
	_size = size;

	return *this;
}

BufferBuilder& BufferBuilder::Writeable() noexcept
{
	_write_able = true;
	return *this;
}

BufferBuilder::BufferBuilder(const Device& device) :
	_device(device),
	_logger(Globals::GetLogger())
{}
