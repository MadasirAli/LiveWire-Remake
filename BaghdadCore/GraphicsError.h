#pragma once

#include <string>

#include "Win32.h"
#include "DXGIInfoQueue.h"
#include "BaghdadError.h"

#define THROW_GERROR(msg)																\
{																						\
	throw GraphicsError(																\
			msg + "\n\nFile: " + __FILE__ + "\nLine: " + std::to_string(__LINE__));	\
}																						\

#ifdef _DEBUG

#define D3D_CALL(__func)																\
{																						\
	auto& __queue = DXGIInfoQueue::GetInstance();										\
    __queue.SetInfoQueue();																\
	const auto __result = __func;														\
	std::string __msg;																	\
	if (__result != S_OK || __queue.Check())											\
	{																					\
		__msg += __queue.GetMessages();													\
		THROW_GERROR(__msg);															\
	}																					\
}																						\

#define D3D_CHECK_CALL(__func)															\
{																						\
	auto& __queue = DXGIInfoQueue::GetInstance();										\
	__queue.SetInfoQueue();																\
	__func;																				\
	if (__queue.Check())																\
	{																					\
		std::string __msg = __queue.GetMessages();										\
		THROW_GERROR(__msg);															\
	}																					\
}																						\

#else

#define D3D_CALL(func)																	\
{																						\
	const auto __result = func;															\
	std::string __msg;																	\
	if (__result != S_OK)																\
	{																					\
		THROW_GERROR(__msg);															\
	}																					\
}																						\

#define D3D_CHECK_CALL(func)					\
{												\
	func;										\
}												\
																					

#endif

namespace BaghdadCore
{
	class GraphicsError : public BaghdadError
	{
	public:
		GraphicsError(std::string msg);
		virtual ~GraphicsError() noexcept override = default;
	};
}

