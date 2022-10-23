#include "menu.h"

#include <filesystem>
namespace fs = std::filesystem;

#include <imgui.h>
#include "extern/imgui_impl_dx11.h"
#include "extern/imgui_stdlib.h"

namespace cathub
{
void CatMenu::draw()
{
    if (!imgui_inited)
        return;

    auto& io = ImGui::GetIO();
    if (ImGui::IsKeyPressed(config.hotkey_config.toggle_key) && (io.KeyMods == config.hotkey_config.toggle_mod))
        toggle();
    io.MouseDrawCursor = show;

    if (!show)
        return;

    if (font)
        ImGui::PushFont(font);
    ImGui::GetStyle().ScaleAllSizes(config.font_config.font_scaling);

    // ImGui::ShowDemoWindow();

    if (ImGui::Begin("CatMenu", &show, ImGuiWindowFlags_NoCollapse))
    {
        ImGui::SetWindowSize({500, 400}, ImGuiCond_FirstUseEver);

        ImGui::BeginChild("main", {0.f, -ImGui::GetFontSize() - 2.f});
        if (ImGui::BeginTabBar("##"))
        {
            if (ImGui::BeginTabItem("Toggle Display"))
            {
                std::scoped_lock _l(draw_funcs_mutex);

                if (draw_funcs.empty())
                    ImGui::Text("Nothing to see here...");
                else
                {
                    uint32_t i = 0;
                    for (auto& draw_func : draw_funcs)
                    {
                        ImGui::Checkbox(fmt::format("{}##{}", draw_func.name, i).c_str(), &draw_func.show);
                        ++i;
                    }
                }

                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Configuration"))
            {
                drawConfig();
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        ImGui::EndChild();

        ImGui::BeginChild("input bar");
        std::string keys_str = "";
        for (uint32_t key = ImGuiKey_NamedKey_BEGIN; key < ImGuiKey_NamedKey_END; ++key)
            if (ImGui::IsKeyDown(key))
                keys_str = (keys_str + ImGui::GetKeyName(key)) + ' ';
        ImGui::TextColored({0.7f, 0.7f, 0.7f, 1.0f}, "Pressed key(s) (in case they are stuck): %s", keys_str.c_str());
        ImGui::EndChild();
    }
    else
        toggle(false);
    ImGui::End();

    {
        std::scoped_lock _l(draw_funcs_mutex);
        for (auto& draw_func : draw_funcs)
            if (draw_func.show)
                draw_func.func();
    }

    if (font)
        ImGui::PopFont();
}

void CatMenu::loadFont()
{
    if (!reload_font.load())
        return;
    reload_font.store(false);

    logger::info("Loading fonts...");

    auto     io          = ImGui::GetIO();
    auto&    font_config = config.font_config;
    fs::path path        = (font_config.font_path);
    if (fs::is_regular_file(path) && ((path.extension() == ".ttf") || (path.extension() == ".otf")))
    {
        ImVector<ImWchar>        ranges;
        ImFontGlyphRangesBuilder builder;
        builder.AddRanges(io.Fonts->GetGlyphRangesDefault());
        if (font_config.glyph_chn_full) builder.AddRanges(io.Fonts->GetGlyphRangesChineseFull());
        if (font_config.glyph_chs_common) builder.AddRanges(io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
        if (font_config.glyph_cyr) builder.AddRanges(io.Fonts->GetGlyphRangesCyrillic());
        if (font_config.glyph_jap) builder.AddRanges(io.Fonts->GetGlyphRangesJapanese());
        if (font_config.glyph_kor) builder.AddRanges(io.Fonts->GetGlyphRangesKorean());
        if (font_config.glyph_thai) builder.AddRanges(io.Fonts->GetGlyphRangesThai());
        if (font_config.glyph_viet) builder.AddRanges(io.Fonts->GetGlyphRangesVietnamese());
        builder.BuildRanges(&ranges);

        io.Fonts->Clear();
        io.Fonts->AddFontDefault();
        font = io.Fonts->AddFontFromFileTTF(font_config.font_path.c_str(), font_config.font_size, NULL, ranges.Data);
        if (io.Fonts->Build())
        {
            ImGui_ImplDX11_ReCreateFontsTexture();
            return;
        }
        else
            logger::error("Failed to build font {}", font_config.font_path);
    }
    else
    {
        font = nullptr;
        logger::info("{} is not a font file", font_config.font_path);
    }

    io.Fonts->Clear();
    io.Fonts->AddFontDefault();
    assert(io.Fonts->Build());

    ImGui_ImplDX11_ReCreateFontsTexture();
}

void CatMenu::addDrawCall(std::string_view name, std::function<void()> draw_func)
{
    logger::info("Adding menu {}", name);

    std::scoped_lock _l(draw_funcs_mutex);
    draw_funcs.push_back({std::string(name), draw_func, false});
}

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

void CatMenu::drawConfig()
{
    if (ImGui::Button("Reload Config"))
        config.loadFile();
    ImGui::SameLine();
    if (ImGui::Button("Save Config"))
        config.saveFile();

    ImGui::Separator();

    if (ImGui::CollapsingHeader("Hotkeys"))
    {
        auto& hotkey_config = config.hotkey_config;

        if (reset_toggle_key.load())
        {
            ImGui::Text("Waiting for input...");
            for (uint32_t key = ImGuiKey_NamedKey_BEGIN; key < ImGuiKey_NamedKey_END; ++key)
            {
                // mod keys allowed
                constexpr auto mod_keys = std::array{ImGuiKey_ModCtrl, ImGuiKey_ModShift, ImGuiKey_ModAlt, ImGuiKey_ModSuper};
                if (std::ranges::find(mod_keys, key) != mod_keys.end())
                    continue;

                if (ImGui::IsKeyPressed(key))
                {
                    auto io = ImGui::GetIO();
                    reset_toggle_key.store(false);
                    hotkey_config.toggle_key = key;
                    hotkey_config.toggle_mod = io.KeyMods;
                }
            }
        }
        else
        {
            if (ImGui::Button("Edit toggle key"))
                reset_toggle_key.store(true);

            ImGui::SameLine();
            ImGui::AlignTextToFramePadding();
            ImGui::Text(("Current: " + modflag2String(hotkey_config.toggle_mod) + ImGui::GetKeyName(hotkey_config.toggle_key)).c_str());
        }
    }
    if (ImGui::CollapsingHeader("Fonts"))
    {
        auto& font_config = config.font_config;

        if (ImGui::Button("Refresh font"))
            reload_font.store(true);

        ImGui::InputTextWithHint("Font", "path to a ttf/otf font e.g. \"C:/Windows/Fonts/NotoSans-Regular.ttf\"", &font_config.font_path);

        ImGui::SliderFloat("Font Size", &font_config.font_size, 8.0f, 40.0f, "%.1f");
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("Control how big the font is.\nOnly applies to custom fonts.");

        if (ImGui::TreeNode("Extra Glyphs"))
        {
            ImGui::Checkbox("Chinese-Full", &font_config.glyph_chn_full), ImGui::SameLine();
            ImGui::Checkbox("Chinese-Common Simplified", &font_config.glyph_chs_common), ImGui::SameLine();
            ImGui::Checkbox("Cyrillic", &font_config.glyph_cyr);
            ImGui::Checkbox("Japanese", &font_config.glyph_jap), ImGui::SameLine();
            ImGui::Checkbox("Korean", &font_config.glyph_kor), ImGui::SameLine();
            ImGui::Checkbox("Thai", &font_config.glyph_thai), ImGui::SameLine();
            ImGui::Checkbox("Vietnamese", &font_config.glyph_viet);

            ImGui::TreePop();
        }
    }
}
} // namespace cathub
