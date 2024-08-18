#pragma once

#include <string>

#include "Windows.h"

#define THROW_GERROR(msg)						\
{												\
	throw GraphicsError(msg);					\
}												\

#ifdef _DEBUG

#define D3D_CALL(func)																	\
{																						\
	auto& queue = DXGIInfoQueue.GetInstance();											\
    queue.SetInfoQueue();																\																					\
	const auto result = func;															\
	std::string msg;																	\
	if (result != S_OK || queue.Check())												\
	{																					\
		msg += queue.GetMessages();														\
		THROW_GERROR(msg);																\
	}																					\
}																						\

#else

#define D3D_CALL(func)																	\
{																						\																				\
	const auto result = func;															\
	std::string msg;																	\
	if (result != S_OK)																	\
	{																					\													\
		THROW_GERROR(msg);																\
	}																					\
}																						\

#endif

namespace BaghdadCore
{
	class GraphicsError : public GraphicsError
	{
	public:
		GraphicsError(std::string msg);
		virtual ~GraphicsError() noexcept override = default;
	};
}

