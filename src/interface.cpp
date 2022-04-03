#include "interface.h"
#include "menu.h"

namespace cathub
{
ImGuiContext* CatHubInterface::getContext()
{
    return ImGui::GetCurrentContext();
}
void CatHubInterface::addMenu(std::string name, std::function<void()> draw_func)
{
    CatMenu::GetSingleton()->AddMenu(name, draw_func);
}
} // namespace cathub