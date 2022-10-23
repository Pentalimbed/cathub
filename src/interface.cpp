#include "interface.h"
#include "menu.h"
#include "render.h"

namespace cathub
{
ImGuiContext* CatHubInterface::getContext()
{
    return ImGui::GetCurrentContext();
}
void CatHubInterface::addMenu(std::string name, std::function<void()> draw_func)
{
    CatMenu::GetSingleton()->addDrawCall(name, draw_func);
}
void CatHubInterface::addDrawCallback(std::function<void()> draw_func)
{
    DXGIPresentHook::callback_mutex.lock();
    DXGIPresentHook::mid_callbacks.push_back(draw_func);
    DXGIPresentHook::callback_mutex.unlock();
}
} // namespace cathub