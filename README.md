# PatchCreator for WOW 12340
- Use for create or change DBC files from MPQ or create new
## Libs
>Storm lib https://github.com/ladislav-zezula/StormLib
## functions from dllexport
#
>bool ExtractSupportedDBCFromMPQ(const char* pathToFile)

Extract DBC from MPQ Supported DBC

- List if Supported DBC
    - SpellVisualEffectName.dbc
    - Spell.dbc
    - SpellVisualKit.dbc
    - SpellItemEnchantment.dbc
    - ItemDisplayInfo.dbc

>bool CreateNewMPQWhitChangedDBC(const char* pathToFile)

Create New MPQ File whit supported .dbc files

>bool ChangeSpellDBC(SpellMapFromApp * spellMapFromApp)

Change Spell.dbc whit this structure
```cpp
struct SpellMapFromApp
    {
        int id;
        int newSpellVisual1Value;
    };
```

>bool ChangeItemDisplayInfoDBC(ItemDisplayInfoMapFromApp * itemDisplayInfoMapFromApp)


Change ItemDisplayInfo.dbc whit this structure
```cpp
struct ItemDisplayInfoMapFromApp
    {
        int id;
        int newspellVisualID;
        int newitemVisualID;
    };
```

> bool ChangeSpellItemEnchantmentDBC(SpellItemEnchantmentMapFromApp * spellItemEnchantmentMapFromApp)

Change SpellItemEnchantment.dbc whit this structure
```cpp
struct SpellItemEnchantmentMapFromApp 
    {
        int id;
        int newitemVisuals;
    };
```

>bool ChangeSpellVisualKitDBC(SpellVisualKitMapFromApp * spellVisualKitMapFromApp)


Change Spell.dbc whit this structure
```cpp
struct SpellVisualKitMapFromApp
    {
        int id;
        int newleftHandEffect;
        int newrightHandEffect;
    };
```
