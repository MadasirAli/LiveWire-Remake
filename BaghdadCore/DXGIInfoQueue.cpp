#include "DXGIInfoQueue.h"

#include <memory>
#include <sstream>
#include "BaghdadError.h"

using namespace BaghdadCore;

bool DXGIInfoQueue::Check() const
{
	return _messageIndex != GetMessageCount();
}

void DXGIInfoQueue::SetInfoQueue()
{
	_messageIndex = GetMessageCount();
}

unsigned long DXGIInfoQueue::GetMessageCount() const
{
	const auto count = _pQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
	return count;
}

std::string DXGIInfoQueue::GetMessages()
{
	const unsigned long startIndex = _messageIndex;
	SetInfoQueue();

	const unsigned long endIndex = _messageIndex;

	std::stringstream stream;

	stream << "[DXGI Info Queue]\n\n";

	for (auto i = startIndex; i < endIndex; i++)
	{
		unsigned long long bufferSize = 0;

		_pQueue->GetMessage(DXGI_DEBUG_ALL,
			i, nullptr, &bufferSize);

		const auto pBuffer = std::make_unique<char>(bufferSize);

		_pQueue->GetMessage(DXGI_DEBUG_ALL,
			i, (DXGI_INFO_QUEUE_MESSAGE*)pBuffer.get(), &bufferSize);

		stream << ((DXGI_INFO_QUEUE_MESSAGE*)pBuffer.get())->pDescription;
		stream << std::endl;
	}

	return stream.str();
}


DXGIInfoQueue& DXGIInfoQueue::GetInstance() const
{
	static auto instance = DXGIInfoQueue();
	return instance;
}

DXGIInfoQueue::DXGIInfoQueue()
{
	const auto handle = LoadLibraryExA("dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
	if (handle == nullptr)
		THROW_LAST_WIN32_BERROR();

	_libHandle = handle;

	typedef HRESULT(WINAPI *DXGIGetDebugInterface)(REFIID, void**);
	const auto funPtr = reinterpret_cast<DXGIGetDebugInterface>(
		GetProcAddress(_libHandle, "DXGIGetDebugInterface"));

	if (funPtr == nullptr)
		THROW_LAST_WIN32_BERROR();

	WIN32_CALL(
	funPtr(__uuidof(IDXGIInfoQueue), (void**)_pQueue.ReleaseAndGetAddressOf()));
}

DXGIInfoQueue::~DXGIInfoQueue() noexcept
{
	if (_libHandle != nullptr)
		FreeLibrary(_libHandle);
}