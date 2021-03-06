#include "menu.h"
#include "utils.h"

#include <Windows.h>
#include <WinUser.h>
#include <dinput.h>

#include "extern/imgui_stdlib.h"
#include "extern/imgui_impl_dx11.h"

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
    if (!imgui_inited || !a_event || !a_eventSource)
        return RE::BSEventNotifyControl::kContinue;

    auto& io = ImGui::GetIO();

    for (auto event = *a_event; event; event = event->next)
    {
        if (event->eventType == RE::INPUT_EVENT_TYPE::kChar)
        {
            io.AddInputCharacter(static_cast<CharEvent*>(event)->keyCode);
        }
        else if (event->eventType == RE::INPUT_EVENT_TYPE::kButton)
        {
            const auto button = static_cast<RE::ButtonEvent*>(event);
            if (!button || (!button->IsDown() && !button->IsUp()))
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
                        io.AddMouseButtonEvent(key, button->IsDown());
                    }
                    key += kMouseOffset;
                    break;
                case RE::INPUT_DEVICE::kKeyboard:
                    io.AddKeyEvent(ImGui_ImplWin32_VirtualKeyToImGuiKey(MapVirtualKeyEx(key, MAPVK_VSC_TO_VK_EX, GetKeyboardLayout(0))), button->IsDown());
                    switch (key)
                    {
                        case DIK_LCONTROL:
                        case DIK_RCONTROL:
                            io.AddKeyEvent(ImGuiKey_ModCtrl, button->IsDown());
                            break;
                        case DIK_LSHIFT:
                        case DIK_RSHIFT:
                            io.AddKeyEvent(ImGuiKey_ModShift, button->IsDown());
                            break;
                        case DIK_LALT:
                        case DIK_RALT:
                            io.AddKeyEvent(ImGuiKey_ModAlt, button->IsDown());
                            break;
                        case DIK_LWIN:
                        case DIK_RWIN:
                            io.AddKeyEvent(ImGuiKey_ModSuper, button->IsDown());
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
                static auto mod_list = {DIK_LSHIFT, DIK_RSHIFT, DIK_LCONTROL, DIK_RCONTROL, DIK_LALT, DIK_RALT, DIK_LWIN, DIK_RWIN};
                if (button->IsDown() && !std::ranges::any_of(mod_list, [=](uint16_t scancode) { return scancode == key; }))
                {
                    config.toggle_key  = key;
                    config.toggle_mod  = io.KeyMods;
                    editing_toggle_key = false;
                }
            }
            else if (button->IsDown() && io.KeyMods == config.toggle_mod && key == config.toggle_key)
            {
                Toggle(!show);
            }
        }
    }

    return RE::BSEventNotifyControl::kContinue;
}

void CatMenu::Draw()
{
    if (!imgui_inited)
        return;

    auto& io           = ImGui::GetIO();
    io.MouseDrawCursor = show;

    if (!show)
        return;

    if (font)
        ImGui::PushFont(font);
    ImGui::GetStyle().ScaleAllSizes(config.font_scaling);

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

    draw_funcs_mutex.lock();
    ImGui::Combo(
        "Selected Menu",
        &item_current,
        &Funcs::ItemGetter,
        draw_funcs.data(),
        draw_funcs.size());
    ImGui::Separator();
    (draw_funcs[item_current].second)();
    draw_funcs_mutex.unlock();

    if (font)
        ImGui::PopFont();

    ImGui::End();
}

void CatMenu::AddMenu(std::string name, std::function<void()> draw_func)
{
    logger::info("Adding menu {}", name);
    draw_funcs_mutex.lock();
    draw_funcs.push_back({name, draw_func});
    draw_funcs_mutex.unlock();
}

void CatMenu::SettingMenu()
{
    auto io = ImGui::GetIO();

    // Save config
    if (ImGui::Button("Save Config"))
    {
        SaveConfig();
    }

    if (ImGui::TreeNode("Input"))
    {
        // Toggle key
        auto toggle_str = editing_toggle_key ? "Type any key" : "Edit Toggle Key - ";
        ImGui::Checkbox(toggle_str, &editing_toggle_key);
        if (!editing_toggle_key)
        {
            ImGui::SameLine();

            auto mod_str = modflag2String(config.toggle_mod);
            auto key_str = scanCode2String(config.toggle_key);

            ImGui::AlignTextToFramePadding();
            if (key_str.empty())
                ImGui::Text((mod_str + "[Unknown Key]").c_str());
            else
                ImGui::Text((mod_str + key_str).c_str());
        }
        // Block input
        // ImGui::Checkbox("Blocking Player Control", &config.block_input);
        // if (ImGui::IsItemHovered())
        //     ImGui::SetTooltip("Blocking player control when menu is opened.");
        // Clear input
        if (ImGui::Button("Clear Input Keys"))
        {
            io.ClearInputCharacters();
            io.ClearInputKeys();
        }
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("If you Alt+Tab switch windows back and forth,\nthose 2 keys tend to get stuck.\nClick this to unstuck any key.");

        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Font"))
    {
        if (ImGui::Button("Refresh Font"))
            reload_font.store(true);
        ImGui::InputTextWithHint("Font", "Enter ttf/otf font path e.g. \"C:/Windows/Fonts/NotoSans-Regular.ttf\"", &config.font_path);
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("Restart game to apply.");
        ImGui::SliderFloat("Font Size", &config.font_size, 8.0f, 40.0f, "%.1f");
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("Control how big the font is.\nOnly applies to custom fonts.");
        if (ImGui::TreeNode("Extra Glyphs"))
        {
            ImGui::Checkbox("Chinese-Full", &config.glyph_chn_full), ImGui::SameLine();
            ImGui::Checkbox("Chinese-Common Simplified", &config.glyph_chs_common), ImGui::SameLine();
            ImGui::Checkbox("Cyrillic", &config.glyph_cyr);
            ImGui::Checkbox("Japanese", &config.glyph_jap), ImGui::SameLine();
            ImGui::Checkbox("Korean", &config.glyph_kor), ImGui::SameLine();
            ImGui::Checkbox("Thai", &config.glyph_thai), ImGui::SameLine();
            ImGui::Checkbox("Vietnamese", &config.glyph_viet);

            ImGui::TreePop();
        }
        ImGui::TreePop();
    }

    static int current_item = 0;
    struct Funcs
    {
        static bool ItemGetter(void* data, int n, const char** out_str)
        {
            auto vec = (decltype(styles.data()))data;
            *out_str = vec[n].first.c_str();
            return true;
        }
    };

    if (ImGui::Combo(
            "Style",
            &current_item,
            &Funcs::ItemGetter,
            styles.data(),
            styles.size()))
    {
        auto& style  = ImGui::GetStyle();
        style        = ImGuiStyle();
        config.style = styles[current_item].first;
        styles[current_item].second();
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
        {"toggle_key", config.toggle_key},
        {"toggle_mod", config.toggle_mod},
        // {"block_input", config.block_input},
        {"font_path", config.font_path},
        {"font_size", config.font_size},
        {"glyph_chn_full", config.glyph_chn_full},
        {"glyph_chs_common", config.glyph_chs_common},
        {"glyph_cyr", config.glyph_cyr},
        {"glyph_jap", config.glyph_jap},
        {"glyph_kor", config.glyph_kor},
        {"glyph_thai", config.glyph_thai},
        {"glyph_viet", config.glyph_viet},
        {"style", config.style}};
    f << tbl;
}

void CatMenu::LoadFont()
{
    if (!reload_font.load())
        return;
    reload_font.store(false);

    logger::info("Loading fonts!");

    auto     io   = ImGui::GetIO();
    fs::path path = (config.font_path);
    if (fs::is_regular_file(path) && ((path.extension() == ".ttf") || (path.extension() == ".otf")))
    {
        ImVector<ImWchar>        ranges;
        ImFontGlyphRangesBuilder builder;
        builder.AddRanges(io.Fonts->GetGlyphRangesDefault());
        if (config.glyph_chn_full) builder.AddRanges(io.Fonts->GetGlyphRangesChineseFull());
        if (config.glyph_chs_common) builder.AddRanges(io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
        if (config.glyph_cyr) builder.AddRanges(io.Fonts->GetGlyphRangesCyrillic());
        if (config.glyph_jap) builder.AddRanges(io.Fonts->GetGlyphRangesJapanese());
        if (config.glyph_kor) builder.AddRanges(io.Fonts->GetGlyphRangesKorean());
        if (config.glyph_thai) builder.AddRanges(io.Fonts->GetGlyphRangesThai());
        if (config.glyph_viet) builder.AddRanges(io.Fonts->GetGlyphRangesVietnamese());
        builder.BuildRanges(&ranges);

        io.Fonts->Clear();
        io.Fonts->AddFontDefault();
        font = io.Fonts->AddFontFromFileTTF(config.font_path.c_str(), config.font_size, NULL, ranges.Data);
        if (io.Fonts->Build())
        {
            ImGui_ImplDX11_ReCreateFontsTexture();
            return;
        }
        else
            logger::error("Failed to build font {}", config.font_path);
    }
    else
    {
        font = nullptr;
        logger::info("{} is not a font file", config.font_path);
    }

    io.Fonts->Clear();
    io.Fonts->AddFontDefault();
    assert(io.Fonts->Build());

    ImGui_ImplDX11_ReCreateFontsTexture();
}
} // namespace cathub