#pragma once

#include <Windows.h>
#include <WinUser.h>
#include <xinput.h>

enum : uint32_t
{
    kInvalid        = static_cast<uint32_t>(-1),
    kKeyboardOffset = 0,
    kMouseOffset    = 256,
    kGamepadOffset  = 266,
    kMaxOffset      = 282
};

enum
{
    kGamepadButtonOffset_DPAD_UP = kGamepadOffset, // 266
    kGamepadButtonOffset_DPAD_DOWN,
    kGamepadButtonOffset_DPAD_LEFT,
    kGamepadButtonOffset_DPAD_RIGHT,
    kGamepadButtonOffset_START,
    kGamepadButtonOffset_BACK,
    kGamepadButtonOffset_LEFT_THUMB,
    kGamepadButtonOffset_RIGHT_THUMB,
    kGamepadButtonOffset_LEFT_SHOULDER,
    kGamepadButtonOffset_RIGHT_SHOULDER,
    kGamepadButtonOffset_A,
    kGamepadButtonOffset_B,
    kGamepadButtonOffset_X,
    kGamepadButtonOffset_Y,
    kGamepadButtonOffset_LT,
    kGamepadButtonOffset_RT // 281
};


inline std::string modflag2String(ImGuiKeyModFlags modflag)
{
    std::string mod_str = "";
    if (modflag & ImGuiKeyModFlags_Ctrl)
        mod_str = "Ctrl+" + mod_str;
    if (modflag & ImGuiKeyModFlags_Shift)
        mod_str = "Shift+" + mod_str;
    if (modflag & ImGuiKeyModFlags_Alt)
        mod_str = "Alt+" + mod_str;
    if (modflag & ImGuiKeyModFlags_Super)
        mod_str = "Super+" + mod_str;
    return mod_str;
}

inline std::string scanCode2String(uint32_t scancode)
{
    if (scancode >= kMaxOffset)
    {
        return "";
    }
    else if (scancode >= kGamepadOffset)
    {
        // not implemented
        return "";
    }
    else if (scancode >= kMouseOffset)
    {
        auto key = scancode - kMouseOffset;
        switch (key)
        {
            case 0:
                return "Left Mouse Button";
            case 1:
                return "Right Mouse Button";
            case 2:
                return "Middle Mouse Button";
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
                return fmt::format("Mouse Button {}", key);
            case 8:
                return "Wheel Up";
            case 9:
                return "Wheel Down";
            default:
                return "";
        }
    }
    else
    {
        TCHAR lpszName[256];
        if (GetKeyNameText(scancode << 16, lpszName, sizeof(lpszName)))
        {
            int         size_needed = WideCharToMultiByte(CP_UTF8, 0, lpszName, wcslen(lpszName), NULL, 0, NULL, NULL);
            std::string key_str(size_needed, 0);
            WideCharToMultiByte(CP_UTF8, 0, lpszName, wcslen(lpszName), &key_str[0], size_needed, NULL, NULL);
            return key_str;
        }
        else
        {
            return "";
        }
    }
}