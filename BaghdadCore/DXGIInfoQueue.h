#pragma once

#include <string>
#include <wrl/client.h>

#include "D3D11.h"

namespace BaghdadCore
{
	class DXGIInfoQueue final
	{
	private:
		unsigned long GetMessageCount() const;

	public:
		void SetInfoQueue();
		std::string GetMessages();
		bool Check() const;

		DXGIInfoQueue();
		~DXGIInfoQueue() noexcept;
		DXGIInfoQueue(const DXGIInfoQueue& other) = delete;
		DXGIInfoQueue(DXGIInfoQueue& old) noexcept = delete;

		DXGIInfoQueue& operator= (const DXGIInfoQueue& other) = delete;
		DXGIInfoQueue& operator= (DXGIInfoQueue&& old) noexcept = delete;


		static DXGIInfoQueue& GetInstance();

	private:
		Microsoft::WRL::ComPtr<IDXGIInfoQueue> _pQueue = nullptr;
		HMODULE _libHandle;
		unsigned long _messageIndex;
	};
}

