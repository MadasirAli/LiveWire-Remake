#include "ShaderModule.h"

#include <assert.h>

#include <wrl/client.h>

#include "BaghdadError.h"

using namespace BaghdadCore;

const Blob& ShaderModule::Compile()
{
    assert(_compiled == false,
        "Shader is already compiled.");

    using namespace Microsoft::WRL;

    ComPtr<ID3DBlob> codeBlob;
    ComPtr<ID3DBlob> errorBlob;

    UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;

#if _DEBUG
    flags |= D3DCOMPILE_DEBUG;
#endif

    auto result = D3DCompileFromFile(
        _name.c_str(),
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        _entryPoint.c_str(),
        _targetFeatureLevel.c_str(),
        flags,
        0,
        codeBlob.ReleaseAndGetAddressOf(),
        errorBlob.ReleaseAndGetAddressOf());

    if (result != S_OK || errorBlob.Get() != nullptr)
    {
        auto count = errorBlob->GetBufferSize();
        auto pMsg = errorBlob->GetBufferPointer();
        std::string compileErrors;
        for (auto i = 0; i < count; i++)
        {
            compileErrors.push_back(((char*)pMsg)[count]);
        }

        THROW_BERROR(
            "Failed to compile shader.\nFile Name: " + _nName +
            "\n\n Compile Errors:\n" + 
             compileErrors);
    }

    _pBlob = std::make_unique<Blob>(std::move(codeBlob));

    _compiled = true;
}

ShaderModule& BaghdadCore::ShaderModule::EntryPoint(std::string& entryName)
{
    assert(_compiled == false);

    _entryPoint = std::string(entryName);
}

ShaderModule& ShaderModule::TagetFeatureLevel(D3D_FEATURE_LEVEL featureLevel) noexcept
{
    assert(_compiled == false);

    _targetFeatureLevel = featureLevel;
}

const Blob& ShaderModule::GetBlob() const noexcept
{
    assert(_compiled,
        "Shader is not compiled yet.");

    return *_pBlob;
}

bool ShaderModule::IsCompiled() const noexcept
{
    return _compiled;
}

ShaderModule::ShaderModule(std::string& name) :
    _compiled(false)
{   
    // to wide str
    const auto wideLength = name.length() * sizeof(wchar_t);
    auto pWStr = std::make_unique<wchar_t[]>(wideLength * sizeof(wchar_t));
    for (auto i = 0; i < wideLength; ++i)
    {
        pWStr[i] = name[i];
    }
    pWStr[wideLength + sizeof(wchar_t)] = 0;

    _name = std::wstring(pWStr.get());
    pWStr.release();

    _nName = std::string(name);
}
