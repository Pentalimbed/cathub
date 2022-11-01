#include "config.h"

#include <filesystem>
namespace fs = std::filesystem;

#include <toml++/toml.h>

const fs::path plugin_dir  = "Data\\SKSE\\Plugins";
const fs::path config_path = "CatHub.toml";

namespace cathub
{
void Config::loadFile()
{
    try
    {
        auto tbl = toml::parse_file((plugin_dir / config_path).c_str());

        hotkey_config.toggle_key = tbl["hotkeys"]["toggle_key"].value_or<ImGuiKey>(ImGuiKey_F6);

        font_config.font_path        = tbl["fonts"]["font_path"].value_or<std::string>("");
        font_config.font_size        = tbl["fonts"]["font_size"].value_or<double>(13.0);
        font_config.glyph_chn_full   = tbl["fonts"]["glyph_chn_full"].value_or<bool>(false);
        font_config.glyph_chs_common = tbl["fonts"]["glyph_chs_common"].value_or<bool>(false);
        font_config.glyph_cyr        = tbl["fonts"]["glyph_cyr"].value_or<bool>(false);
        font_config.glyph_jap        = tbl["fonts"]["glyph_jap"].value_or<bool>(false);
        font_config.glyph_kor        = tbl["fonts"]["glyph_kor"].value_or<bool>(false);
        font_config.glyph_thai       = tbl["fonts"]["glyph_thai"].value_or<bool>(false);
        font_config.glyph_viet       = tbl["fonts"]["glyph_viet"].value_or<bool>(false);

        logger::info("Configuration loaded.");
    }
    catch (const toml::parse_error& err)
    {
        logger::warn("Failed to parse configuration file. Using default settings.\nError: {}", err.description());
    }
}

void Config::saveFile()
{
    std::ofstream f(plugin_dir / config_path);
    if (!f.is_open())
    {
        logger::error("Failed to save configuration!");
        return;
    }
    auto tbl = toml::table{
        {"hotkeys", toml::table{{"toggle_key", hotkey_config.toggle_key}}},
        {"fonts", toml::table{
                      {"font_path", font_config.font_path},
                      {"font_size", font_config.font_size},
                      {"glyph_chn_full", font_config.glyph_chn_full},
                      {"glyph_chs_common", font_config.glyph_chs_common},
                      {"glyph_cyr", font_config.glyph_cyr},
                      {"glyph_jap", font_config.glyph_jap},
                      {"glyph_kor", font_config.glyph_kor},
                      {"glyph_thai", font_config.glyph_thai},
                      {"glyph_viet", font_config.glyph_viet},
                  }},

    };
    f << tbl;
    logger::info("Configuration saved.");
}
} // namespace cathub
