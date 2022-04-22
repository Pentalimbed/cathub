#include "skse/NiRenderer.h"

#ifdef BUILD_SE
REL::Relocation<BSRenderManager> g_renderManager(REL::ID(524907));
#else
REL::Relocation<BSRenderManager> g_renderManager(REL::ID(411393));
#endif

BSRenderManager* BSRenderManager::GetSingleton()
{
    return (BSRenderManager*)g_renderManager.address();
}
