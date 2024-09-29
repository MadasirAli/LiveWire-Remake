#pragma once

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "dxgi.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "D3DCompiler.lib")

#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#ifdef _DEBUG
#include <dxgidebug.h>
#endif

namespace DirectXUtil
{
    //-------------------------------------------------------------------------------------
    // Returns bits-per-pixel for a given DXGI format, or 0 on failure
    //-------------------------------------------------------------------------------------
    size_t BitsPerPixel(DXGI_FORMAT fmt);
}