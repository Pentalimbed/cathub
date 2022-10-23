#pragma once

#include <imgui.h>
#include <Windows.h>

namespace cathub
{
class CatHubAPI
{
public:
    virtual ImGuiContext* getContext()                                               = 0;
    virtual void          addMenu(std::string name, std::function<void()> draw_func) = 0; // for menus in CatMenu
    virtual void          addDrawCallback(std::function<void()> draw_func)           = 0; // for stuff outside CatMenu
};

[[nodiscard]] inline cathub::CatHubAPI* RequestCatHubAPI()
{
    typedef cathub::CatHubAPI* (*_RequestCatHubAPIFunc)();

    auto                  pluginHandle       = GetModuleHandle(L"CatHub.dll");
    _RequestCatHubAPIFunc requestAPIFunction = (_RequestCatHubAPIFunc)GetProcAddress(pluginHandle, "GetCatHubInterface");
    if (requestAPIFunction)
    {
        return requestAPIFunction();
    }
    return nullptr;
}
} // namespace cathub