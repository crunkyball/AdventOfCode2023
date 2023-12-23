#include "Day15.h"
#include "File.h"
#include "Utils.h"
#include <cassert>

namespace
{
    int HashString(const char* pString)
    {
        int hash = 0;

        int stringLen = strlen(pString);

        for (int i = 0; i < stringLen; ++i)
        {
            int ascii = pString[i];
            hash += ascii;
            hash *= 17;
            hash %= 256;
        }

        return hash;
    }

    void RunPuzzle1(const char* pDataFileName)
    {
        File file(pDataFileName);

        const int kMaxLineBuf = 1024 * 32;
        char lineBuf[kMaxLineBuf];

        file.ReadLine(lineBuf, kMaxLineBuf);

        const int kMaxSeqLen = 16;
        const int kMaxSequences = 1024 * 8;

        char sequences[kMaxSequences][kMaxSeqLen];
        int numSequences = Utils::Tokenise(lineBuf, ",", &sequences[0][0], kMaxSequences, kMaxSeqLen);

        int sumHashes = 0;
        for (int i = 0; i < numSequences; ++i)
        {
            sumHashes += HashString(sequences[i]);
        }

        printf("Sum Hashes: %d\n", sumHashes);
    }

    struct HashBucket
    {
        int Hash = 0;

        struct HashEntry
        {
            static const int kKeySize = 8;
            char Key[kKeySize] = "\0";

            int Value = 0;
        };

        void AddEntry(const char* pKey, int value)
        {
            for (HashEntry& Entry : Entries)
            {
                if (strcmp(pKey, Entry.Key) == 0)
                {
                    Entry.Value = value;
                    return;
                }
            }

            Entries.emplace_back();
            strcpy(Entries[Entries.size() - 1].Key, pKey);
            Entries[Entries.size() - 1].Value = value;
        }

        void RemoveEntry(const char* pKey)
        {
            for (int i = 0; i < Entries.size(); ++i)
            {
                if (strcmp(pKey, Entries[i].Key) == 0)
                {
                    Entries.erase(Entries.begin() + i);
                    return;
                }
            }
        }

        bool GetValue(const char* pKey, int& outValue)
        {
            for (HashEntry& Entry : Entries)
            {
                if (strcmp(pKey, Entry.Key) == 0)
                {
                    outValue = Entry.Value;
                    return true;
                }
            }

            return false;
        }

        std::vector<HashEntry> Entries;
    };

    void RunPuzzle2(const char* pDataFileName)
    {
        File file(pDataFileName);

        const int kMaxLineBuf = 1024 * 32;
        char lineBuf[kMaxLineBuf];

        file.ReadLine(lineBuf, kMaxLineBuf);

        const int kMaxSeqLen = 16;
        const int kMaxSequences = 1024 * 8;

        char sequences[kMaxSequences][kMaxSeqLen];
        int numSequences = Utils::Tokenise(lineBuf, ",", &sequences[0][0], kMaxSequences, kMaxSeqLen);

        const int kNumHashBuckets = 256;
        HashBucket hashMap[kNumHashBuckets];

        for (int i = 0; i < numSequences; ++i)
        {
            char* pInstruction = strchr(sequences[i], '-');

            if (pInstruction != nullptr)
            {
                *pInstruction = 0;
                int hash = HashString(sequences[i]);
                hashMap[hash].RemoveEntry(sequences[i]);
            }
            else
            {
                pInstruction = strchr(sequences[i], '=');
                assert(pInstruction != nullptr);

                const char* pVal = pInstruction + 1;
                *pInstruction = 0;
                
                int val = atoi(pVal);

                int hash = HashString(sequences[i]);
                hashMap[hash].AddEntry(sequences[i], val);
            }
        }

        int sumFocusingPower = 0;

        for (int i = 0; i < kNumHashBuckets; ++i)
        {
            int bucketFocusingPower = 0;

            for (int j = 0; j < hashMap[i].Entries.size(); ++j)
            {
                int entryFocusingPower = (i + 1) * (j + 1) * hashMap[i].Entries[j].Value;
                bucketFocusingPower += entryFocusingPower;
            }

            sumFocusingPower += bucketFocusingPower;
        }

        printf("Sum Focusing Power: %d\n", sumFocusingPower);
    }
}

void Day15::Run()
{
    //RunPuzzle1("Day15_Sample.txt");
    //RunPuzzle1("Day15_Puzzle.txt");

    //RunPuzzle2("Day15_Sample.txt");
    RunPuzzle2("Day15_Puzzle.txt");
}
