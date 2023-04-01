#ifndef connectFunc_HPP
#define connectFunc_HPP
    #include <string>
    #include <map>

    struct JsonMap
    {
        int Key;
        int Value;
    };

    extern "C" __declspec(dllexport) bool PatchCreate(JsonMap * spellMap, int spellMapCount, JsonMap * spellVusualKitMap, int spellVusualKitCount, const char* path);

    bool MainFunction(JsonMap* spellMap, int spellMapCount, JsonMap* spellVusualKitMap, int spellVusualKitCount, std::string path);
    bool ExtractMPQ(std::string);
    bool ParseJsons(JsonMap*, int, JsonMap*, int);

    TCHAR* ConverterToTCHAR(const char* orig);
    bool ChangeSpellDBC();
    bool ChangeItemDisplayInfoDBC();
    bool ChangeSpellItemEnchantmentDBC();
    //bool ChangeSpellVisualDBC();
    bool ChangeSpellVisualKitDBC();
    //bool ChangeSpellVisualEffectNameDBC();
    bool CreateMPQ(std::string);

#endif