#include <Windows.h>
#include <chrono>
#include <fstream>

#include "nexus/Nexus.h"
#include "imgui/imgui.h"
#include "shared.h"
#include "proofs.h"
#include "gui.h"

AddonDefinition AddonDef = {};
HMODULE hSelf = nullptr;

void AddonOptions() {
	ImGui::Separator();
	ImGui::Text("Log Proofs");
	RenderSettings();
}

void AddonRender() {
	UpdateProofs();
	RenderProofs();
}

void AddonLoad(AddonAPI* addonApi) {
	APIDefs = addonApi;

	ImGui::SetCurrentContext((ImGuiContext*)APIDefs->ImguiContext); // cast to ImGuiContext*
	ImGui::SetAllocatorFunctions((void* (*)(size_t, void*))APIDefs->ImguiMalloc, (void(*)(void*, void*))APIDefs->ImguiFree); // on imgui 1.80+

	NexusLink = (NexusLinkData*)APIDefs->GetResource("DL_NEXUS_LINK");

	APIDefs->RegisterRender(ERenderType_Render, AddonRender);
	APIDefs->RegisterRender(ERenderType_OptionsRender, AddonOptions);

	APIDefs->Log(ELogLevel_DEBUG, "Log Proofs", "<c=#00ff00>Log Proofs</c> was loaded.");
}

void AddonUnload() {
	APIDefs->DeregisterRender(AddonRender);
	APIDefs->DeregisterRender(AddonOptions);
	APIDefs->Log(ELogLevel_DEBUG, "Log Proofs", "<c=#ff0000>Log Proofs</c> was unloaded.");
}

extern "C" __declspec(dllexport) AddonDefinition * GetAddonDef() {
	AddonDef.Signature = -541372;
	AddonDef.APIVersion = NEXUS_API_VERSION;
	AddonDef.Name = "Log Proofs";
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