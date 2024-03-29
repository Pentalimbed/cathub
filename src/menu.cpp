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
    if (ImGui::IsKeyPressed(config.hotkey_config.toggle_key))
        toggle();
    io.MouseDrawCursor = show;

    if (!show)
        return;

    if (font)
        ImGui::PushFont(font);
    ImGui::GetStyle().ScaleAllSizes(config.font_config.font_scaling);

    // ImGui::ShowDemoWindow();

    if (ImGui::Begin(fmt::format("CatMenu (CatHub Ver. {})", SKSE::PluginDeclaration::GetSingleton()->GetVersion().string()).c_str(),
                     &show, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse))
    {
        ImGui::SetWindowSize({400, 300}, ImGuiCond_FirstUseEver);

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

        std::string keys_str = "";
        for (uint32_t key = ImGuiKey_NamedKey_BEGIN; key < ImGuiKey_NamedKey_END; ++key)
            if (ImGui::IsKeyDown(key))
                keys_str = (keys_str + ImGui::GetKeyName(key)) + ' ';
        ImGui::TextDisabled("Pressed key(s) (in case they are stuck): %s", keys_str.c_str());
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

    logger::info("Font loaded.");
}

void CatMenu::addDrawCall(std::string_view name, std::function<void()> draw_func)
{
    logger::info("Adding menu {}", name);

    std::scoped_lock _l(draw_funcs_mutex);
    draw_funcs.push_back({std::string(name), draw_func, false});
}
void CatMenu::drawConfig()
{
    if (ImGui::BeginTable("config", 2))
    {
        ImGui::TableNextColumn();
        if (ImGui::Button("Reload Config", {-FLT_MIN, 0.f}))
            config.loadFile();
        ImGui::TableNextColumn();
        if (ImGui::Button("Save Config", {-FLT_MIN, 0.f}))
            config.saveFile();
        ImGui::EndTable();
    }

    ImGui::Separator();

    if (ImGui::CollapsingHeader("Hotkeys"))
    {
        ImGui::Indent();

        auto& hotkey_config = config.hotkey_config;

        if (reset_toggle_key.load())
        {
            ImGui::Text("Waiting for input...");
            for (uint32_t key = ImGuiKey_NamedKey_BEGIN; key < ImGuiKey_NamedKey_END; ++key)
                if (ImGui::IsKeyPressed(key))
                {
                    auto io = ImGui::GetIO();
                    reset_toggle_key.store(false);
                    hotkey_config.toggle_key = key;
                }
        }
        else
        {
            if (ImGui::Button("Edit toggle key"))
                reset_toggle_key.store(true);

            ImGui::SameLine();
            ImGui::AlignTextToFramePadding();
            ImGui::Text(("Current: "s + ImGui::GetKeyName(hotkey_config.toggle_key)).c_str());
        }

        ImGui::Unindent();
    }
    if (ImGui::CollapsingHeader("Fonts"))
    {
        ImGui::Indent();

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

        ImGui::Unindent();
    }
}
} // namespace cathub
