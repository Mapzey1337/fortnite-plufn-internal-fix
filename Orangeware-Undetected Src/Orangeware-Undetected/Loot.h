#pragma once
#include "SDK.h"
#include <string>

bool LootESP(ImDrawList* Renderer, const char* name, uintptr_t CurrentActor, Vector3 LocalRelativeLocation) {

	if (SDK::Utils::CheckItemInScreen(CurrentActor, Renderer_Defines::Width, Renderer_Defines::Height)) {

		uintptr_t ItemRootComponent = read<uintptr_t>(CurrentActor + Offsets::RootComponent);
		Vector3 ItemPosition = read<Vector3>(ItemRootComponent + Offsets::RelativeLocation);
		float ItemDist = LocalRelativeLocation.Distance(ItemPosition) / 100.f;

		std::string null = ("");


		if (Settings::ChestESP && strstr(name, ("Tiered_Chest"))) {


			if (ItemDist < Settings::MaxESPDistance) {
				Vector3 ChestPosition;
				SDK::Classes::AController::WorldToScreen(ItemPosition, &ChestPosition);

				std::string Text = null + ("Chest [") + std::to_string((int)ItemDist) + ("m]");

				ImVec2 TextSize = ImGui::CalcTextSize(Text.c_str());


				Renderer->AddText(ImVec2(ChestPosition.x - TextSize.x / 2, ChestPosition.y - TextSize.y / 2), SettingsColor::ChestESP, Text.c_str());

			}

		}
		else if (Settings::AmmoBoxESP && strstr(name, ("Tiered_Ammo"))) {

			if (ItemDist < Settings::MaxESPDistance) {
				Vector3 AmmoPosition;
				SDK::Classes::AController::WorldToScreen(ItemPosition, &AmmoPosition);

				std::string Text = null + ("AB [") + std::to_string((int)ItemDist) + ("m]");

				ImVec2 TextSize = ImGui::CalcTextSize(Text.c_str());


				Renderer->AddText(ImVec2(AmmoPosition.x - TextSize.x / 2, AmmoPosition.y - TextSize.y / 2), SettingsColor::AmmoBox, Text.c_str());

			}

		}
		else if (strstr(name, ("FortPickupAthena")) && Settings::LootESP) {


			if (ItemDist < Settings::MaxESPDistance) {
				auto item = read<uintptr_t>(CurrentActor + Offsets::PrimaryPickupItemEntry + Offsets::ItemDefinition);
				if (!item) return false;

				auto itemName = reinterpret_cast<SDK::Structs::FText*>(read<uintptr_t>(item + Offsets::DisplayName));
				if (!itemName || !itemName->c_str()) return false;
				auto isAmmo = strstr(itemName->c_str(), ("Ammo: "));

				CHAR text[0xFF] = { 0 };
				uintptr_t CurGay = read<uintptr_t>(CurrentActor + Offsets::CurrentWeapon);
				uintptr_t CurRootGay = read<uintptr_t>(CurGay + Offsets::WeaponData);
				uintptr_t bWTier = read<uintptr_t>(CurRootGay + Offsets::Tier);
				wcstombs(text, itemName->c_wstr() + (isAmmo ? 6 : 0), sizeof(text));

				Vector3 LootPosition;

				SDK::Classes::AController::WorldToScreen(ItemPosition, &LootPosition);


				std::string Text = null + text + (" [") + std::to_string((int)ItemDist) + ("m]");

				ImVec2 TextSize = ImGui::CalcTextSize(Text.c_str());


				Renderer->AddText(ImVec2(LootPosition.x - TextSize.x / 2, LootPosition.y - TextSize.y / 2), SettingsColor::LootESP, Text.c_str());
			}
		}

	}
}