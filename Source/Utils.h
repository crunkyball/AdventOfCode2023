#pragma once

#include <vector>

class Utils
{
public:
    static void SplitString(const char* pString, const char* splitString, char* outLeft, const int leftSize, char* outRight, const int rightSize);
    static int Tokenise(const char* pString, const char* splitString, char* outStrings, const int numStrings, const int stringSize);
};
