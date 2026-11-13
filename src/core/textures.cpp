#include "textures.h"
#include "shared.h"

Texture* GetBossTexture(const BossInfo& boss) {
	return APIDefs->Textures.GetOrCreateFromResource(boss.texture.id, boss.texture.resourceId, hSelf);
}

Texture* GetCurrencyTexture(const std::string& name) {
	if (name == "Legendary Insight")
		return APIDefs->Textures.GetOrCreateFromResource("TEX_LEGENDARY_INSIGHT", IDB_LEGENDARY_INSIGHT, hSelf);
	if (name == "Legendary Divination")
		return APIDefs->Textures.GetOrCreateFromResource("TEX_LEGENDARY_DIVINATION", IDB_LEGENDARY_DIVINATION, hSelf);
	if (name == "Unstable Fractal Essence")
		return APIDefs->Textures.GetOrCreateFromResource("TEX_UNSTABLE_FRACTAL_ESSENCE", IDB_UNSTABLE_FRACTAL_ESSENCE, hSelf);
	if (name == "Boneskinner Ritual Vial")
		return APIDefs->Textures.GetOrCreateFromResource("TEX_BONESKINNER_RITUAL_VIAL", IDB_BONESKINNER_RITUAL_VIAL, hSelf);
	return APIDefs->Textures.GetOrCreateFromResource("TEX_UNDER_CONSTRUCTION", IDB_UNDER_CONSTRUCTION, hSelf);
}
