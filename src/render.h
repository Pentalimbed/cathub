#pragma once

#include <Windows.h>

// Cretdit: SlavicPotato
namespace cathub
{

struct WndProcHook
{
    static LRESULT        thunk(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static inline WNDPROC func;
};

using FuncVector = std::vector<std::function<void()>>;

struct D3DInitHook
{
    static void                                    thunk();
    static inline REL::Relocation<decltype(thunk)> func;

#ifdef BUILD_SE
    static inline uint64_t id     = 75595;
    static inline size_t   offset = 0x9;
#else
    static inline uint64_t id     = 77226;
    static inline size_t   offset = 0x275;
#endif

    static inline std::atomic<bool> initialized = false;

    static inline FuncVector post_init_callbacks;
};

struct DXGIPresentHook
{
    static void                                    thunk(std::uint32_t a_p1);
    static inline REL::Relocation<decltype(thunk)> func;

#ifdef BUILD_SE
    static inline uint64_t id     = 75461;
    static inline size_t   offset = 0x9;
#else
    static inline uint64_t id     = 77246;
    static inline size_t   offset = 0x9;
#endif

    static inline std::mutex callback_mutex;
    static inline FuncVector pre_callbacks;
    static inline FuncVector mid_callbacks;
};
} // namespace cathub