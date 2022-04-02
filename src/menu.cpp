#include "menu.h"

#include <Windows.h>
#include <WinUser.h>
#include <dinput.h>

#include <utils.h>

// copied from imgui win32 impl
// Map VK_xxx to ImGuiKey_xxx.
#define IM_VK_KEYPAD_ENTER (VK_RETURN + 256)
static ImGuiKey ImGui_ImplWin32_VirtualKeyToImGuiKey(WPARAM wParam)
{
    switch (wParam)
    {
        case VK_TAB: return ImGuiKey_Tab;
        case VK_LEFT: return ImGuiKey_LeftArrow;
        case VK_RIGHT: return ImGuiKey_RightArrow;
        case VK_UP: return ImGuiKey_UpArrow;
        case VK_DOWN: return ImGuiKey_DownArrow;
        case VK_PRIOR: return ImGuiKey_PageUp;
        case VK_NEXT: return ImGuiKey_PageDown;
        case VK_HOME: return ImGuiKey_Home;
        case VK_END: return ImGuiKey_End;
        case VK_INSERT: return ImGuiKey_Insert;
        case VK_DELETE: return ImGuiKey_Delete;
        case VK_BACK: return ImGuiKey_Backspace;
        case VK_SPACE: return ImGuiKey_Space;
        case VK_RETURN: return ImGuiKey_Enter;
        case VK_ESCAPE: return ImGuiKey_Escape;
        case VK_OEM_7: return ImGuiKey_Apostrophe;
        case VK_OEM_COMMA: return ImGuiKey_Comma;
        case VK_OEM_MINUS: return ImGuiKey_Minus;
        case VK_OEM_PERIOD: return ImGuiKey_Period;
        case VK_OEM_2: return ImGuiKey_Slash;
        case VK_OEM_1: return ImGuiKey_Semicolon;
        case VK_OEM_PLUS: return ImGuiKey_Equal;
        case VK_OEM_4: return ImGuiKey_LeftBracket;
        case VK_OEM_5: return ImGuiKey_Backslash;
        case VK_OEM_6: return ImGuiKey_RightBracket;
        case VK_OEM_3: return ImGuiKey_GraveAccent;
        case VK_CAPITAL: return ImGuiKey_CapsLock;
        case VK_SCROLL: return ImGuiKey_ScrollLock;
        case VK_NUMLOCK: return ImGuiKey_NumLock;
        case VK_SNAPSHOT: return ImGuiKey_PrintScreen;
        case VK_PAUSE: return ImGuiKey_Pause;
        case VK_NUMPAD0: return ImGuiKey_Keypad0;
        case VK_NUMPAD1: return ImGuiKey_Keypad1;
        case VK_NUMPAD2: return ImGuiKey_Keypad2;
        case VK_NUMPAD3: return ImGuiKey_Keypad3;
        case VK_NUMPAD4: return ImGuiKey_Keypad4;
        case VK_NUMPAD5: return ImGuiKey_Keypad5;
        case VK_NUMPAD6: return ImGuiKey_Keypad6;
        case VK_NUMPAD7: return ImGuiKey_Keypad7;
        case VK_NUMPAD8: return ImGuiKey_Keypad8;
        case VK_NUMPAD9: return ImGuiKey_Keypad9;
        case VK_DECIMAL: return ImGuiKey_KeypadDecimal;
        case VK_DIVIDE: return ImGuiKey_KeypadDivide;
        case VK_MULTIPLY: return ImGuiKey_KeypadMultiply;
        case VK_SUBTRACT: return ImGuiKey_KeypadSubtract;
        case VK_ADD: return ImGuiKey_KeypadAdd;
        case IM_VK_KEYPAD_ENTER: return ImGuiKey_KeypadEnter;
        case VK_LSHIFT: return ImGuiKey_LeftShift;
        case VK_LCONTROL: return ImGuiKey_LeftCtrl;
        case VK_LMENU: return ImGuiKey_LeftAlt;
        case VK_LWIN: return ImGuiKey_LeftSuper;
        case VK_RSHIFT: return ImGuiKey_RightShift;
        case VK_RCONTROL: return ImGuiKey_RightCtrl;
        case VK_RMENU: return ImGuiKey_RightAlt;
        case VK_RWIN: return ImGuiKey_RightSuper;
        case VK_APPS: return ImGuiKey_Menu;
        case '0': return ImGuiKey_0;
        case '1': return ImGuiKey_1;
        case '2': return ImGuiKey_2;
        case '3': return ImGuiKey_3;
        case '4': return ImGuiKey_4;
        case '5': return ImGuiKey_5;
        case '6': return ImGuiKey_6;
        case '7': return ImGuiKey_7;
        case '8': return ImGuiKey_8;
        case '9': return ImGuiKey_9;
        case 'A': return ImGuiKey_A;
        case 'B': return ImGuiKey_B;
        case 'C': return ImGuiKey_C;
        case 'D': return ImGuiKey_D;
        case 'E': return ImGuiKey_E;
        case 'F': return ImGuiKey_F;
        case 'G': return ImGuiKey_G;
        case 'H': return ImGuiKey_H;
        case 'I': return ImGuiKey_I;
        case 'J': return ImGuiKey_J;
        case 'K': return ImGuiKey_K;
        case 'L': return ImGuiKey_L;
        case 'M': return ImGuiKey_M;
        case 'N': return ImGuiKey_N;
        case 'O': return ImGuiKey_O;
        case 'P': return ImGuiKey_P;
        case 'Q': return ImGuiKey_Q;
        case 'R': return ImGuiKey_R;
        case 'S': return ImGuiKey_S;
        case 'T': return ImGuiKey_T;
        case 'U': return ImGuiKey_U;
        case 'V': return ImGuiKey_V;
        case 'W': return ImGuiKey_W;
        case 'X': return ImGuiKey_X;
        case 'Y': return ImGuiKey_Y;
        case 'Z': return ImGuiKey_Z;
        case VK_F1: return ImGuiKey_F1;
        case VK_F2: return ImGuiKey_F2;
        case VK_F3: return ImGuiKey_F3;
        case VK_F4: return ImGuiKey_F4;
        case VK_F5: return ImGuiKey_F5;
        case VK_F6: return ImGuiKey_F6;
        case VK_F7: return ImGuiKey_F7;
        case VK_F8: return ImGuiKey_F8;
        case VK_F9: return ImGuiKey_F9;
        case VK_F10: return ImGuiKey_F10;
        case VK_F11: return ImGuiKey_F11;
        case VK_F12: return ImGuiKey_F12;
        default: return ImGuiKey_None;
    }
}

// From skse
class CharEvent : public RE::InputEvent
{
public:
    uint32_t keyCode; // 18 (ascii code)
};

namespace cathub
{
RE::BSEventNotifyControl CatMenu::ProcessEvent(RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>* a_eventSource)
{
    static auto mod_list = {DIK_LSHIFT, DIK_RSHIFT, DIK_LCONTROL, DIK_RCONTROL, DIK_LALT, DIK_RALT, DIK_LWIN, DIK_RWIN};

    if (!imgui_inited || !a_event || !a_eventSource)
        return RE::BSEventNotifyControl::kContinue;

    auto& io = ImGui::GetIO();

    // if (RE::UI::GetSingleton()->GameIsPaused())
    // {
    //     io.ClearInputKeys();
    //     io.ClearInputCharacters();
    // }

    for (auto event = *a_event; event; event = event->next)
    {
        if (event->eventType == RE::INPUT_EVENT_TYPE::kChar)
        {
            io.AddInputCharacter(static_cast<CharEvent*>(event)->keyCode);
        }
        else if (event->eventType == RE::INPUT_EVENT_TYPE::kButton)
        {

            const auto button = static_cast<RE::ButtonEvent*>(event);
            if (!button || (!button->IsPressed() && !button->IsUp()))
                continue;

            auto key = button->GetIDCode();
            switch (button->device.get())
            {
                case RE::INPUT_DEVICE::kMouse:
                    if (key > 7)
                        io.AddMouseWheelEvent(0, button->Value() * (key == 8 ? 1 : -1));
                    else
                    {
                        if (key > 5) key = 5;
                        io.AddMouseButtonEvent(key, button->IsPressed());
                    }
                    key += kMouseOffset;
                    break;
                case RE::INPUT_DEVICE::kKeyboard:
                    io.AddKeyEvent(ImGui_ImplWin32_VirtualKeyToImGuiKey(MapVirtualKeyEx(key, MAPVK_VSC_TO_VK_EX, GetKeyboardLayout(0))), button->IsPressed());
                    switch (key)
                    {
                        case DIK_LCONTROL:
                        case DIK_RCONTROL:
                            io.AddKeyEvent(ImGuiKey_ModCtrl, button->IsPressed());
                            break;
                        case DIK_LSHIFT:
                        case DIK_RSHIFT:
                            io.AddKeyEvent(ImGuiKey_ModShift, button->IsPressed());
                            break;
                        case DIK_LALT:
                        case DIK_RALT:
                            io.AddKeyEvent(ImGuiKey_ModAlt, button->IsPressed());
                            break;
                        case DIK_LWIN:
                        case DIK_RWIN:
                            io.AddKeyEvent(ImGuiKey_ModSuper, button->IsPressed());
                            break;
                        default: break;
                    }
                    key += kKeyboardOffset;
                    break;
                case RE::INPUT_DEVICE::kGamepad:
                    // not implemented yet
                    // key = GetGamepadIndex((RE::BSWin32GamepadDevice::Key)key);
                    break;
                default:
                    continue;
            }

            if (editing_toggle_key)
            {
                if (button->IsPressed() && !std::ranges::any_of(mod_list, [=](uint16_t scancode) { return scancode == key; }))
                {
                    toggle_key         = key;
                    toggle_mod         = io.KeyMods;
                    editing_toggle_key = false;
                    SaveConfig();
                }
            }

            if (!show && io.KeyMods == toggle_mod && key == toggle_key)
            {
                Toggle(true);
            }
        }
    }

    return RE::BSEventNotifyControl::kContinue;
}

void CatMenu::Draw()
{
    auto& io           = ImGui::GetIO();
    io.MouseDrawCursor = show;

    if (!show)
        return;

    if (!ImGui::Begin("CatMenu", &show, ImGuiWindowFlags_NoCollapse))
    {
        Toggle(false);
        ImGui::End();
        return;
    }

    static int item_current = 0;
    struct Funcs
    {
        static bool ItemGetter(void* data, int n, const char** out_str)
        {
            auto vec = (decltype(draw_funcs.data()))data;
            *out_str = vec[n].first.c_str();
            return true;
        }
    };
    ImGui::Combo(
        "Selected Menu",
        &item_current,
        &Funcs::ItemGetter,
        draw_funcs.data(),
        draw_funcs.size());

    (draw_funcs[item_current].second)();

    ImGui::End();
}

void CatMenu::SettingMenu()
{
    auto toggle_str = editing_toggle_key ? "Type any key" : "Edit Toggle Key - ";
    ImGui::Checkbox(toggle_str, &editing_toggle_key);

    if (!editing_toggle_key)
    {
        ImGui::SameLine();

        auto mod_str = modflag2String(toggle_mod);
        auto key_str = scanCode2String(toggle_key);

        if (key_str.empty())
        {
            ImGui::Text((mod_str + "[Unknown Key]").c_str());
        }
        else
        {
            ImGui::Text((mod_str + key_str).c_str());
        }
    }
}

void CatMenu::SaveConfig()
{
    std::ofstream f(plugin_dir / config_path);
    if (!f.is_open())
    {
        logger::error("Failed to write settings!");
        return;
    }
    auto tbl = toml::table{
        {"toggle_key", toggle_key},
        {"toggle_mod", toggle_mod}};
    f << tbl;
}
} // namespace cathub