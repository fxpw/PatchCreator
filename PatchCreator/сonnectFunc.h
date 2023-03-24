#pragma once
#ifdef PathCreator_EXPORTS
#define PathCreator_API __declspec(dllexport)
#else
#define PathCreator_API __declspec(dllimport)
#endif
#include <string>
#include <map>


struct Maap
{
    int Key;
    int Value;
};

extern "C" PathCreator_API bool PrintHelloWorld(Maap * pMaap,const char* a);
