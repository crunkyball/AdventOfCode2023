#include "Day6.h"
#include "File.h"
#include "Utils.h"
#include <cassert>

namespace
{
    struct Race
    {
        uint64_t Time;
        uint64_t Distance;
    };

    using RaceList = std::vector<Race>;

    //Data is simple for this one so I'm not bothering to parse.
    RaceList Sample1Races = {
        { 7, 9 },
        { 15, 40 },
        { 30, 200 }
    };

    RaceList Puzzle1Races = {
        { 41, 249 },
        { 77, 1362 },
        { 70, 1127 },
        { 96, 1011 }
    };

    Race Sample2Race = { 71530, 940200 };
    Race Puzzle2Race = { 41777096, 249136211271011 };

    uint64_t GetWaysToWin(const Race& race)
    {
        uint64_t numValid = 0;

        for (uint64_t buttSecs = 0; buttSecs <= race.Time; ++buttSecs)
        {
            uint64_t distanceTraveled = buttSecs * (race.Time - buttSecs);

            if (distanceTraveled > race.Distance)
            {
                numValid++;
            }
        }

        return numValid;
    }

    void RunPuzzle1(const RaceList& races)
    {
        uint64_t validProduct = 1;

        for (const Race& race : races)
        {
            uint64_t numValid = GetWaysToWin(race);
            validProduct *= numValid;
        }

        printf("Valid: %llu\n", validProduct);
    }

    void RunPuzzle2(const Race& race)
    {
        uint64_t waysToWin = GetWaysToWin(race);

        printf("Ways to win: %llu\n", waysToWin);
    }
}

void Day6::Run()
{
    //RunPuzzle1(Sample1Races /*"Day6_Sample.txt"*/);
    //RunPuzzle1(Puzzle1Races /*"Day6_Puzzle.txt"*/);

    //RunPuzzle2(Sample2Race /*"Day6_Sample.txt"*/);
    RunPuzzle2(Puzzle2Race /*"Day6_Puzzle.txt"*/);
}
