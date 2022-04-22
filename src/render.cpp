#include "render.h"

#include "skse/NiRenderer.h"
#include "extern/imgui_impl_dx11.h"
#include <imgui_impl_win32.h>

namespace cathub
{

LRESULT WndProcHook::thunk(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    auto& io = ImGui::GetIO();
    if (uMsg == WM_KILLFOCUS)
    {
        io.ClearInputCharacters();
        io.ClearInputKeys();
    }

    return func(hWnd, uMsg, wParam, lParam);
}

void D3DInitHook::thunk()
{
    func();

    logger::info("D3DInit Hooked!");
    auto render_manager = BSRenderManager::GetSingleton();
    if (!render_manager)
    {
        logger::error("Cannot find render manager. Initialization failed!");
        return;
    }

    logger::info("Getting swapchain...");
    auto swapchain = render_manager->swapChain;
    if (!swapchain)
    {
        logger::error("Cannot find swapchain. Initialization failed!");
        return;
    }

    logger::info("Getting swapchain desc...");
    DXGI_SWAP_CHAIN_DESC sd{};
    if (swapchain->GetDesc(std::addressof(sd)) < 0)
    {
        logger::error("IDXGISwapChain::GetDesc failed.");
        return;
    }

    auto device  = render_manager->forwarder;
    auto context = render_manager->context;

    logger::info("Initializing ImGui...");
    ImGui::CreateContext();
    if (!ImGui_ImplWin32_Init(sd.OutputWindow))
    {
        logger::error("ImGui initialization failed (Win32)");
        return;
    }

    if (!ImGui_ImplDX11_Init(device, context))
    {
        logger::error("ImGui initialization failed (DX11)");
        return;
    }
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    logger::info("ImGui initialized!");

    initialized.store(true);

    WndProcHook::func = reinterpret_cast<WNDPROC>(SetWindowLongPtrA(
        sd.OutputWindow,
        GWLP_WNDPROC,
        reinterpret_cast<LONG_PTR>(WndProcHook::thunk)));
    if (!WndProcHook::func)
        logger::error("SetWindowLongPtrA failed!");

    for (auto& fn : post_init_callbacks) fn();
}

void DXGIPresentHook::thunk(std::uint32_t a_p1)
{
    func(a_p1);

    if (!D3DInitHook::initialized.load())
        return;

    callback_mutex.lock();
    for (auto& fn : pre_callbacks) fn();
    callback_mutex.unlock();

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    callback_mutex.lock();
    for (auto& fn : mid_callbacks) fn();
    callback_mutex.unlock();

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
} // namespace cathub