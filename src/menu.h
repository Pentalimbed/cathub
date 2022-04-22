#pragma once

#include <filesystem>
#include <mutex>
#include <toml++/toml.h>

#include "styles.h"

namespace cathub
{

namespace fs = std::filesystem;

const fs::path plugin_dir  = "Data\\SKSE\\Plugins";
const fs::path config_path = "CatHub.toml";

struct CatMenuConfig
{
    uint32_t         toggle_key = 43; // \ - Back Slash
    ImGuiKeyModFlags toggle_mod = ImGuiKeyModFlags_None;
    // bool             block_input = false;

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

    std::string style = "Default";
};

class CatMenu : public RE::BSTEventSink<RE::InputEvent*>
{
public:
    static CatMenu* GetSingleton()
    {
        static CatMenu menu;
        return std::addressof(menu);
    }

    virtual RE::BSEventNotifyControl ProcessEvent(RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>* a_eventSource) override;
    void                             Draw();
    void                             AddMenu(std::string name, std::function<void()> draw_func);
    void                             LoadFont();

    inline void Toggle(bool enabled)
    {
        auto& io = ImGui::GetIO();
        io.ClearInputCharacters();
        io.ClearInputKeys();
        show = enabled;
    }
    inline void NotifyInit()
    {
        logger::info("CatMenu Initialized!");

        bool has_style = false;
        for (auto& [name, func] : styles)
            if (name == config.style)
            {
                has_style = true;
                func();
            }
        if (!has_style)
            config.style = "Default";
        imgui_inited = true;
    }

    std::atomic<bool> reload_font = false;

private:
    CatMenu()
    {
        logger::info("Loading CatMenu!");

        draw_funcs.push_back({"General Setting", [&]() {
                                  SettingMenu();
                              }});
        try
        {
            auto tbl          = toml::parse_file((plugin_dir / config_path).string());
            config.toggle_key = tbl["toggle_key"].value_or<int64_t>(43);
            config.toggle_mod = tbl["toggle_mod"].value_or<int64_t>(ImGuiKeyModFlags_None);
            // config.block_input = tbl["block_input"].value_or<bool>(false);

            config.font_path        = tbl["font_path"].value_or<std::string>("");
            config.font_size        = tbl["font_size"].value_or<double>(13.0);
            config.glyph_chn_full   = tbl["glyph_chn_full"].value_or<bool>(false);
            config.glyph_chs_common = tbl["glyph_chs_common"].value_or<bool>(false);
            config.glyph_cyr        = tbl["glyph_cyr"].value_or<bool>(false);
            config.glyph_jap        = tbl["glyph_jap"].value_or<bool>(false);
            config.glyph_kor        = tbl["glyph_kor"].value_or<bool>(false);
            config.glyph_thai       = tbl["glyph_thai"].value_or<bool>(false);
            config.glyph_viet       = tbl["glyph_viet"].value_or<bool>(false);

            config.style = tbl["style"].value_or<std::string>("Default");
        }
        catch (const toml::parse_error& err)
        {
            logger::error("Failed to parse setting file. Using default settings.\nError: {}", err.description());
        }
    }

    bool imgui_inited = false;
    bool show         = false;

    bool    editing_toggle_key = false;
    ImFont* font               = nullptr;

    CatMenuConfig config;

    std::mutex                                                 draw_funcs_mutex;
    std::vector<std::pair<std::string, std::function<void()>>> draw_funcs;

    void SettingMenu();
    void SaveConfig();
};
} // namespace cathub