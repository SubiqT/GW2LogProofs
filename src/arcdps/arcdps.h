#include <stdint.h>

/* arcdps export table */
typedef struct arcdps_exports {
	uintptr_t size;        /* size of exports table */
	uint32_t sig;          /* pick a number between 0 and uint32_t max that isn't used by other modules */
	uint32_t imguivers;    /* set this to IMGUI_VERSION_NUM. if you don't use imgui, 18000 (as of 2021-02-02) */
	const char* out_name;  /* name string */
	const char* out_build; /* build string */
	void* wnd_nofilter;    /* wndproc callback, fn(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam), return assigned to umsg */
	void* combat;          /* combat event callback, fn(cbtevent* ev, ag* src, ag* dst, char* skillname, uint64_t id, uint64_t revision) */
	void* imgui;           /* ::present callback, before imgui::render, fn(uint32_t not_charsel_or_loading, uint32_t hide_if_combat_or_ooc) */
	void* options_end;     /* ::present callback, appending to the end of options window in arcdps, fn() */
	void* combat_local;    /* combat event callback like area but from chat log, fn(cbtevent* ev, ag* src, ag* dst, char* skillname, uint64_t id, uint64_t revision) */
	void* wnd_filter;      /* wndproc callback like wnd_nofilter above, input filered using modifiers */
	void* options_windows; /* called once per 'window' option checkbox, with null at the end, non-zero return disables arcdps drawing that checkbox, fn(char* windowname) */
} arcdps_exports;

/* combat event - see evtc docs for details, revision param in combat cb is equivalent of revision byte header */
typedef struct cbtevent {
	uint64_t time;
	uint64_t src_agent;
	uint64_t dst_agent;
	int32_t value;
	int32_t buff_dmg;
	uint32_t overstack_value;
	uint32_t skillid;
	uint16_t src_instid;
	uint16_t dst_instid;
	uint16_t src_master_instid;
	uint16_t dst_master_instid;
	uint8_t iff;
	uint8_t buff;
	uint8_t result;
	uint8_t is_activation;
	uint8_t is_buffremove;
	uint8_t is_ninety;
	uint8_t is_fifty;
	uint8_t is_moving;
	uint8_t is_statechange;
	uint8_t is_flanking;
	uint8_t is_shields;
	uint8_t is_offcycle;
	uint8_t pad61;
	uint8_t pad62;
	uint8_t pad63;
	uint8_t pad64;
} cbtevent;

/* agent short */
typedef struct ag {
	char* name;     /* agent name. may be null. valid only at time of event. utf8 */
	uintptr_t id;   /* agent unique identifier */
	uint32_t prof;  /* profession at time of event. refer to evtc notes for identification */
	uint32_t elite; /* elite spec at time of event. refer to evtc notes for identification */
	uint32_t self;  /* 1 if self, 0 if not */
	uint16_t team;  /* sep21+ */
} ag;

/* Nexus */
struct EvCombatData {
	cbtevent* ev = nullptr;
	ag* src = nullptr;
	ag* dst = nullptr;
	char* skillname = nullptr;
	uint64_t id = NULL;
	uint64_t revision = NULL;
};

struct EvAgentUpdateData // when ev is null
{
	char account[64] = "";   // dst->name	= account name
	char character[64] = ""; // src->name	= character name
	uintptr_t id;            // src->id		= agent id
	uintptr_t instanceId;    // dst->id		= instance id (per map)
	uint32_t added;          // src->prof	= is new agent
	uint32_t target;         // src->elite	= is new targeted agent
	uint32_t self;           // dst->self	= is self
	uint32_t prof;           // dst->prof	= profession / core spec
	uint32_t elite;          // dst->elite	= elite spec
	uint16_t team;           // src->team	= team
	uint16_t subgroup;       // dst->team	= subgroup
};