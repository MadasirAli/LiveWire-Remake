#pragma once

#include <wrl/client.h>

#include "BaghdadError.h"

namespace BaghdadCore
{
	class ComUtility final
	{
	public:
		template<typename T1, typename T2>
		static Microsoft::WRL::ComPtr<T2> As(const Microsoft::WRL::ComPtr<T1>& ptr)
		{
			Microsoft::WRL::ComPtr<T2> target = nullptr;

			WIN32_CALL(
			ptr.As(&target));

			return target;
		}

	private:
		ComUtility() = default;
	};
}

