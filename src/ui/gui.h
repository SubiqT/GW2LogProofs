#ifndef GUI_H
#define GUI_H

void RenderWindowLogProofs(void);
void RenderWindowSettings(void);

void ToggleShowWindowLogProofs(const char* keybindIdentifier, bool isRelease);

void RegisterQuickAccessShortcut(void);
void DeregisterQuickAccessShortcut(void);

#endif