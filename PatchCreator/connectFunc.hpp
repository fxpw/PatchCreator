#ifndef connectFunc_HPP
#define connectFunc_HPP
    #include <string>
    #include <map>

    struct Maap
    {
        int Key;
        int Value;
    };

    extern "C" __declspec(dllexport) bool PatchCreate(Maap * pMaap, int count, const char* a);
    //extern "C" __declspec(dllimport) bool PatchCreate(Maap * pMaap, const char* path, int count);
#endif