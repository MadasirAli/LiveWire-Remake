#include "DXGIFactory.h"
#include "BaghdadError.h"

using namespace BaghdadCore;

DXGIFactory::DXGIFactory() {
	WIN32_CALL(
		CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)_ptr.ReleaseAndGetAddressOf()));
}