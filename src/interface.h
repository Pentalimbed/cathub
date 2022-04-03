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

    virtual ImGuiContext* getContext() override;
    virtual void          addMenu(std::string name, std::function<void()> draw_func) override;
};
} // namespace cathub