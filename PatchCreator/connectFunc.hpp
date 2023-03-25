#ifndef connectFunc_HPP
#define connectFunc_HPP
    #include <string>
    #include <map>

    struct Maap
    {
        int Key;
        int Value;
        //int Count;
    };

    extern "C" __declspec(dllexport) bool PatchCreate(const char* path, Maap * pMaap, int count);
    //extern "C" __declspec(dllimport) bool PatchCreate(Maap * pMaap, const char* path, int count);
#endif