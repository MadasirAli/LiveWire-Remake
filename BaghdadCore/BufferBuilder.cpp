#include "BufferBuilder.h"

#include <wrl/client.h>

#include "Globals.h"
#include "GraphicsError.h"

using namespace BaghdadCore;

ConstantBuffer BaghdadCore::BufferBuilder::BuildCBuffer()
{
	using namespace Microsoft::WRL;

	unsigned int bindFlags = _bindFlags;
	D3D11_USAGE usage = _write ? D3D11_USAGE::D3D11_USAGE_DYNAMIC : D3D11_USAGE::D3D11_USAGE_DEFAULT;
	unsigned int cpuFlags = _write ? D3D11_CPU_ACCESS_WRITE : 0u;
	cpuFlags |= _read ? D3D11_CPU_ACCESS_READ : 0u;

	// creating constant buffer
	D3D11_BUFFER_DESC desc = { 0 };
	desc.ByteWidth = _size;
	desc.Usage = usage;
	desc.BindFlags = bindFlags;
	desc.CPUAccessFlags = cpuFlags;
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
			std::move(pBuffer), Resource::View())));
}

BufferBuilder& BufferBuilder::Clear() noexcept
{
	_read = false;
	_write = false;
	_bindFlags = 0u;

	_pData = nullptr;
	_size = 0u;

	return *this;
}

BufferBuilder& BufferBuilder::InitialData(const char* pData, const unsigned int size) noexcept
{
	_pData = pData;
	_size = size;

	return *this;
}

BufferBuilder& BufferBuilder::Read() noexcept
{
	_read = true;
	return *this;
}

BufferBuilder& BufferBuilder::Write() noexcept
{
	_write = true;
	return *this;
}

BufferBuilder& BaghdadCore::BufferBuilder::Bind(D3D11_BIND_FLAG bindFlag) noexcept
{
	_bindFlags |= (unsigned int)bindFlag;

	return *this;
}

BufferBuilder::BufferBuilder(const Device& device) :
	_device(device),
	_logger(Globals::GetLogger())
{}
