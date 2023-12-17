#include "Day11.h"
#include "File.h"
#include "Utils.h"
#include <cassert>
#include <algorithm>
#include <cmath>
#include <map>

namespace
{
    struct Vector2
    {
        Vector2 operator -(const Vector2& other) const
        {
            Vector2 vec;
            vec.X = X - other.X;
            vec.Y = Y - other.Y;

            return vec;
        }

        float LengthSquared() const
        {
            return (X*X + Y*Y);
        }

        float Length() const
        {
            return sqrt(LengthSquared());
        }

        int ManhattanDistance(const Vector2& other) const
        {
            return std::abs(X - other.X) + std::abs(Y - other.Y);
        }

        int X;
        int Y;
    };

    struct Galaxy
    {
        std::vector<int64_t> Dists;
        Vector2 Pos;
    };

    using GalaxyList = std::vector<Galaxy>;
    
    std::vector<int> EmptyRows;
    std::vector<int> EmptyCols;

    struct Grid
    {
        int Width = 0;
        int Height = 0;
        std::vector<bool> Entries;

        bool IsRowEmpty(int rowIdx) const
        {
            int y = rowIdx * Width;

            for (int i = y; i < (y + Width); ++i)
            {
                if (Entries[i])
                {
                    return false;
                }
            }

            return true;
        }

        bool IsColumnEmpty(int colIdx) const
        {
            for (int y = 0; y < Height; ++y)
            {
                int i = (y * Width) + colIdx;

                if (Entries[i])
                {
                    return false;
                }
            }

            return true;
        }
    };

    void ParseData(const char* pDataFileName, Grid& outGrid, GalaxyList& outGalaxyList)
    {
        File file(pDataFileName);

        int y = 0;

        while(!file.IsAtEnd())
        {
            int x = 0;

            while (!file.IsLineEmpty() && !file.IsAtEnd())
            {
                char posChar = file.ReadChar();

                bool isGalaxy = posChar == '#';

                outGrid.Entries.push_back(isGalaxy);

                if (isGalaxy)
                {
                    Galaxy galaxy;
                    galaxy.Pos = { x, y };
                    outGalaxyList.push_back(galaxy);
                }

                x++;

                file.NextChar();
            }

            assert(outGrid.Width == 0 || outGrid.Width == x);
            outGrid.Width = x;
            y++;

            file.NextLine();
        }

        outGrid.Height = y;
    }

    void FindEmptySpace(const Grid& grid)
    {
        EmptyRows.clear();
        EmptyCols.clear();

        for (int i = 0; i < grid.Height; ++i)
        {
            if (grid.IsRowEmpty(i))
            {
                EmptyRows.push_back(i);
            }
        }

        for (int i = 0; i < grid.Width; ++i)
        {
            if (grid.IsColumnEmpty(i))
            {
                EmptyCols.push_back(i);
            }
        }
    }

    void EmptySpaceBetween(const Vector2& a, const Vector2& b, int& outNumEmptyRows, int& outNumEmptyCols)
    {
        outNumEmptyCols = 0;
        outNumEmptyRows = 0;
        
        int minX, maxX, minY, maxY;
        minX = std::min(a.X, b.X);
        maxX = std::max(a.X, b.X);
        minY = std::min(a.Y, b.Y);
        maxY = std::max(a.Y, b.Y);

        for (int row : EmptyRows)
        {
            if (row > minY && row < maxY)
            {
                outNumEmptyRows++;
            }
        }

        for (int col : EmptyCols)
        {
            if (col > minX && col < maxX)
            {
                outNumEmptyCols++;
            }
        }
    }

    void CalculateDistances(GalaxyList& outGalaxyList, int emptySpaceSize)
    {
        for (int i = 0; i < outGalaxyList.size(); ++i)
        {
            for (int j = 0; j < outGalaxyList.size(); ++j)
            {
                int64_t dist = (outGalaxyList[i].Pos.ManhattanDistance(outGalaxyList[j].Pos));

                int numEmptyRows, numEmptyCols;
                EmptySpaceBetween(outGalaxyList[i].Pos, outGalaxyList[j].Pos, numEmptyRows, numEmptyCols);

                dist += ((numEmptyRows + numEmptyCols) * emptySpaceSize);

                outGalaxyList[i].Dists.push_back(dist);
            }
        }
    }

    void RunPuzzle(const char* pDataFileName, int emptySpaceSize)
    {
        Grid grid;
        GalaxyList galaxyList;
        ParseData(pDataFileName, grid, galaxyList);

        FindEmptySpace(grid);

        CalculateDistances(galaxyList, emptySpaceSize);

        int64_t sumDist = 0;

        std::map<std::pair<int, int>, int64_t> pairDists;

        for (int i = 0; i < galaxyList.size(); ++i)
        {
            for (int j = 0; j < galaxyList[i].Dists.size(); ++j)
            {
                if (i != j)
                {
                    std::pair<int, int> key = { std::min(i, j), std::max(i, j) };

                    if (pairDists.find(key) == pairDists.end())
                    {
                        sumDist += galaxyList[i].Dists[j];
                        pairDists[key] = galaxyList[i].Dists[j];
                    }
                }
            }
        }

        printf("Sum Dists: %lld\n", sumDist);
    }

    void RunPuzzle1(const char* pDataFileName)
    {
        RunPuzzle(pDataFileName, 1);
    }

    void RunPuzzle2(const char* pDataFileName)
    {
        RunPuzzle(pDataFileName, 999999);
    }
}

void Day11::Run()
{
    //RunPuzzle1("Day11_Sample.txt");
    //RunPuzzle1("Day11_Puzzle.txt");

    //RunPuzzle2("Day11_Sample.txt");
    RunPuzzle2("Day11_Puzzle.txt");
}
