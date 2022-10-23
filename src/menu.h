#pragma once

#include "config.h"

#include <mutex>
#include <optional>
#include <string_view>

namespace cathub
{

class CatMenu
{
public:
    static CatMenu* GetSingleton()
    {
        static CatMenu menu;
        return std::addressof(menu);
    }

    inline void notifyInit() { imgui_inited = true; }
    inline void notifyFontReload() { reload_font.store(true); }

    inline void toggle(std::optional<bool> enabled = std::nullopt)
    {
        auto& io = ImGui::GetIO();
        io.ClearInputCharacters();
        io.ClearInputKeys();
        show = enabled.value_or(!show);
    }
    void draw();
    void loadFont();

    void addDrawCall(std::string_view name, std::function<void()> draw_func);

private:
    inline CatMenu()
    {
        config.readFile();
        logger::info("CatMenu initialized!");
    }

    bool imgui_inited = false;
    bool show         = false;

    Config            config;
    std::atomic<bool> reset_toggle_key = false;
    std::atomic<bool> reload_font      = false;
    ImFont*           font             = nullptr;

    struct DrawFunc
    {
        std::string           name;
        std::function<void()> func;
        bool                  show = false;
    };
    std::mutex            draw_funcs_mutex;
    std::vector<DrawFunc> draw_funcs;

    void drawConfig();
};
} // namespace cathub
