#include "GraphicsCard.h"

using namespace BaghdadCore;

GraphicsCard::GraphicsCard(Microsoft::WRL::ComPtr<IDXGIAdapter>&& pOther) :
	_ptr(pOther)
{}