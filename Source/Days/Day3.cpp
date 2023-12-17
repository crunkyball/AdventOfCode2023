#include "Day3.h"
#include "File.h"
#include "Utils.h"
#include <cassert>

namespace
{
    struct Schematic
    {
        struct Num
        {
            int XStart = -1;
            int XEnd = -1;
            int Y = -1;
            int Val = -1;
        };

        struct Symbol
        {
            int X = -1;
            int Y = -1;
            char C = -1;
        };

        void ParseFromFile(const char* pDataFileName)
        {
            //Raw data.
            File file(pDataFileName);

            LineLen = 0;

            while (!file.IsAtEnd())
            {
                int lineLen = 0;

                while (!file.IsLineEmpty() && !file.IsAtEnd())
                {
                    Data.push_back(file.ReadChar());
                    file.NextChar();
                    lineLen++;
                }

                assert(LineLen == 0 || lineLen == LineLen);

                LineLen = lineLen;

                file.NextLine();
            }

            NumLines = Data.size() / LineLen;

            //Parsed into useful structures.
            for (int y = 0; y < NumLines; ++y)
            {
                int numStart = -1;
                int numEnd = -1;

                for (int x = 0; x < LineLen; ++x)
                {
                    char c = Get(x, y);

                    if (c >= '0' && c <= '9')
                    {
                        if (numStart == -1)
                        {
                            numStart = x;
                            numEnd = x;
                        }
                        else
                        {
                            numEnd++;
                        }
                    }
                    else
                    {
                        if (numStart != -1)
                        {
                            Num num;
                            num.XStart = numStart;
                            num.XEnd = numEnd;
                            num.Y = y;
                            num.Val = GetAsNum(numStart, numEnd, y);

                            Nums.push_back(num);

                            numStart = numEnd = -1;
                        }

                        if (IsSymbol(c))
                        {
                            Symbol symbol;
                            symbol.X = x;
                            symbol.Y = y;
                            symbol.C = c;

                            Symbols.push_back(symbol);
                        }
                    }
                }

                if (numStart != -1)
                {
                    Num num;
                    num.XStart = numStart;
                    num.XEnd = numEnd;
                    num.Y = y;
                    num.Val = GetAsNum(numStart, numEnd, y);

                    Nums.push_back(num);
                }
            }
        }

        char Get(int x, int y) const
        {
            if (x >= 0 && y >= 0 && x < LineLen && y < NumLines)
            {
                return Data[(y * LineLen) + x];
            }

            assert(0);
            return -1;
        }

        int GetAsNum(int xStart, int xEnd, int y)
        {
            char buf[8];
            int len = (xEnd - xStart) + 1;
            assert(len < 8);
            for (int i = 0; i < len; ++i)
            {
                buf[i] = Get(xStart + i, y);
            }

            buf[len] = 0;

            return atoi(buf);
        }

        bool IsNum(char c)
        {
            return (c >= '0' && c <= '9');
        }

        bool IsSymbol(char c)
        {
            if (c == '.' || IsNum(c))
            {
                return false;
            }

            return true;
        }

        const Num* FindNum(int x, int y)
        {
            for (const Num& num : Nums)
            {
                if (x <= num.XEnd && x >= num.XStart && num.Y == y)
                {
                    return &num;
                }
            }

            return nullptr;
        }

        bool IsNumPart(const Num& num)
        {
            for (int cy = std::max(num.Y - 1, 0); cy <= std::min(num.Y + 1, NumLines - 1); ++cy)
            {
                for (int cx = std::max(num.XStart - 1, 0); cx <= std::min(num.XEnd + 1, LineLen - 1); ++cx)
                {
                    char c = Get(cx, cy);

                    if (IsSymbol(c))
                    {
                        return true;
                    }
                }
            }

            //If we get here then it's valid.
            return false;
        }

        void GetAdjacentNumbers(const Symbol& symbol, std::vector<const Num*>& outAdjacent)
        {
            for (int cy = std::max(symbol.Y - 1, 0); cy <= std::min(symbol.Y + 1, NumLines - 1); ++cy)
            {
                for (int cx = std::max(symbol.X - 1, 0); cx <= std::min(symbol.X + 1, LineLen - 1); ++cx)
                {
                    char c = Get(cx, cy);

                    if (IsNum(c))
                    {
                        const Num* pNum = FindNum(cx, cy);
                        assert(pNum);

                        outAdjacent.push_back(pNum);

                        cx = pNum->XEnd;
                    }
                }
            }
        }
    
        std::vector<Num> Nums;
        std::vector<Symbol> Symbols;

    private:
        std::vector<char> Data;

        int NumLines;
        int LineLen;
    };

    void RunPuzzle1(const char* pDataFileName)
    {
        Schematic schematic;
        schematic.ParseFromFile(pDataFileName);

        int sum = 0;

        for (Schematic::Num& num : schematic.Nums)
        {
            if (schematic.IsNumPart(num))
            {
                sum += num.Val;
            }
        }

        printf("Sum: %d\n", sum);
    }

    void RunPuzzle2(const char* pDataFileName)
    {
        Schematic schematic;
        schematic.ParseFromFile(pDataFileName);

        int ratioSum = 0;

        for (Schematic::Symbol& symbol : schematic.Symbols)
        {
            if (symbol.C == '*')
            {
                std::vector<const Schematic::Num*> adjacent;
                schematic.GetAdjacentNumbers(symbol, adjacent);
                
                if (adjacent.size() == 2)
                {
                    int ratio = adjacent[0]->Val * adjacent[1]->Val;

                    ratioSum += ratio;
                }
            }
        }

        printf("Ratio Sum: %d\n", ratioSum);
    }
}


void Day3::Run()
{
    //RunPuzzle1("Day3_Sample.txt");
    //RunPuzzle1("Day3_Puzzle.txt");

    //RunPuzzle2("Day3_Sample.txt");
    RunPuzzle2("Day3_Puzzle.txt");
}
