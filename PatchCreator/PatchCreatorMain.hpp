#ifndef connectFunc_HPP
#define connectFunc_HPP
    #include <string>
    #include <map>

    struct Maap
    {
        int Key;
        int Value;
    };

    extern "C" __declspec(dllexport) bool PatchCreate(Maap * pMaap, int count, const char* path);

    bool MainFunction(Maap* pMaap, int count, std::string path);

    TCHAR* ConverterToTCHAR(const char* orig);

    bool ChangeSpellDBC(std::string);
    bool ChangeItemDisplayInfoDBC(std::string);
    bool ChangeSpellItemEnchantmentDBC(std::string);
    bool ChangeSpellVisualDBC(std::string);
    bool ChangeSpellVisualKitDBC(std::string);
    bool ChangeSpellVisualEffectNameDBC(std::string);
    bool CreateMPQ(std::string);
    bool ParseJsons(Maap*, int);
    bool ExtractMPQ(std::string);
    //extern "C" __declspec(dllimport) bool PatchCreate(Maap * pMaap, const char* path, int count);
#endif