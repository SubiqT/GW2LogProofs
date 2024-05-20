#include <Windows.h>
#include <chrono>

#include "nexus/Nexus.h"
#include "imgui/imgui.h"
#include "shared.h"
#include "proofs.h"
#include "gui.h"

AddonDefinition AddonDef = {};
HMODULE hSelf = nullptr;

void AddonOptions() {
	ImGui::Separator();
	ImGui::Text(ADDON_NAME);
	RenderSettings();
}

void AddonRender() {
	UpdatePlayers();
	RenderWindow();
}

void AddonLoad(AddonAPI* addonApi) {
	APIDefs = addonApi;

	ImGui::SetCurrentContext((ImGuiContext*)APIDefs->ImguiContext); // cast to ImGuiContext*
	ImGui::SetAllocatorFunctions((void* (*)(size_t, void*))APIDefs->ImguiMalloc, (void(*)(void*, void*))APIDefs->ImguiFree); // on imgui 1.80+

	NexusLink = (NexusLinkData*)APIDefs->GetResource("DL_NEXUS_LINK");

	APIDefs->SubscribeEvent("EV_UNOFFICIAL_EXTRAS_SQUAD_UPDATE", SquadEventHandler);
	APIDefs->SubscribeEvent("EV_ARCDPS_COMBATEVENT_LOCAL_RAW" , CombatEventHandler);
	APIDefs->RegisterKeybindWithString(SHOW_ADDON_KEYBIND, ToggleShowWindow, "CTRL+P");

	APIDefs->GetTextureOrCreateFromURL("log_icon_normal", "https://arekimirai.com", "/static/quick_access/log_normal_v3.png");
	APIDefs->GetTextureOrCreateFromURL("log_icon_hover", "https://arekimirai.com", "/static/quick_access/log_hover_v3.png");
	APIDefs->AddShortcut(ADDON_NAME, "log_icon_normal", "log_icon_hover", SHOW_ADDON_KEYBIND, "Log Proofs Table");

	APIDefs->RegisterRender(ERenderType_Render, AddonRender);
	APIDefs->RegisterRender(ERenderType_OptionsRender, AddonOptions);

	APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, "<c=#00ff00>Log Proofs</c> was loaded.");
}

void AddonUnload() {
	APIDefs->DeregisterKeybind(SHOW_ADDON_KEYBIND);
	APIDefs->RemoveShortcut(ADDON_NAME);
	APIDefs->SubscribeEvent("EV_ARCDPS_COMBATEVENT_LOCAL_RAW", CombatEventHandler);
	APIDefs->UnsubscribeEvent("EV_UNOFFICIAL_EXTRAS_SQUAD_UPDATE", SquadEventHandler);
	APIDefs->DeregisterRender(AddonOptions);
	APIDefs->DeregisterRender(AddonRender);
	APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, "<c=#ff0000>Log Proofs</c> was unloaded.");
}

extern "C" __declspec(dllexport) AddonDefinition * GetAddonDef() {
	AddonDef.Signature = -104905;
	AddonDef.APIVersion = NEXUS_API_VERSION;
	AddonDef.Name = ADDON_NAME;
	AddonDef.Version.Major = 1;
	AddonDef.Version.Minor = 0;
	AddonDef.Version.Build = 0;
	AddonDef.Version.Revision = 1;
	AddonDef.Author = "Subi";
	AddonDef.Description = "An addon for checking log proofs.";
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