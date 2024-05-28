#include <Windows.h>

#include "nexus/Nexus.h"
#include "imgui/imgui.h"

#include "shared.h"
#include "log_proofs.h"
#include "gui.h"
#include "settings.h"
#include "resource.h"
#include "version.h"

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

	ImGui::SetCurrentContext((ImGuiContext*)APIDefs->ImguiContext); // cast to ImGuiContext*
	ImGui::SetAllocatorFunctions((void* (*)(size_t, void*))APIDefs->ImguiMalloc, (void(*)(void*, void*))APIDefs->ImguiFree); // on imgui 1.80+

	NexusLink = (NexusLinkData*)APIDefs->GetResource("DL_NEXUS_LINK");

	AddonPath = APIDefs->GetAddonDirectory("log_proofs");
	SettingsPath = APIDefs->GetAddonDirectory("log_proofs/settings.json");
	std::filesystem::create_directory(AddonPath);
	Settings::Load(SettingsPath);
	
	APIDefs->GetTextureOrCreateFromResource("TEX_LOG_NORMAL", IDB_LOG_NORMAL, hSelf);
	APIDefs->GetTextureOrCreateFromResource("TEX_LOG_HOVER", IDB_LOG_HOVER, hSelf);
	APIDefs->RegisterKeybindWithString("KEYBIND_TOGGLE_SHOW_WINDOW_LOG_PROOFS", ToggleShowWindowLogProofs, "(null)");
	if (Settings::ShowQuickAccessShortcut) RegisterQuickAccessShortcut();

	APIDefs->SubscribeEvent("EV_UNOFFICIAL_EXTRAS_SQUAD_UPDATE", LogProofs::SquadEventHandler);
	APIDefs->SubscribeEvent("EV_ARCDPS_COMBATEVENT_LOCAL_RAW", LogProofs::CombatEventHandler);

	APIDefs->RegisterRender(ERenderType_Render, AddonRender);
	APIDefs->RegisterRender(ERenderType_OptionsRender, AddonOptions);

	APIDefs->Log(ELogLevel_INFO, ADDON_NAME, "<c=#00ff00>Log Proofs</c> was loaded.");
}

void AddonUnload() {
	APIDefs->DeregisterRender(AddonOptions);
	APIDefs->DeregisterRender(AddonRender);

	if (&Settings::ShowQuickAccessShortcut) DeregisterQuickAccessShortcut();
	APIDefs->DeregisterKeybind("KEYBIND_TOGGLE_SHOW_WINDOW_LOG_PROOFS");
	APIDefs->UnsubscribeEvent("EV_ARCDPS_COMBATEVENT_LOCAL_RAW", LogProofs::CombatEventHandler);
	APIDefs->UnsubscribeEvent("EV_UNOFFICIAL_EXTRAS_SQUAD_UPDATE", LogProofs::SquadEventHandler);

	LogProofs::threadpool.shutdown();

	Settings::Save(SettingsPath);
	APIDefs->Log(ELogLevel_INFO, ADDON_NAME, "<c=#ff0000>Log Proofs</c> was unloaded.");
}

extern "C" __declspec(dllexport) AddonDefinition * GetAddonDef() {
	AddonDef.Signature = -104905;
	AddonDef.APIVersion = NEXUS_API_VERSION;
	AddonDef.Name = ADDON_NAME;
	AddonDef.Version.Major = V_MAJOR;
	AddonDef.Version.Minor = V_MINOR;
	AddonDef.Version.Build = V_BUILD;
	AddonDef.Version.Revision = V_REVISION;
	AddonDef.Author = "Subi";
	AddonDef.Description = "Displays kill proofs based on logs uploaded to wingman.";
	AddonDef.Load = AddonLoad;
	AddonDef.Unload = AddonUnload;
	AddonDef.Flags = EAddonFlags_None;
	return &AddonDef;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH: hSelf = hModule; break;
	case DLL_PROCESS_DETACH: break;
	case DLL_THREAD_ATTACH: break;
	case DLL_THREAD_DETACH: break;
	}
	return TRUE;
}