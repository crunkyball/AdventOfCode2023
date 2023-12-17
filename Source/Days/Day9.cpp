#include "Day9.h"
#include "File.h"
#include "Utils.h"
#include <cassert>
#include <algorithm>

namespace
{
    struct History 
    {
        int TopLineLength = 0;
        int NumLines = 0;

        int GetLineStartIdx(int lineIdx)
        {
            int idx = 0;
            for (int i = 0; i < lineIdx; ++i)
            {
                idx += (TopLineLength - i);
            }

            return idx;
        }

        int GetLineLength(int lineIdx)
        {
            return TopLineLength - lineIdx;
        }

        std::vector<std::int64_t> Values;
    };

    using HistoryList = std::vector<History>;

    void ParseHistories(const char* pDataFilename, HistoryList& outHistoryList)
    {
        File file(pDataFilename);

        while (!file.IsAtEnd())
        {
            outHistoryList.emplace_back();
            History& history = outHistoryList[outHistoryList.size() - 1];

            if (!file.IsLineEmpty())
            {
                const int kLineBufSize = 128;
                char lineBuf[kLineBufSize];

                file.ReadLine(lineBuf, kLineBufSize);

                const int kNumBufSize = 32;
                const int kMaxNums = 32;
                char numBuf[kMaxNums][kNumBufSize];

                int numNums = Utils::Tokenise(lineBuf, " ", &numBuf[0][0], kMaxNums, kNumBufSize);

                for (int i = 0; i < numNums; ++i)
                {
                    history.Values.push_back(atoi(numBuf[i]));
                }

                history.TopLineLength = numNums;
                history.NumLines = 1;
            }

            file.NextLine();
        }        
    }

    bool GenerateNextLine(History& outHistory)
    {
        bool bDone = true;

        int lineStartIdx = outHistory.GetLineStartIdx(outHistory.NumLines - 1);
        int lineLen = outHistory.GetLineLength(outHistory.NumLines - 1);

        for (int i = lineStartIdx; i < (lineStartIdx + lineLen) - 1; ++i)
        {
            int64_t val = outHistory.Values[i + 1] - outHistory.Values[i];
            outHistory.Values.push_back(val);

            if (val != 0)
            {
                bDone = false;
            }
        }

        outHistory.NumLines++;

        return bDone;
    }

    void RunPuzzle(const char* pDataFileName, bool bUsePreviousLine)
    {
        HistoryList historyList;

        ParseHistories(pDataFileName, historyList);

        int64_t sumValLeft = 0;

        for (History& history : historyList)
        {
            while (!GenerateNextLine(history))
            {
            }

            int64_t valLeft = 0;

            if (bUsePreviousLine)
            {
                for (int i = 0; i < history.NumLines; ++i)
                {
                    int thisLineIdx = (history.NumLines - i) - 1;
                    int insertPosIdx = history.GetLineStartIdx(thisLineIdx);

                    int64_t newVal = history.Values[insertPosIdx] - valLeft;
                    valLeft = newVal;

                    history.Values.insert(history.Values.begin() + insertPosIdx, newVal);
                }
            }
            else
            {
                for (int i = 0; i < history.NumLines; ++i)
                {
                    int thisLineIdx = (history.NumLines - i) - 1;
                    int insertPosIdx = history.GetLineStartIdx(thisLineIdx) + history.GetLineLength(thisLineIdx);

                    int64_t newVal = history.Values[insertPosIdx - 1] + valLeft;
                    valLeft = newVal;

                    history.Values.insert(history.Values.begin() + insertPosIdx, newVal);
                }

                history.TopLineLength++;
                history.NumLines++;
            }

            sumValLeft += valLeft;
        }

        printf("SumValLeft: %llu\n", sumValLeft);
    }

    void RunPuzzle1(const char* pDataFileName)
    {
        RunPuzzle(pDataFileName, false);
    }

    void RunPuzzle2(const char* pDataFileName)
    {
        RunPuzzle(pDataFileName, true);
    }
}

void Day9::Run()
{
    //RunPuzzle1("Day9_Sample.txt");
    //RunPuzzle1("Day9_Puzzle.txt");

    //RunPuzzle2("Day9_Sample.txt");
    RunPuzzle2("Day9_Puzzle.txt");
}
