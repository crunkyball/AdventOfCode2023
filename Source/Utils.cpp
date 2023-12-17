#include <cassert>
#include <cstring>

#include "Utils.h"

void Utils::SplitString(const char* pString, const char* splitString, char* outLeft, const int leftSize, char* outRight, const int rightSize)
{
    memset(outLeft, 0, leftSize);
    memset(outRight, 0, rightSize);

    int stringLen = static_cast<int>(strlen(pString));
    int breakPoint = stringLen; //If splitString isn't found then put the whole string on the left side.

    int splitCharsLen = static_cast<int>(strlen(splitString));
    int splitCharsIdx = 0;

    for (const char* pChar = pString; *pChar != 0; pChar++)
    {
        if (*pChar == splitString[splitCharsIdx])
        {
            if (++splitCharsIdx == splitCharsLen)
            {
                breakPoint = static_cast<int>((pChar - pString)) - (splitCharsLen - 1);
                break;
            }
        }
        else
        {
            splitCharsIdx = 0;
        }
    }

    assert(breakPoint < leftSize);
    memcpy(outLeft, pString, breakPoint);

    breakPoint += splitCharsLen;	//Don't copy split char.

    int rightNumChars = stringLen - breakPoint;

    if (rightNumChars > 0)
    {
        assert(stringLen - breakPoint < rightSize);
        memcpy(outRight, pString + breakPoint, rightNumChars);
    }
}

//outStrings needs to be a 2D array of numStrings * stringSize. SplitString is probably redundant now.
int Utils::Tokenise(const char* pString, const char* splitString, char* outStrings, const int numStrings, const int stringSize)
{
    memset(outStrings, 0, numStrings * stringSize);

    int stringLen = static_cast<int>(strlen(pString));

    int splitCharsLen = static_cast<int>(strlen(splitString));
    int splitCharsIdx = 0;

    int stringIdx = 0;
    const char* pStringStart = pString;

    for (const char* pChar = pString; *pChar != 0; pChar++)
    {
        if (*pChar == splitString[splitCharsIdx])
        {
            if (++splitCharsIdx == splitCharsLen)
            {
                int thisStrLen = static_cast<int>((pChar - pStringStart)) - (splitCharsLen - 1);
                assert(thisStrLen < stringSize);
                memcpy(outStrings + (stringIdx * stringSize), pStringStart, thisStrLen);

                stringIdx++;
                pStringStart += (thisStrLen + splitCharsLen);   //Don't copy the split chars.

                splitCharsIdx = 0;
            }
        }
        else
        {
            splitCharsIdx = 0;
        }
    }

    //Copy the remaining characters.
    int remainingStrLen = static_cast<int>(strlen(pStringStart));
    assert(remainingStrLen < stringSize);
    memcpy(outStrings + (stringIdx * stringSize), pStringStart, remainingStrLen);

    return stringIdx + 1;
}
