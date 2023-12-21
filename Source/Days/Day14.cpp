#include "Day14.h"
#include "File.h"
#include "Utils.h"
#include <cassert>
#include <windows.h>

namespace
{
    enum class Entry
    {
        EMPTY,
        ROUND,
        CUBE
    };

    enum class Direction
    {
        NORTH,
        WEST,
        SOUTH,
        EAST
    };

    struct Dish
    {
        int Width = 0;
        int Height = 0;
        std::vector<Entry> Entries;

        void Draw()
        {
            for (int y = 0; y < Height; ++y)
            {
                for (int x = 0; x < Width; ++x)
                {
                    int idx = (y * Width) + x;

                    if (Entries[idx] == Entry::EMPTY)
                    {
                        printf(".");
                    }
                    else if (Entries[idx] == Entry::ROUND)
                    {
                        printf("O");
                    }
                    else if (Entries[idx] == Entry::CUBE)
                    {
                        printf("#");
                    }
                }

                printf("\n");
            }

            printf("\n");
        }

        void Tilt(Direction direction)
        {
            struct TiltGroup
            {
                int Floor = -1;
                int NumRound = -1;
            };

            //Assumption so I can treat both X and Y dimensions as the same.
            assert(Width == Height);

            //i and j were originally x and y but I've changed them to be ambiguous now we can tilt in any direction.
            for (int i = 0; i < Width; ++i)
            {
                std::vector<TiltGroup> groups;

                TiltGroup currentTiltGroup;

                for (int j = 0; j < Height; ++j)
                {
                    int idx = -1;
                    if (direction == Direction::NORTH)
                    {
                        idx = (j * Width) + i;
                    }
                    else if (direction == Direction::SOUTH)
                    {
                        idx = ((Height - (j + 1)) * Width) + i;
                    }
                    else if (direction == Direction::WEST)
                    {
                        idx = (i * Height) + j;
                    }
                    else if (direction == Direction::EAST)
                    {
                        idx = (i * Height) + (Width - (j + 1));
                    }

                    if (Entries[idx] != Entry::CUBE)
                    {
                        if (currentTiltGroup.Floor == -1)
                        {
                            currentTiltGroup.Floor = j;
                            currentTiltGroup.NumRound = 0;
                        }

                        if (Entries[idx] == Entry::ROUND)
                        {
                            currentTiltGroup.NumRound++;

                            Entries[idx] = Entry::EMPTY;
                        }
                    }
                    else
                    {
                        if (currentTiltGroup.Floor != -1)
                        {
                            groups.push_back(currentTiltGroup);
                            currentTiltGroup.Floor = -1;
                            currentTiltGroup.NumRound = -1;
                        }
                    }
                }

                if (currentTiltGroup.Floor != -1)
                {
                    groups.push_back(currentTiltGroup);
                }

                //Now replace the rounds.
                for (const TiltGroup& group : groups)
                {
                    for (int j = group.Floor; j < group.Floor + group.NumRound; ++j)
                    {
                        int idx = -1;

                        if (direction == Direction::NORTH)
                        {
                            idx = (j * Width) + i;
                        }
                        else if (direction == Direction::SOUTH)
                        {
                            idx = ((Height - (j + 1)) * Width) + i;
                        }
                        else if (direction == Direction::WEST)
                        {
                            idx = (i * Height) + j;
                        }
                        else if (direction == Direction::EAST)
                        {
                            idx = (i * Height) + (Width - (j + 1));
                        }

                        Entries[idx] = Entry::ROUND;
                    }
                }
            }
        }

        int CalculateLoad()
        {
            int load = 0;

            for (int y = 0; y < Height; ++y)
            {
                int numRound = 0;

                for (int x = 0; x < Width; ++x)
                {
                    int idx = (y * Width) + x;

                    if (Entries[idx] == Entry::ROUND)
                    {
                        numRound++;
                    }
                }

                load += (numRound * (Height - y));
            }

            return load;
        }
    };

    void ParseDish(const char* pDataFileName, Dish& outDish)
    {
        File file(pDataFileName);

        int y = 0;

        while (!file.IsAtEnd())
        {
            int x = 0;

            while (!file.IsLineEmpty() && !file.IsAtEnd())
            {
                char posChar = file.ReadChar();

                Entry entry;
                
                switch (posChar)
                {
                    case '.': entry = Entry::EMPTY; break;
                    case '#': entry = Entry::CUBE; break;
                    case 'O': entry = Entry::ROUND; break;
                    default: assert(0);
                }

                outDish.Entries.push_back(entry);

                x++;

                file.NextChar();
            }

            assert(outDish.Width == 0 || outDish.Width == x);
            outDish.Width = x;
            y++;

            file.NextLine();
        }

        outDish.Height = y;
    }

    void RunPuzzle1(const char* pDataFileName)
    {
        Dish dish;
        ParseDish(pDataFileName, dish);

        //dish.Draw();
        dish.Tilt(Direction::NORTH);
        //dish.Draw();

        int load = dish.CalculateLoad();

        printf("Load: %d\n", load);
    }

    int CheckLoopPoint(const std::vector<int>& loadResults, int& outLength)
    {
        int loopPoint = -1;

        //Crude assumption that there'll be a loop length less than 50.
        const int kMaxLoopLength = 50;

        //There's probably a better way of doing this.

        //First find the loop point.
        for (int i = 0; i < loadResults.size() && loopPoint == -1; ++i)
        {
            for (int j = 1; j < loadResults.size() && loopPoint == -1; ++j)
            {
                if (i != j && loadResults[i] == loadResults[j])
                {
                    bool bValid = true;

                    for (int k = 0; k < kMaxLoopLength; ++k)
                    {
                        int idxa = i + k;
                        int idxb = j + k;

                        if (idxa >= loadResults.size() || idxb >= loadResults.size() || loadResults[idxa] != loadResults[idxb])
                        {
                            bValid = false;
                            break;
                        }
                    }

                    if (bValid)
                    {
                        loopPoint = i;
                    }
                }
            }
        }

        outLength = -1;

        if (loopPoint != -1)
        {
            //Then find the loop length.
            for (int len = 2; len < kMaxLoopLength; ++len)
            {
                bool bValid = true;

                for (int i = 0; i < kMaxLoopLength; ++i)
                {
                    int idx = i % len;

                    if (loadResults[loopPoint + i] != loadResults[loopPoint + idx])
                    {
                        bValid = false;
                        break;
                    }
                }

                if (bValid)
                {
                    outLength = len;
                    break;
                }
            }
        }

        return loopPoint;
    }

    void RunPuzzle2(const char* pDataFileName)
    {
        Dish dish;
        ParseDish(pDataFileName, dish);

        int total = 1000000000;

        std::vector<int> loadResults;

        int load = -1;

        for (int i = 0; i < total && load == -1; ++i)
        {
            for (int dirIdx = 0; dirIdx < 4; ++dirIdx)
            {                
                Direction dir = static_cast<Direction>(dirIdx);
                dish.Tilt(dir);
            }

            int thisLoad = dish.CalculateLoad();

            loadResults.push_back(thisLoad);

            if (loadResults.size() > 1)
            {
                int loopLength;
                int loopPoint = CheckLoopPoint(loadResults, loopLength);

                if (loopPoint != -1)
                {
                    int loadPoint = loopPoint + ((total - loopPoint) % loopLength) - 1;
                    load = loadResults[loadPoint];
                    break;
                }
            }
        }

        printf("Load: %d\n", load);
    }
}

void Day14::Run()
{
    //RunPuzzle1("Day14_Sample.txt");
    //RunPuzzle1("Day14_Puzzle.txt");

    //RunPuzzle2("Day14_Sample.txt");
    RunPuzzle2("Day14_Puzzle.txt");
}
