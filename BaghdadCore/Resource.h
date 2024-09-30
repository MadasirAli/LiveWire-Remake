#pragma once

#include <wrl/client.h>

#include "DeviceChild.h"

namespace BaghdadCore
{
	class Resource : protected DeviceChild
	{
	public:
		class View final
		{
		public:
			enum class Type
			{
				NONE,
				SRV = 1u << 1,
				RTV = 1u << 2,
				DSV = 1u << 3
			};

		public:
			const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& GetSRVComPtr() const noexcept;
			const Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& GetRTVComPtr() const noexcept;
			const Microsoft::WRL::ComPtr<ID3D11DepthStencilView>& GetDSVComPtr() const noexcept;

			const Microsoft::WRL::ComPtr<ID3D11SamplerState>& GetSamplerComPtr() const noexcept;

			bool HasSRV() const noexcept;
			bool HasRTV() const noexcept;
			bool HasDSV() const noexcept;

			View(unsigned int typeFlags,
				Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>&& pSRV,
				Microsoft::WRL::ComPtr<ID3D11RenderTargetView>&& pRTV,
				Microsoft::WRL::ComPtr<ID3D11DepthStencilView>&& pDSV,
				Microsoft::WRL::ComPtr<ID3D11SamplerState>&& pSampler);

			View() = default;

		public:
			unsigned int _typeFlags = 0u;

		private:
			Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _pSRV;
			Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _pRTV;
			Microsoft::WRL::ComPtr<ID3D11DepthStencilView> _pDSV;

			Microsoft::WRL::ComPtr<ID3D11SamplerState> _pSampler;
		};

	public:
		const View& GetView() const noexcept;
		const Microsoft::WRL::ComPtr<ID3D11Resource>& GetComResourcePtr() const noexcept;

	protected:
		Resource(Microsoft::WRL::ComPtr<ID3D11Resource>&& pResource, View&& view);

		virtual ~Resource() noexcept override = default;

	private:
		Microsoft::WRL::ComPtr<ID3D11Resource> _ptr;
		View _view;
	};
}
