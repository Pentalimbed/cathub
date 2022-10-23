#pragma once

#include <imgui.h>

namespace cathub
{
class Config
{
public:
    void readFile();
    void saveFile();

    struct HotkeyConfig
    {
        ImGuiKey         toggle_key = ImGuiKey_Backslash;
        ImGuiKeyModFlags toggle_mod = ImGuiKeyModFlags_None;
    } hotkey_config;

    struct FontConfig
    {
        std::string font_path        = "";
        float       font_size        = 13.0f;
        float       font_scaling     = 1.0f;
        bool        glyph_chn_full   = false;
        bool        glyph_chs_common = false;
        bool        glyph_cyr        = false;
        bool        glyph_jap        = false;
        bool        glyph_kor        = false;
        bool        glyph_thai       = false;
        bool        glyph_viet       = false;
    } font_config;
};
} // namespace cathub