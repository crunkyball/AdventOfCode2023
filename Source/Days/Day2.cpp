#include "Day2.h"
#include "File.h"
#include "Utils.h"
#include <cassert>

namespace
{
    struct Game
    {
        struct Set
        {
            int NumRed = 0;
            int NumGreen = 0;
            int NumBlue = 0;
        };

        int Id;

        std::vector<Set> Sets;
    };

    void ParseGame(const char* gameStr, Game& outGame)
    {
        const int kGameNameBufSize = 16;
        char gameNameBuf[kGameNameBufSize];

        const int kGameBufSize = 256;
        char gameBuf[kGameBufSize];

        Utils::SplitString(gameStr, ": ", gameNameBuf, kGameNameBufSize, gameBuf, kGameBufSize);

        outGame.Id = atoi(gameNameBuf + 5); //strlen("Game ")

        const int kMaxSets = 16;
        const int kSetSize = 32;
        char sets[kMaxSets][kSetSize];

        int numSets = Utils::Tokenise(gameBuf, "; ", &sets[0][0], kMaxSets, kSetSize);

        for (int i = 0; i < numSets; ++i)
        {
            Game::Set set;

            const int kMaxCols = 8;
            const int kColBufSize = 16;
            char cols[kMaxCols][kColBufSize];

            int numCols = Utils::Tokenise(sets[i], ", ", &cols[0][0], kMaxCols, kColBufSize);

            for (int j = 0; j < numCols; ++j)
            {
                const int kColNumSize = 8;
                const int kColNameSize = 16;
                char colNum[kColNumSize];
                char colName[kColNameSize];

                Utils::SplitString(cols[j], " ", colNum, kColNumSize, colName, kColNameSize);

                int numVal = atoi(colNum);

                if (strcmp(colName, "red") == 0)
                {
                    set.NumRed = numVal;
                }
                else if (strcmp(colName, "green") == 0)
                {
                    set.NumGreen = numVal;
                }
                else if (strcmp(colName, "blue") == 0)
                {
                    set.NumBlue = numVal;
                }
                else
                {
                    assert(0);  //What?
                }
            }

            outGame.Sets.push_back(set);
        }
    }

    void ParseGames(const char* pDataFileName, std::vector<Game>& outGames)
    {
        File file(pDataFileName);

        while (!file.IsAtEnd())
        {
            if (!file.IsLineEmpty())
            {
                const int kLineBufSize = 256;
                char lineBuf[kLineBufSize];

                file.ReadLine(lineBuf, kLineBufSize);

                Game game;
                ParseGame(lineBuf, game);

                outGames.push_back(game);
            }

            file.NextLine();
        }
    }

    void RunPuzzle1(const char* pDataFileName)
    {
        std::vector<Game> games;

        ParseGames(pDataFileName, games);

        int sumPossible = 0;

        for (const Game& game : games)
        {
            int maxRed, maxGreen, maxBlue;
            maxRed = maxGreen = maxBlue = 0;

            for (const Game::Set& set : game.Sets)
            {
                maxRed = std::max(maxRed, set.NumRed);
                maxBlue = std::max(maxBlue, set.NumBlue);
                maxGreen = std::max(maxGreen, set.NumGreen);
            }

            if (maxRed <= 12 && maxGreen <= 13 && maxBlue <= 14)
            {
                sumPossible += game.Id;
            }
        }

        printf("Sum Possible: %d\n", sumPossible);
    }

    void RunPuzzle2(const char* pDataFileName)
    {
        std::vector<Game> games;

        ParseGames(pDataFileName, games);

        int sumPower = 0;

        for (const Game& game : games)
        {
            int maxRed, maxGreen, maxBlue;
            maxRed = maxGreen = maxBlue = 0;

            for (const Game::Set& set : game.Sets)
            {
                maxRed = std::max(maxRed, set.NumRed);
                maxBlue = std::max(maxBlue, set.NumBlue);
                maxGreen = std::max(maxGreen, set.NumGreen);
            }

            int setPower = (maxRed * maxBlue * maxGreen);
            sumPower += setPower;
        }

        printf("Sum Power: %d\n", sumPower);
    }
}


void Day2::Run()
{
    //RunPuzzle1("Day2_Sample.txt");
    //RunPuzzle1("Day2_Puzzle.txt");

    //RunPuzzle2("Day2_Sample.txt");
    RunPuzzle2("Day2_Puzzle.txt");
}
