#pragma once

#define UNICODE
#define _UNICODE
#define WIN32_MEAN_AND_LEAN

#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>

namespace logger = SKSE::log;
using namespace std::literals;

namespace stl
{
template <class T>
void write_thunk_call()
{
    auto&                           trampoline = SKSE::GetTrampoline();
    REL::Relocation<std::uintptr_t> hook{T::id, T::offset};
    T::func = trampoline.write_call<5>(hook.address(), T::thunk);
}
} // namespace stl