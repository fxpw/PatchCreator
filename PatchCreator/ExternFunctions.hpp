#ifndef ExternFunc_HPP
#define ExternFunc_HPP
#include <string>
#include <map>
#include "include/MPQ.hpp"

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    extern "C" __declspec(dllexport) bool ExtractSupportedDBCFromMPQ(const char* pathToFile) {
        return MPQ::ExtractDBCFromMPQ(std::string(pathToFile));
    };
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    extern "C" __declspec(dllexport) bool CreateNewMPQWhitChangedDBC(const char* pathToFile) {
        return MPQ::CreateMPQWhitDBC(std::string(pathToFile));
    };
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    struct SpellMapFromApp
    {
        int id;
        int newSpellVisual1Value;
    };
    extern "C" __declspec(dllexport) bool ChangeSpellDBC(SpellMapFromApp * spellMapFromApp) {
        auto* dbc = new DBC<SpellFile, SpellRecord>("./","Spell.dbc");
        int sizeOfMap = sizeof(spellMapFromApp) / sizeof(SpellMapFromApp);
        std::map<uint32_t, uint32_t> SpellMap{};
        for (int i = 0; i < sizeOfMap; i++) {
            SpellMap[spellMapFromApp[i].id] = spellMapFromApp[i].newSpellVisual1Value;
        }
        for (int i = 0; i < sizeOfMap; i++) {
            for (uint64_t i = 0; i < dbc->file.header.recordCount; i++) {
                auto& record = dbc->file.records[i];
                if (SpellMap.count(record.ID)) {
                    record.SpellVisual1 = SpellMap[record.ID];
                }
            }
        }
        dbc->CreateNewFileIn("./newDBC/");
        return true;
    };
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    struct ItemDisplayInfoMapFromApp {
        int id;
        int newspellVisualID;
        int newitemVisualID;
    };
    struct ItemDisplayInfoMapForMap {
        int newspellVisualID;
        int newitemVisualID;
    };
    extern "C" __declspec(dllexport) bool ChangeItemDisplayInfoDBC(ItemDisplayInfoMapFromApp * itemDisplayInfoMapFromApp) {
        auto* dbc = new DBC<ItemDisplayInfoFile, ItemDisplayInfoRecord>("./", "ItemDisplayInfo.dbc");
        int sizeOfMap = sizeof(itemDisplayInfoMapFromApp) / sizeof(ItemDisplayInfoMapFromApp);
        std::map<uint32_t, ItemDisplayInfoMapForMap> ItemDisplayInfoMap{};
        for (int i = 0; i < sizeOfMap; i++) {
            ItemDisplayInfoMap[itemDisplayInfoMapFromApp[i].id].newspellVisualID = itemDisplayInfoMapFromApp[i].newspellVisualID;
            ItemDisplayInfoMap[itemDisplayInfoMapFromApp[i].id].newitemVisualID = itemDisplayInfoMapFromApp[i].newitemVisualID;
        }
        for (int i = 0; i < sizeOfMap; i++) {
            for (uint64_t i = 0; i < dbc->file.header.recordCount; i++) {
                auto& record = dbc->file.records[i];
                if (ItemDisplayInfoMap.count(record.ID)) {
                    record.spellVisualID = ItemDisplayInfoMap[record.ID].newspellVisualID;
                    record.itemVisualID = ItemDisplayInfoMap[record.ID].newitemVisualID;
                }
            }
        }
        dbc->CreateNewFileIn("./newDBC/");
        return true;
    };
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    struct SpellItemEnchantmentMapFromApp {
        int id;
        int newitemVisuals;
    };
    extern "C" __declspec(dllexport) bool ChangeSpellItemEnchantmentDBC(SpellItemEnchantmentMapFromApp * spellItemEnchantmentMapFromApp) {
        auto* dbc = new DBC<SpellItemEnchantmentFile, SpellItemEnchantmentRecord>("./", "SpellItemEnchantment.dbc");
        int sizeOfMap = sizeof(spellItemEnchantmentMapFromApp) / sizeof(SpellItemEnchantmentMapFromApp);
        std::map<uint32_t, uint32_t> SpellItemEnchantmentMap{};
        for (int i = 0; i < sizeOfMap; i++) {
            SpellItemEnchantmentMap[spellItemEnchantmentMapFromApp[i].id] = spellItemEnchantmentMapFromApp[i].newitemVisuals;
        }
        for (int i = 0; i < sizeOfMap; i++) {
            for (uint64_t i = 0; i < dbc->file.header.recordCount; i++) {
                auto& record = dbc->file.records[i];
                if (SpellItemEnchantmentMap.count(record.ID)) {
                    record.ItemVisuals = SpellItemEnchantmentMap[record.ID];
                }
            }
        }
        dbc->CreateNewFileIn("./newDBC/");
        return true;
    };
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    struct SpellVisualEffectNameMapFromApp {
        int id;
        int newspellVisualID;
        int newitemVisualID;
    };
    extern "C" __declspec(dllexport) bool ChangeSpellVisualEffectNameDBC(SpellVisualEffectNameMapFromApp * spellVisualEffectNameMapFromApp) {
        return true;
    };
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    struct SpellVisualKitMapFromApp {
        int id;
        int newleftHandEffect;
        int newrightHandEffect;
    };
    struct SpellVisualKitMapForMap {
        int newleftHandEffect;
        int newrightHandEffect;
    };
    extern "C" __declspec(dllexport) bool ChangeSpellVisualKitDBC(SpellVisualKitMapFromApp * spellVisualKitMapFromApp) {
        auto* dbc = new DBC<SpellVisualKitFile, SpellVisualKitRecord>("./", "SpellVisualKit.dbc");
        int sizeOfMap = sizeof(spellVisualKitMapFromApp) / sizeof(SpellVisualKitMapFromApp);
        std::map<uint32_t, SpellVisualKitMapForMap> SpellVisualKitMap{};
        for (int i = 0; i < sizeOfMap; i++) {
            SpellVisualKitMap[spellVisualKitMapFromApp[i].id].newleftHandEffect = spellVisualKitMapFromApp[i].newleftHandEffect;
            SpellVisualKitMap[spellVisualKitMapFromApp[i].id].newrightHandEffect = spellVisualKitMapFromApp[i].newrightHandEffect;
        }
        for (int i = 0; i < sizeOfMap; i++) {
            for (uint64_t i = 0; i < dbc->file.header.recordCount; i++) {
                auto& record = dbc->file.records[i];
                if (SpellVisualKitMap.count(record.ID)) {
                    record.leftHandEffect = SpellVisualKitMap[record.ID].newleftHandEffect;
                    record.rightHandEffect = SpellVisualKitMap[record.ID].newrightHandEffect;
                }
            }
        }
        dbc->CreateNewFileIn("./newDBC/");
        return true;
    };
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif