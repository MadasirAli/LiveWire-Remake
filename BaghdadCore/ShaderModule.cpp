#include "ShaderModule.h"

#include <assert.h>

#include <wrl/client.h>

#include "BaghdadError.h"

using namespace BaghdadCore;

#define _MODULE_DUMP std::string("\nName: " + _nName + "\nFeature Level: " + _targetFeatureLevel + "\nEntry Point: " + _entryPoint + "\n")

const Blob& ShaderModule::Compile()
{
    assert(_compiled == false);

    using namespace Microsoft::WRL;

    ComPtr<ID3DBlob> codeBlob;
    ComPtr<ID3DBlob> errorBlob;

    UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;

#if _DEBUG
    flags |= D3DCOMPILE_DEBUG;
#endif

    // compiling
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

    // failture handling
    if (result != S_OK || errorBlob.Get() != nullptr)
    {
        auto count = errorBlob->GetBufferSize();
        auto pMsg = errorBlob->GetBufferPointer();
        std::string compileErrors;
        for (auto i = 0; i < count; i++)
        {
            compileErrors.push_back(((char*)pMsg)[count]);
        }

        _logger.LogError(
            "Shader Module Compilation Failed." +
            _MODULE_DUMP +
            "\nCompilation Errors: " + compileErrors);

        THROW_BERROR(
            "Failed to compile shader." +
            _MODULE_DUMP +
            "\n\n Compile Errors:\n" + 
             compileErrors);
    }

    // obtaining reflection interface
    result = D3DReflect(codeBlob->GetBufferPointer(), codeBlob->GetBufferSize(),
        IID_ID3D11ShaderReflection, (void**)_pReflection.ReleaseAndGetAddressOf());

    if (result != S_OK)
    {
        _logger.LogError("Shader Module Acuiring Reflection Interface Failed." + _MODULE_DUMP);

        THROW_BERROR("Shader Module Acuiring Reflection Interface Failed." + _MODULE_DUMP);
    }

    _pBlob = std::make_unique<Blob>(std::move(codeBlob));

    _compiled = true;

    _logger.WriteLine("Shader Module Compiled. " + _MODULE_DUMP);

    return *_pBlob;
}

const Microsoft::WRL::ComPtr<ID3D11ShaderReflection>& ShaderModule::GetComReflectionPointer() const noexcept
{
    assert(_compiled);

    return _pReflection;
}

ShaderModule& BaghdadCore::ShaderModule::EntryPoint(std::string& entryName)
{
    assert(_compiled == false);

    _entryPoint = std::string(entryName);

    return *this;
}

ShaderModule& ShaderModule::TagetFeatureLevel(D3D_FEATURE_LEVEL featureLevel) noexcept
{
    assert(_compiled == false);

    _targetFeatureLevel = featureLevel;

    return *this;
}

const Blob& ShaderModule::GetBlob() const noexcept
{
    assert(_compiled);

    return *_pBlob;
}

bool ShaderModule::IsCompiled() const noexcept
{
    return _compiled;
}

ShaderModule::ShaderModule(std::string& name) :
    _compiled(false),
    _logger(Globals::GetLogger())
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

    _logger.WriteLine("Shader Module Created: " + _nName);
}
