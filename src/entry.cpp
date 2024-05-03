#include <Windows.h>
#include <chrono>

#include "nexus/Nexus.h"
#include "imgui/imgui.h"
#include "unofficial_extras/Definitions.h"
#include "shared.h"
#include "proofs.h"
#include "gui.h"

AddonDefinition AddonDef = {};
HMODULE hSelf = nullptr;
const char* ADDON_NAME = "Log Proofs";

void AddonOptions() {
	ImGui::Separator();
	ImGui::Text(ADDON_NAME);
	RenderSettings();
}

void AddonRender() {
	UpdatePlayers();
	RenderProofs();
}

void AddonLoad(AddonAPI* addonApi) {
	APIDefs = addonApi;

	ImGui::SetCurrentContext((ImGuiContext*)APIDefs->ImguiContext); // cast to ImGuiContext*
	ImGui::SetAllocatorFunctions((void* (*)(size_t, void*))APIDefs->ImguiMalloc, (void(*)(void*, void*))APIDefs->ImguiFree); // on imgui 1.80+

	NexusLink = (NexusLinkData*)APIDefs->GetResource("DL_NEXUS_LINK");

	APIDefs->RegisterRender(ERenderType_Render, AddonRender);
	APIDefs->RegisterRender(ERenderType_OptionsRender, AddonOptions);

	APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, "<c=#00ff00>Log Proofs</c> was loaded.");
}

void AddonUnload() {
	if (ExtrasSubscriberInfo) {
		ExtrasSubscriberInfo->InfoVersion = 0;
		ExtrasSubscriberInfo->SubscriberName = nullptr;
		ExtrasSubscriberInfo->SquadUpdateCallback = nullptr;
		APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, "<c=#ff0000>Log Proofs</c> unsubscribed to unofficial extras.");
	}
	APIDefs->DeregisterRender(AddonRender);
	APIDefs->DeregisterRender(AddonOptions);
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

extern "C" __declspec(dllexport) void arcdps_unofficial_extras_subscriber_init(const ExtrasAddonInfo * pExtrasInfo, void* pSubscriberInfo) {
	if (pExtrasInfo->ApiVersion == 2) { // Only load if V2 of the unofficial extras API
		extrasLoaded = true;
		
		ExtrasSubscriberInfo = static_cast<ExtrasSubscriberInfoV2*>(pSubscriberInfo);

		std::string account = pExtrasInfo->SelfAccountName;
		if (account.at(0) == ':')
			account.erase(0, 1);
		if (self.empty()) {
			self = account;
			shouldAddPlayer = self;
		}

		ExtrasSubscriberInfo->InfoVersion = 2;
		ExtrasSubscriberInfo->SubscriberName = ADDON_NAME;
		ExtrasSubscriberInfo->SquadUpdateCallback = SquadEventHandler;

		APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, "<c=#00ff00>Log Proofs</c> subscribed to unofficial extras.");
	}
	else {
		APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, "<c=#ff0000>Log Proofs</c> failed to subscribe to unofficial extras.");
	}
	
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