#pragma once
#include "cathub.h"

namespace cathub
{
class CatHubInterface : public CatHubAPI
{
public:
    static CatHubInterface* getSingleton()
    {
        static CatHubInterface face;
        return std::addressof(face);
    }

    inline virtual REL::Version getVersion() override { return SKSE::PluginDeclaration::GetSingleton()->GetVersion(); }
    virtual ImGuiContext*       getContext() override;
    virtual void                addMenu(std::string name, std::function<void()> draw_func) override;
    virtual void                addDrawCallback(std::function<void()> draw_func) override;
    virtual bool                isMenuOpen() override;
};
} // namespace cathub