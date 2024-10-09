#include "InputManager.h"

using namespace LiveWireRemake;

bool InputManager::GetKey(Key key) const noexcept
{
    return _keyboard.GetKey(key);
}

bool InputManager::GetKeyDown(Key key) const noexcept
{
    return _keyboard.GetKeyDown(key);
}

bool InputManager::GetKeyUp(Key key) const noexcept
{
    return _keyboard.GetKeyUp(key);
}

void InputManager::Push(const MSG& msg) noexcept
{
    _keyboard.Push(msg);
}

void InputManager::Flush(const MSG& msg) noexcept
{
    _keyboard.Flush();
}

bool InputManager::Keyboard::GetKey(Key key) const noexcept
{
    for (const auto& k : _down)
    {
        if (k != key)
            continue;

        return true;
    }

    for (const auto& k : _hold)
    {
        if (k != key)
            continue;

        return true;
    }

    return false;
}

bool InputManager::Keyboard::GetKeyDown(Key key) const noexcept
{
    for (const auto& k : _down)
    {
        if (k != key)
            continue;

        return true;
    }

    return false;
}

bool InputManager::Keyboard::GetKeyUp(Key key) const noexcept
{
    for (const auto& k : _up)
    {
        if (k != key)
            continue;

        return true;
    }

    return false;
}

void InputManager::Keyboard::Push(const MSG& msg) noexcept
{
    unsigned int message = (unsigned int)msg.message;
    const auto key = (Key)msg.wParam;
    switch (message)
    {
    case WM_KEYDOWN:
        if (GetKey(key) == false)
        {
            _hold.push_back(key);
            _down.push_back(key);
        }
        break;
    case WM_KEYUP:
        if (!GetKeyUp(key) && GetKey(key))
        {
            _up.push_back(key);
            for (auto pBegin = _hold.begin(); pBegin != _hold.end(); ++pBegin)
            {
                if (*pBegin != key)
                    continue;

                _hold.erase(pBegin);
                break;
            }
        }
        break;
    default:
        break;
    }
}

void InputManager::Keyboard::Flush() noexcept
{
    _down.clear();
    _up.clear();
}
