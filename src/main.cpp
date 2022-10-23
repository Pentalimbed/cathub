#include "render.h"
#include "input.h"
#include "menu.h"
#include "interface.h"

#include <spdlog/sinks/basic_file_sink.h>

namespace cathub
{
bool installLog()
{
    auto path = logger::log_directory();
    if (!path)
        return false;

    *path /= fmt::format(FMT_STRING("{}.log"), SKSE::PluginDeclaration::GetSingleton()->GetName());
    auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);

    auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));

#ifdef NDEBUG
    log->set_level(spdlog::level::info);
    log->flush_on(spdlog::level::info);
#else
    log->set_level(spdlog::level::trace);
    log->flush_on(spdlog::level::trace);
#endif

    spdlog::set_default_logger(std::move(log));
    spdlog::set_pattern("[%H:%M:%S:%e] %v"s);

    return true;
}

void postInitCallback()
{
    DXGIPresentHook::callback_mutex.lock();
    DXGIPresentHook::pre_callbacks.push_back([]() { CatMenu::GetSingleton()->loadFont(); });
    DXGIPresentHook::mid_callbacks.push_back([]() { CatMenu::GetSingleton()->draw(); });
    DXGIPresentHook::callback_mutex.unlock();
    CatMenu::GetSingleton()->notifyInit();
}

void processMessage(SKSE::MessagingInterface::Message* a_msg)
{
    switch (a_msg->type)
    {
        case SKSE::MessagingInterface::kDataLoaded:
            logger::info("Game: data loaded");
            CatMenu::GetSingleton()->notifyFontReload();
            RE::BSInputDeviceManager::GetSingleton()->AddEventSink(InputListener::GetSingleton());
            logger::info("Input listener registered");
            break;
        default:
            break;
    }
}
} // namespace cathub

SKSEPluginLoad(const SKSE::LoadInterface* skse)
{
    using namespace cathub;

    installLog();

    auto* plugin  = SKSE::PluginDeclaration::GetSingleton();
    auto  version = plugin->GetVersion();
    logger::info("{} {} is loading...", plugin->GetName(), version);

    SKSE::Init(skse);
    SKSE::AllocTrampoline(14 * 2);

    D3DInitHook::post_init_callbacks.push_back(postInitCallback);
    stl::write_thunk_call<D3DInitHook>();
    stl::write_thunk_call<DXGIPresentHook>();

    auto messaging = SKSE::GetMessagingInterface();
    if (!messaging->RegisterListener("SKSE", processMessage))
        return false;

    logger::info("{} has finished loading.", plugin->GetName());
    return true;
}

// API function
extern "C" __declspec(dllexport) cathub::CatHubAPI* GetCatHubInterface()
{
    return cathub::CatHubInterface::getSingleton();
}