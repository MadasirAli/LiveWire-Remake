#include "Resource.h"

#include <memory>

#include "ComUtility.h"

using namespace BaghdadCore;

const Resource::View& Resource::GetView() const noexcept
{
	return _view;
}

const Microsoft::WRL::ComPtr<ID3D11Resource>& Resource::GetComResourcePtr() const noexcept
{
	return _ptr;
}

Resource::Resource(Microsoft::WRL::ComPtr<ID3D11Resource>&& pResource, View&& view) :
	DeviceChild(std::move(ComUtility::As<ID3D11Resource, ID3D11DeviceChild>(pResource))),
	_view(std::move(view))

{
	_ptr = std::move(pResource);
}

const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& Resource::View::GetSRVComPtr() const noexcept
{
	return _pSRV;
}

const Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& Resource::View::GetRTVComPtr() const noexcept
{
	return _pRTV;
}

const Microsoft::WRL::ComPtr<ID3D11DepthStencilView>& Resource::View::GetDSVComPtr() const noexcept
{
	return _pDSV;
}

const Microsoft::WRL::ComPtr<ID3D11SamplerState>& Resource::View::GetSamplerComPtr() const noexcept
{
	return _pSampler;
}

bool Resource::View::HasSRV() const noexcept
{
	return (_typeFlags & (unsigned int)Type::SRV) != 0;
}

bool Resource::View::HasRTV() const noexcept
{
	return (_typeFlags & (unsigned int)Type::RTV) != 0;
}

bool Resource::View::HasDSV() const noexcept
{
	return (_typeFlags & (unsigned int)Type::DSV) != 0;
}

Resource::View::View(unsigned int typeFlags,
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>&& pSRV,
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>&& pRTV,
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>&& pDSV,
	Microsoft::WRL::ComPtr<ID3D11SamplerState>&& pSampler) :
	_typeFlags(typeFlags),
	_pSRV(std::move(pSRV)),
	_pRTV(std::move(pRTV)),
	_pDSV(std::move(pDSV)),
	_pSampler(std::move(pSampler))
{}
