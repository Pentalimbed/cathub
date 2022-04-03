#pragma once

#include <Windows.h>

namespace cathub
{
class CatHubAPI
{
public:
    virtual ImGuiContext* getContext()                                               = 0;
    virtual void          addMenu(std::string name, std::function<void()> draw_func) = 0;
};

typedef cathub::CatHubAPI* (*_RequestCatHubAPI)();

[[nodiscard]] inline cathub::CatHubAPI* RequestCatHubAPI()
{
    auto              pluginHandle       = GetModuleHandle(L"CatHub.dll");
    _RequestCatHubAPI requestAPIFunction = (_RequestCatHubAPI)GetProcAddress(pluginHandle, "GetCatHubInterface");
    if (requestAPIFunction)
    {
        return requestAPIFunction();
    }
    return nullptr;
}
} // namespace cathub