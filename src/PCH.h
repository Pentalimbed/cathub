#pragma once

#define NOGDI

#ifndef UNICODE
#    define UNICODE
#endif

#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"

#include <spdlog/sinks/basic_file_sink.h>

#include <imgui.h>

#define DLLEXPORT __declspec(dllexport)

namespace logger = SKSE::log;
using namespace std::literals;

namespace stl
{
template <class T>
void write_thunk_call()
{
    auto&                           trampoline = SKSE::GetTrampoline();
    REL::Relocation<std::uintptr_t> hook{REL::ID(T::id)};
    T::func = trampoline.write_call<5>(hook.address() + T::offset, T::thunk);
}
} // namespace stl

#include "Version.h"
