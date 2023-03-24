#ifndef connectFunc_HPP
#define connectFunc_HPP
    #include <string>
    #include <map>

    struct Maap
    {
        int Key;
        int Value;
    };

    extern "C" __declspec(dllexport) bool PatchCreate(Maap * pMaap,const char* a);
#endif