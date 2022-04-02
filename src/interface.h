#pragma once

struct MenuInfo
{
    std::function<ImGuiContext*()>                          get_ctx_func  = nullptr;
    std::function<void(std::string, std::function<void()>)> add_menu_func = nullptr;
};

typedef MenuInfo* (*_GetCatMenuInfo)();

inline MenuInfo* requestMenuInfo()
{
    auto            pluginHandle = GetModuleHandle(L"CatHub.dll");
    _GetCatMenuInfo requestFunc  = (_GetCatMenuInfo)GetProcAddress(pluginHandle, "GetCatMenuInfo");
    if (requestFunc)
    {
        return requestFunc();
    }
    return nullptr;
}