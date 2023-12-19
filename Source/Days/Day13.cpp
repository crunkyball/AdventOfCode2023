#include "Day13.h"
#include "File.h"
#include "Utils.h"
#include <cassert>

namespace
{
    struct Map
    {
        int Width = 0;
        int Height = 0;
        std::vector<bool> Entries;

        std::vector<uint64_t> RowBitmap;
        std::vector<uint64_t> ColBitmap;

        bool CheckRowMirrorFrom(int y) const
        {
            bool bMirror = true;

            for (int gap = 0; ; gap++)
            {
                int y1 = y - gap;
                int y2 = y + 1 + gap;

                if (y1 < 0 || y2 >= Height)
                {
                    break;
                }
                else if (RowBitmap[y1] != RowBitmap[y2])
                {
                    bMirror = false;
                    break;
                }
            }

            return bMirror;
        }

        bool CheckColMirrorFrom(int x) const
        {
            bool bMirror = true;

            for (int gap = 0; ; gap++)
            {
                int x1 = x - gap;
                int x2 = x + 1 + gap;

                if (x1 < 0 || x2 >= Width)
                {
                    break;
                }
                else if (ColBitmap[x1] != ColBitmap[x2])
                {
                    bMirror = false;
                    break;
                }
            }

            return bMirror;
        }

        void FlipRowBit(int rowIdx, int bitIdx)
        {
            RowBitmap[rowIdx] ^= (1 << bitIdx);
        }

        void FlipColBit(int colIdx, int bitIdx)
        {
            ColBitmap[colIdx] ^= (1 << bitIdx);
        }

        void PopulateBitmaps()
        {
            for (int y = 0; y < Height; ++y)
            {
                uint64_t rowVal = 0;
                for (int x = 0; x < Width; ++x)
                {
                    int idx = (y * Width) + x;
                    if (Entries[idx])
                    {
                        rowVal |= (1 << x);
                    }
                    else
                    {
                        rowVal &= ~(1 << x);
                    }
                }

                RowBitmap.push_back(rowVal);
            }

            for (int x = 0; x < Width; ++x)
            {
                uint64_t colVal = 0;
                for (int y = 0; y < Height; ++y)
                {
                    int idx = (y * Width) + x;
                    if (Entries[idx])
                    {
                        colVal |= (1 << y);
                    }
                    else
                    {
                        colVal &= ~(1 << y);
                    }
                }

                ColBitmap.push_back(colVal);
            }
        }
    };

    using MapList = std::vector<Map>;

    void ParseMapList(const char* pDataFilename, MapList& outMapList)
    {
        File file(pDataFilename);

        int y = 0;

        Map map;

        while (!file.IsAtEnd())
        {
            int x = 0;

            while (!file.IsLineEmpty() && !file.IsAtEnd())
            {
                char posChar = file.ReadChar();

                bool isRock = posChar == '#';

                map.Entries.push_back(isRock);

                x++;

                file.NextChar();
            }

            assert(map.Width == 0 || map.Width == x);
            map.Width = x;
            y++;

            file.NextLine();

            if (file.IsLineEmpty() || file.IsAtEnd())
            {
                map.Height = y;
                map.PopulateBitmaps();

                outMapList.push_back(map);
                
                map.Width = map.Height = 0;
                map.Entries.clear();
                map.ColBitmap.clear();
                map.RowBitmap.clear();

                y = 0;

                file.NextLine();
            }
        }
    }

    int FindRowMirror(const Map& map, int ignoreRow = -1)
    {
        for (int y = 0; y < map.Height - 1; ++y)
        {
            if (ignoreRow == y)
            {
                continue;
            }

            if (map.RowBitmap[y] == map.RowBitmap[y + 1])
            {
                if (map.CheckRowMirrorFrom(y))
                {
                    return y;
                }
            }
        }

        return -1;
    }

    int FindColMirror(const Map& map, int ignoreCol = -1)
    {
        for (int x = 0; x < map.Width - 1; ++x)
        {
            if (ignoreCol == x)
            {
                continue;
            }

            if (map.ColBitmap[x] == map.ColBitmap[x + 1])
            {
                if (map.CheckColMirrorFrom(x))
                {
                    return x;
                }
            }
        }
        
        return -1;
    }

    void RunPuzzle1(const char* pDataFileName)
    {
        MapList mapList;
        ParseMapList(pDataFileName, mapList);

        int sum = 0;

        for (const Map& map : mapList)
        {
            int val = -1;

            int rowMirror = FindRowMirror(map);

            if (rowMirror != -1)
            {
                val = (rowMirror + 1) * 100;
            }
            else
            {
                int colMirror = FindColMirror(map);

                if (colMirror != -1)
                {
                    val = (colMirror + 1);
                }
            }

            sum += val;
        }

        printf("Sum: %d\n", sum);
    }

    void RunPuzzle2(const char* pDataFileName)
    {
        MapList mapList;
        ParseMapList(pDataFileName, mapList);

        int sum = 0;

        for (Map& map : mapList)
        {
            int rowMirror = -1;
            int colMirror = -1;

            int originalRowMirror = FindRowMirror(map);
            int originalColMirror = FindColMirror(map);

            for (int y = 0; y < map.Height; ++y)
            {
                for (int x = 0; x < map.Width; ++x)
                {
                    map.FlipRowBit(y, x);

                    int thisRowMirror = FindRowMirror(map, originalRowMirror);

                    if (thisRowMirror != -1)
                    {
                        rowMirror = thisRowMirror;
                    }

                    map.FlipRowBit(y, x);
                }
            }

            int val = -1;

            if (rowMirror != -1)
            {
                val = (rowMirror + 1) * 100;
            }
            else
            {
                for (int x = 0; x < map.Width; ++x)
                {
                    for (int y = 0; y < map.Height; ++y)
                    {
                        map.FlipColBit(x, y);

                        int thisColMirror = FindColMirror(map, originalColMirror);

                        if (thisColMirror != -1)
                        {
                            colMirror = thisColMirror;
                        }

                        map.FlipColBit(x, y);
                    }
                }

                if (colMirror != -1)
                {
                    val = (colMirror + 1);
                }
            }

            sum += val;
        }

        printf("Sum: %d\n", sum);
    }
}

void Day13::Run()
{
    //RunPuzzle1("Day13_Sample.txt");
    //RunPuzzle1("Day13_Puzzle.txt");

    //RunPuzzle2("Day13_Sample.txt");
    RunPuzzle2("Day13_Puzzle.txt");
}
