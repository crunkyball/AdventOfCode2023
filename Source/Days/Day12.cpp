#include "Day12.h"
#include "File.h"
#include "Utils.h"
#include <cassert>
#include <unordered_map>

enum class SpringState
{
    OPERATIONAL,
    DAMAGED,
    UNKNOWN
};

struct Row
{
    std::vector<int> DamagedGroups;
    std::vector<SpringState> SpringStates;

    bool operator==(const Row& other) const
    {
        return DamagedGroups == other.DamagedGroups && SpringStates == other.SpringStates;
    }

    void Expand()
    {
        std::vector<int> InitialDamagedGroups = DamagedGroups;
        std::vector<SpringState> InitialSpringStates = SpringStates;

        for (int i = 0; i < 4; ++i)
        {
            SpringStates.push_back(SpringState::UNKNOWN);

            for (SpringState state : InitialSpringStates)
            {
                SpringStates.push_back(state);
            }

            for (int damagedGroup : InitialDamagedGroups)
            {
                DamagedGroups.push_back(damagedGroup);
            }
        }
    }
};

namespace std {
    template <>
    struct hash<Row>
    {
        std::size_t operator()(const Row& row) const
        {
            size_t hash = 0;

            for (int damageGroup : row.DamagedGroups)
            {
                hash ^= (std::hash<int>()(damageGroup) << 1);
            }

            for (SpringState state : row.SpringStates)
            {
                hash ^= (std::hash<int>()(static_cast<int>(state)) << 1);
            }

            return hash;
        }
    };
}

namespace
{
    std::unordered_map<Row, uint64_t> cache;

    uint64_t FindPossibleArrangements(Row row, std::unordered_map<Row, uint64_t>& cache)
    {
        const auto& entry = cache.find(row);
        if (entry != cache.end())
        {
            return entry->second;
        }

        if (row.SpringStates.size() > 0)
        {
            if (row.SpringStates[0] == SpringState::OPERATIONAL)
            {
                row.SpringStates.erase(row.SpringStates.begin());
                uint64_t arrangements = FindPossibleArrangements(row, cache);
                cache[row] = arrangements;

                return arrangements;
            }
            else if (row.SpringStates[0] == SpringState::UNKNOWN)
            {
                row.SpringStates[0] = SpringState::OPERATIONAL;
                uint64_t operationalArrangements = FindPossibleArrangements(row, cache);
                cache[row] = operationalArrangements;

                row.SpringStates[0] = SpringState::DAMAGED;
                uint64_t damagedArrangements = FindPossibleArrangements(row, cache);
                cache[row] = damagedArrangements;

                return operationalArrangements + damagedArrangements;
            }
            else if (row.SpringStates[0] == SpringState::DAMAGED)
            {
                int numDamaged = 0;
                for (int i = 0; i < row.SpringStates.size(); ++i)
                {
                    if (row.SpringStates[i] == SpringState::DAMAGED)
                    {
                        numDamaged++;
                    }
                    else if (row.SpringStates[i] == SpringState::UNKNOWN)
                    {
                        row.SpringStates[i] = SpringState::OPERATIONAL;
                        uint64_t operationalArrangements = FindPossibleArrangements(row, cache);
                        cache[row] = operationalArrangements;

                        row.SpringStates[i] = SpringState::DAMAGED;
                        uint64_t damagedArrangements = FindPossibleArrangements(row, cache);
                        cache[row] = damagedArrangements;

                        return operationalArrangements + damagedArrangements;
                    }
                    else
                    {
                        break;
                    }
                }

                if (row.DamagedGroups.size() > 0 && numDamaged == row.DamagedGroups[0])
                {
                    row.SpringStates.erase(row.SpringStates.begin(), row.SpringStates.begin() + numDamaged);
                    row.DamagedGroups.erase(row.DamagedGroups.begin());

                    uint64_t arrangements = FindPossibleArrangements(row, cache);
                    cache[row] = arrangements;

                    return arrangements;
                }
            }
        }

        uint64_t numArrangements = 0;

        if (row.DamagedGroups.size() == 0 && row.SpringStates.size() == 0)
        {
            numArrangements = 1;
        }

        return numArrangements;
    }

    using RowList = std::vector<Row>;

    void ParseRowList(const char* pDataFileName, RowList& outRowList)
    {
        File file(pDataFileName);

        while (!file.IsAtEnd())
        {
            outRowList.emplace_back();
            Row& row = outRowList[outRowList.size() - 1];

            while (!file.IsLineEmpty())
            {
                char c = file.ReadChar();
                file.NextChar();

                if (c == ' ')
                {
                    const int kDamagedBuf = 32;
                    char damagedBuf[kDamagedBuf];

                    file.ReadLine(damagedBuf, kDamagedBuf);

                    const int kMaxDamagedGroups = 32;
                    const int kDamagedGroupBufSize = 8;
                    char damagedGroupsBuf[kMaxDamagedGroups][kDamagedGroupBufSize];

                    int numDamagedGroups = Utils::Tokenise(damagedBuf, ",", &damagedGroupsBuf[0][0], kMaxDamagedGroups, kDamagedGroupBufSize);

                    for (int i = 0; i < numDamagedGroups; ++i)
                    {
                        row.DamagedGroups.push_back(atoi(damagedGroupsBuf[i]));
                    }

                    break;
                }
                else
                {
                    if (c == '#')
                    {
                        row.SpringStates.push_back(SpringState::DAMAGED);
                    }
                    else if (c == '.')
                    {
                        row.SpringStates.push_back(SpringState::OPERATIONAL);
                    }
                    else if (c == '?')
                    {
                        row.SpringStates.push_back(SpringState::UNKNOWN);
                    }
                    else
                    {
                        assert(0);
                    }
                }
            }

            file.NextLine();
        }
    }

    void RunPuzzle(const char* pDataFileName, bool bExpand)
    {
        RowList rowList;
        ParseRowList(pDataFileName, rowList);

        if (bExpand)
        {
            for (Row& row : rowList)
            {
                row.Expand();
            }
        }

        uint64_t sumArrangements = 0;

        int numRows = rowList.size();

        for (int i = 0; i < numRows; ++i)
        {
            std::unordered_map<Row, uint64_t> cache;

            Row& row = rowList[i];

            printf("Processing row %d of %d...\n", i, numRows);
            uint64_t possibleArrangements = FindPossibleArrangements(row, cache);
            cache[row] = possibleArrangements;

            sumArrangements += possibleArrangements;
        }

        printf("Sum Arrangements: %llu\n", sumArrangements);
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

void Day12::Run()
{
    //RunPuzzle1("Day12_Sample.txt");
    //RunPuzzle1("Day12_Puzzle.txt");

    //RunPuzzle2("Day12_Sample.txt");
    RunPuzzle2("Day12_Puzzle.txt");
}
