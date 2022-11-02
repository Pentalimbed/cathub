#pragma once

#include <variant>
#include <imgui.h>
#include <Windows.h>

// credit: Ersh

namespace cathub
{
constexpr REL::Version API_VER = {1, 6, 0, 0};

class CatHubAPI
{
public:
    virtual REL::Version  getVersion();
    virtual ImGuiContext* getContext()                                               = 0;
    virtual void          addMenu(std::string name, std::function<void()> draw_func) = 0; // for menus in CatMenu
    virtual void          addDrawCallback(std::function<void()> draw_func)           = 0; // for stuff outside CatMenu
    virtual bool          isMenuOpen()                                               = 0;
};

[[nodiscard]] inline std::variant<cathub::CatHubAPI*, std::string> RequestCatHubAPI()
{
    typedef cathub::CatHubAPI* (*_RequestCatHubAPIFunc)();

    auto pluginHandle = GetModuleHandle(L"CatHub.dll");
    if (!pluginHandle)
        return "Cannot find Cathub.";

    _RequestCatHubAPIFunc requestAPIFunc = (_RequestCatHubAPIFunc)GetProcAddress(pluginHandle, "GetCatHubInterface");
    if (requestAPIFunc)
    {
        auto api = requestAPIFunc();
        if (api->getVersion() == API_VER)
            return api;
        else
            return std::format("Version mismatch! Requested {}. Get {}.", API_VER, api->getVersion());
    }

    return "Failed to get";
}
} // namespace cathub