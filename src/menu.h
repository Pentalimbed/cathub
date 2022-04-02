#pragma once

#include <filesystem>
#include <toml++/toml.h>

namespace cathub
{
namespace fs = std::filesystem;

const fs::path plugin_dir  = "Data\\SKSE\\Plugins";
const fs::path config_path = "CatHub.toml";

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

    inline void AddMenu(std::string name, std::function<void()> draw_func) { draw_funcs.push_back({name, draw_func}); }
    inline void Toggle(bool enabled) { show = enabled; }
    inline void NotifyInit() { imgui_inited = true; }



private:
    CatMenu()
    {
        draw_funcs.push_back({"Setting", [&]() {
                                  SettingMenu();
                              }});
        try
        {
            auto tbl   = toml::parse_file((plugin_dir / config_path).string());
            toggle_key = tbl["toggle_key"].value_or<int64_t>(43);
            toggle_mod = tbl["toggle_mod"].value_or<int64_t>(ImGuiKeyModFlags_None);
        }
        catch (const toml::parse_error& err)
        {
            logger::error("Failed to parse setting file. Using default settings. Error: {}", err.description());
        }
    }

    bool imgui_inited = false;
    bool show         = false;

    bool             editing_toggle_key = false;
    ImGuiKeyModFlags toggle_mod         = ImGuiKeyModFlags_None;
    uint32_t         toggle_key         = 43; // \ - Back Slash

    std::vector<std::pair<std::string, std::function<void()>>> draw_funcs;

    void SettingMenu();
    void SaveConfig();
};
} // namespace cathub