#include <chrono>
#include <Windows.h>

#include "imgui/imgui.h"
#include "nexus/Nexus.h"

#include "core/bosses.h"
#include "core/data_loader.h"
#include "core/event_handlers.h"
#include "core/player_manager.h"
#include "core/settings.h"
#include "core/shared.h"
#include "providers/common/provider_registry.h"
#include "providers/kpme/kpme_provider.h"
#include "providers/wingman/wingman_provider.h"
#include "resource.h"
#include "trackers/arcdps_tracker.h"
#include "trackers/realtime_api_tracker.h"
#include "trackers/unofficial_extras_tracker.h"
#include "ui/gui.h"
#include "utils/httpclient.h"
#include "version.h"

AddonDefinition AddonDef = {};

static void LoadPlayerDataWrapper(const std::string& account, const std::string& provider, const std::string& key) {
	DataLoader::LoadPlayerDataLazy(account, provider, key);
}

void AddonOptions() {
	RenderWindowSettings();
}

void AddonRender() {
	// Periodic cleanup of expired cache entries
	if (Settings::ShowWindowLogProofs) {
		static auto lastCleanup = std::chrono::steady_clock::now();
		auto now = std::chrono::steady_clock::now();
		if (now - lastCleanup > std::chrono::seconds(1)) {
			PlayerManager::lazyLoadManager.CleanupExpiredEntries();
			lastCleanup = now;
		}
	}

	RenderWindowLogProofs();
}

void AddonLoad(AddonAPI* addonApi) {
	APIDefs = addonApi;

	ImGui::SetCurrentContext((ImGuiContext*) APIDefs->ImguiContext);
	ImGui::SetAllocatorFunctions((void* (*) (size_t, void*) ) APIDefs->ImguiMalloc, (void (*)(void*, void*)) APIDefs->ImguiFree);

	NexusLink = (NexusLinkData*) APIDefs->DataLink.Get("DL_NEXUS_LINK");

	AddonPath = APIDefs->Paths.GetAddonDirectory("log_proofs");
	SettingsPath = APIDefs->Paths.GetAddonDirectory("log_proofs/settings.json");
	std::filesystem::create_directory(AddonPath);
	Settings::Load(SettingsPath);

	APIDefs->Textures.GetOrCreateFromResource("TEX_LOG_NORMAL", IDB_LOG_NORMAL, hSelf);
	APIDefs->Textures.GetOrCreateFromResource("TEX_LOG_HOVER", IDB_LOG_HOVER, hSelf);
	APIDefs->InputBinds.RegisterWithString(KB_TOGGLE_SHOW_WINDOW_LOG_PROOFS, ToggleShowWindowLogProofs, "(null)");
	if (Settings::ShowQuickAccessShortcut)
		RegisterQuickAccessShortcut();

	ProviderRegistry::Instance().RegisterProvider("Wingman", []() { return std::make_unique<WingmanProvider>(); });
	ProviderRegistry::Instance().RegisterProvider("KPME", []() { return std::make_unique<KpmeProvider>(); });

	trackerManager.RegisterTracker(std::make_unique<RealtimeApiTracker>());
	trackerManager.RegisterTracker(std::make_unique<UnofficialExtrasTracker>());
	trackerManager.RegisterTracker(std::make_unique<ArcdpsTracker>());
	auto activeTracker = trackerManager.GetActiveTracker();

	PlayerManager::lazyLoadManager.SetLoadFunction(LoadPlayerDataWrapper);

	HTTPClient::Initialize();
	InitializeBossRegistry();
	InitializeTrackerManager();

	APIDefs->Renderer.Register(ERenderType_Render, AddonRender);
	APIDefs->Renderer.Register(ERenderType_OptionsRender, AddonOptions);

	APIDefs->UI.RegisterCloseOnEscape("Log Proofs", &Settings::ShowWindowLogProofs);

	APIDefs->Log(ELogLevel_INFO, ADDON_NAME, "Log Proofs loaded successfully");
}

void AddonUnload() {
	APIDefs->UI.DeregisterCloseOnEscape("Log Proofs");

	APIDefs->Renderer.Deregister(AddonOptions);
	APIDefs->Renderer.Deregister(AddonRender);

	if (Settings::ShowQuickAccessShortcut)
		DeregisterQuickAccessShortcut();
	APIDefs->InputBinds.Deregister(KB_TOGGLE_SHOW_WINDOW_LOG_PROOFS);

	ShutdownTrackerManager();
	DataLoader::Shutdown();

	APIDefs->Log(ELogLevel_INFO, ADDON_NAME, "Log Proofs unloaded successfully");
}

extern "C" __declspec(dllexport) AddonDefinition* GetAddonDef() {
	AddonDef.Signature = -104905;
	AddonDef.APIVersion = NEXUS_API_VERSION;
	AddonDef.Name = ADDON_NAME;
	AddonDef.Version.Major = V_MAJOR;
	AddonDef.Version.Minor = V_MINOR;
	AddonDef.Version.Build = V_BUILD;
	AddonDef.Version.Revision = V_REVISION;
	AddonDef.Author = "Subi";
	AddonDef.Description = "Displays kill proofs based on logs uploaded to wingman or currencies, tokens and coffers tracked by kpme.";
	AddonDef.Load = AddonLoad;
	AddonDef.Unload = AddonUnload;
	AddonDef.Flags = EAddonFlags_None;
	AddonDef.Provider = EUpdateProvider_GitHub;
	AddonDef.UpdateLink = "https://github.com/SubiqT/GW2LogProofs";
	return &AddonDef;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
	switch (ul_reason_for_call) {
		case DLL_PROCESS_ATTACH:
			hSelf = hModule;
			break;
		case DLL_PROCESS_DETACH:
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
	}
	return TRUE;
}