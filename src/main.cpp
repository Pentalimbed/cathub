#include "menu.h"
#include "interface.h"
#include "render.h"

namespace cathub
{
void draw()
{
    CatMenu::GetSingleton()->Draw();
}

void loadFont()
{
    CatMenu::GetSingleton()->LoadFont();
}

void processMessage(SKSE::MessagingInterface::Message* a_msg)
{
    switch (a_msg->type)
    {
        case SKSE::MessagingInterface::kDataLoaded:
            logger::info("Data Loaded");
            CatMenu::GetSingleton()->reload_font.store(true);
            RE::BSInputDeviceManager::GetSingleton()->AddEventSink(CatMenu::GetSingleton());
            break;
        default:
            break;
    }
}
} // namespace cathub

bool installLog()
{
    auto path = logger::log_directory();
    if (!path)
        return false;

    *path /= fmt::format(FMT_STRING("{}.log"), Version::PROJECT);
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

extern "C"
{
    DLLEXPORT bool SKSEAPI SKSEPlugin_Query(const SKSE::QueryInterface* a_skse, SKSE::PluginInfo* a_info)
    {
        a_info->infoVersion = SKSE::PluginInfo::kVersion;
        a_info->name        = Version::PROJECT.data();
        a_info->version     = Version::VERSION[0];

        if (a_skse->IsEditor())
        {
            logger::critical("Loaded in editor, marking as incompatible"sv);
            return false;
        }

        const auto ver = a_skse->RuntimeVersion();
        if (ver < SKSE::RUNTIME_1_5_39)
        {
            logger::critical(FMT_STRING("Unsupported runtime version {}"), ver.string());
            return false;
        }

        return true;
    }

#ifndef BUILD_SE
    extern "C" DLLEXPORT constinit auto SKSEPlugin_Version = []() {
        SKSE::PluginVersionData v;

        v.PluginVersion(Version::VERSION);
        v.PluginName(Version::PROJECT);

        v.UsesAddressLibrary(true);
        v.CompatibleVersions({SKSE::RUNTIME_LATEST});

        return v;
    }();
#endif

    DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
    {
        installLog();

        logger::info("loaded plugin");

        SKSE::Init(a_skse);
        SKSE::AllocTrampoline(14 * 2);

        using namespace cathub;

        D3DInitHook::post_init_callbacks.push_back([]() {
            DXGIPresentHook::callback_mutex.lock();
            DXGIPresentHook::pre_callbacks.push_back(loadFont);
            DXGIPresentHook::mid_callbacks.push_back(draw);
            DXGIPresentHook::callback_mutex.unlock();
            CatMenu::GetSingleton()->NotifyInit(); });
        stl::write_thunk_call<D3DInitHook>();
        stl::write_thunk_call<DXGIPresentHook>();

        auto messaging = SKSE::GetMessagingInterface();
        if (!messaging->RegisterListener("SKSE", processMessage))
        {
            return false;
        }

        return true;
    }

    DLLEXPORT cathub::CatHubAPI* GetCatHubInterface()
    {
        return cathub::CatHubInterface::getSingleton();
    }
}