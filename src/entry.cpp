#include <Windows.h>

#include "imgui/imgui.h"
#include "nexus/Nexus.h"

#include "gui.h"
#include "log_proofs.h"
#include "resource.h"
#include "settings.h"
#include "shared.h"
#include "version.h"
#include "provider_registry.h"
#include "providers/wingman_provider.h"
#include "providers/kpme_provider.h"

AddonDefinition AddonDef = {};

void AddonOptions() {
	ImGui::Separator();
	ImGui::Text(ADDON_NAME);
	RenderWindowSettings();
}

void AddonRender() {
	RenderWindowLogProofs();
}

void AddonLoad(AddonAPI* addonApi) {
	APIDefs = addonApi;

	ImGui::SetCurrentContext((ImGuiContext*) APIDefs->ImguiContext);                                                              // cast to ImGuiContext*
	ImGui::SetAllocatorFunctions((void* (*) (size_t, void*) ) APIDefs->ImguiMalloc, (void (*)(void*, void*)) APIDefs->ImguiFree); // on imgui 1.80+

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

	APIDefs->Events.Subscribe("EV_UNOFFICIAL_EXTRAS_SQUAD_UPDATE", LogProofs::UnExSquadEventHandler);
	APIDefs->Events.Subscribe("EV_ARCDPS_SQUAD_JOIN", LogProofs::ArcSquadJoinEventHandler);
	APIDefs->Events.Subscribe("EV_ARCDPS_SQUAD_LEAVE", LogProofs::ArcSquadLeaveEventHandler);
	APIDefs->Events.Subscribe("EV_ARCDPS_SELF_JOIN", LogProofs::ArcSelfDetectedEventHandler);
	APIDefs->Events.Subscribe("EV_ARCDPS_SELF_LEAVE", LogProofs::ArcSelfLeaveEventHandler);
	APIDefs->Events.Raise("EV_REPLAY_ARCDPS_SELF_JOIN", nullptr);
	APIDefs->Events.Raise("EV_REPLAY_ARCDPS_SQUAD_JOIN", nullptr);

	APIDefs->Renderer.Register(ERenderType_Render, AddonRender);
	APIDefs->Renderer.Register(ERenderType_OptionsRender, AddonOptions);

	APIDefs->UI.RegisterCloseOnEscape("Log Proofs", &Settings::ShowWindowLogProofs);

	APIDefs->Log(ELogLevel_INFO, ADDON_NAME, "<c=#00ff00>Log Proofs</c> was loaded.");
}

void AddonUnload() {
	APIDefs->UI.DeregisterCloseOnEscape("Log Proofs");

	APIDefs->Renderer.Deregister(AddonOptions);
	APIDefs->Renderer.Deregister(AddonRender);

	if (&Settings::ShowQuickAccessShortcut)
		DeregisterQuickAccessShortcut();
	APIDefs->InputBinds.Deregister(KB_TOGGLE_SHOW_WINDOW_LOG_PROOFS);
	APIDefs->Events.Unsubscribe("EV_ARCDPS_SELF_LEAVE", LogProofs::ArcSelfLeaveEventHandler);
	APIDefs->Events.Unsubscribe("EV_ARCDPS_SELF_JOIN", LogProofs::ArcSelfDetectedEventHandler);
	APIDefs->Events.Unsubscribe("EV_ARCDPS_SQUAD_LEAVE", LogProofs::ArcSquadLeaveEventHandler);
	APIDefs->Events.Unsubscribe("EV_ARCDPS_SQUAD_JOIN", LogProofs::ArcSquadJoinEventHandler);
	APIDefs->Events.Unsubscribe("EV_UNOFFICIAL_EXTRAS_SQUAD_UPDATE", LogProofs::UnExSquadEventHandler);

	LogProofs::threadpool.shutdown();

	APIDefs->Log(ELogLevel_INFO, ADDON_NAME, "<c=#ff0000>Log Proofs</c> was unloaded.");
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