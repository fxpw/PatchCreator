#pragma once
#ifdef PATCHCREATOR_EXPORTS
#define PATCHCREATOR_API __declspec(dllexport)
#else
#define PATCHCREATOR_API __declspec(dllimport)
#endif
#include <string>
#include <map>


struct Maap
{
    int Key;
    int Value;
    int Count;
};

extern "C" PATCHCREATOR_API bool PatchCreate(Maap * pMaap,const char* a);
