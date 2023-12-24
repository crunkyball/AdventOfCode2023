#include "Day16.h"
#include "File.h"
#include "Utils.h"
#include <cassert>

namespace
{
    enum class Direction
    {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    struct Contraption
    {
        int Width = 0;
        int Height = 0;
        std::vector<char> Entries;
        std::vector<bool> VisitedEntries;
        std::vector<int> SplitLocationsBitmap;

        char Get(int x, int y)
        {
            return Entries[(y * Width) + x];
        }

        void Draw()
        {
            for (int y = 0; y < Height; ++y)
            {
                for (int x = 0; x < Width; ++x)
                {
                    printf("%c", Get(x, y));
                }

                printf("\n");
            }

            printf("\n");
        }

        void DrawVisited()
        {
            for (int y = 0; y < Height; ++y)
            {
                for (int x = 0; x < Width; ++x)
                {
                    if (VisitedEntries[(y * Width) + x])
                    {
                        printf("#");
                    }
                    else
                    {
                        //printf("%c", Entries[(y * Width) + x]);
                        printf(".");
                    }
                }

                printf("\n");
            }

            printf("\n");
        }

        bool AlreadySplitAt(int x, int y, Direction dir)
        {
            int idx = (y * Width) + x;
            int dirBit = (1 << static_cast<int>(dir));

            if ((SplitLocationsBitmap[idx] & dirBit) != 0)
            {
                return true;
            }

            return false;
        }

        void SplitAt(int x, int y, Direction dir)
        {
            int idx = (y * Width) + x;
            int dirBit = (1 << static_cast<int>(dir));

            SplitLocationsBitmap[idx] |= dirBit;
        }

        bool ShouldIgnore(char c, Direction dir)
        {
            if (c == '.')
            {
                return true;
            }
            else if (c == '-' && (dir == Direction::LEFT || dir == Direction::RIGHT))
            {
                return true;
            }
            else if (c == '|' && (dir == Direction::UP || dir == Direction::DOWN))
            {
                return true;
            }

            return false;
        }

        void Reset()
        {
            for (int i = 0; i < VisitedEntries.size(); ++i)
            {
                VisitedEntries[i] = false;
            }

            for (int i = 0; i < SplitLocationsBitmap.size(); ++i)
            {
                SplitLocationsBitmap[i] = 0;
            }
        }

        void RunBeam(int x, int y, Direction dir)
        {
            while (1)
            {
                if (x < 0 || y < 0 || x >= Width || y >= Height)
                {
                    return;
                }

                int idx = (y * Width) + x;
                VisitedEntries[idx] = true;

                char c = Get(x, y);

                if (ShouldIgnore(c, dir))
                {
                    if (dir == Direction::RIGHT)
                    {
                        x++;
                    }
                    else if (dir == Direction::LEFT)
                    {
                        x--;
                    }
                    else if (dir == Direction::UP)
                    {
                        y--;
                    }
                    else if (dir == Direction::DOWN)
                    {
                        y++;
                    }
                }
                else if (c == '\\')
                {
                    if (dir == Direction::RIGHT)
                    {
                        dir = Direction::DOWN;
                        y++;
                    }
                    else if (dir == Direction::LEFT)
                    {
                        dir = Direction::UP;
                        y--;
                    }
                    else if (dir == Direction::DOWN)
                    {
                        dir = Direction::RIGHT;
                        x++;
                    }
                    else if (dir == Direction::UP)
                    {
                        dir = Direction::LEFT;
                        x--;
                    }
                }
                else if (c == '/')
                {
                    if (dir == Direction::RIGHT)
                    {
                        dir = Direction::UP;
                        y--;
                    }
                    else if (dir == Direction::LEFT)
                    {
                        dir = Direction::DOWN;
                        y++;
                    }
                    else if (dir == Direction::DOWN)
                    {
                        dir = Direction::LEFT;
                        x--;
                    }
                    else if (dir == Direction::UP)
                    {
                        dir = Direction::RIGHT;
                        x++;
                    }
                }
                else if (c == '-')
                {
                    assert(dir == Direction::DOWN || dir == Direction::UP);
                    
                    if (!AlreadySplitAt(x, y, dir))
                    {
                        SplitAt(x, y, dir);

                        RunBeam(x-1, y, Direction::LEFT);                        
                        RunBeam(x+1, y, Direction::RIGHT);
                    }

                    return;
                }
                else if (c == '|')
                {
                    assert(dir == Direction::LEFT || dir == Direction::RIGHT);

                    if (!AlreadySplitAt(x, y, dir))
                    {
                        SplitAt(x, y, dir);

                        RunBeam(x, y-1, Direction::UP);
                        RunBeam(x, y+1, Direction::DOWN);                        
                    }

                    return;
                }
            }

        }
    };


    void ParseContraption(const char* pDataFileName, Contraption& outContraption)
    {
        File file(pDataFileName);

        int y = 0;

        while (!file.IsAtEnd())
        {
            int x = 0;

            while (!file.IsLineEmpty() && !file.IsAtEnd())
            {
                char entry = file.ReadChar();
                outContraption.Entries.push_back(entry);
                outContraption.VisitedEntries.push_back(false);
                outContraption.SplitLocationsBitmap.push_back(0);

                x++;

                file.NextChar();
            }

            assert(outContraption.Width == 0 || outContraption.Width == x);
            outContraption.Width = x;
            y++;

            file.NextLine();
        }

        outContraption.Height = y;
    }

    void RunPuzzle1(const char* pDataFileName)
    {
        Contraption contraption;
        ParseContraption(pDataFileName, contraption);

        //contraption.Draw();

        contraption.RunBeam(0, 0, Direction::RIGHT);

        //contraption.DrawVisited();

        int numEnergized = 0;

        for (int i = 0; i < contraption.VisitedEntries.size(); ++i)
        {
            if (contraption.VisitedEntries[i])
            {
                numEnergized++;
            }
        }

        printf("Num Energized: %d\n", numEnergized);
    }

    void RunPuzzle2(const char* pDataFileName)
    {
        Contraption contraption;
        ParseContraption(pDataFileName, contraption);

        int mostEnergized = 0;

        for (int i = 0 ; i < (contraption.Width * 2) + (contraption.Height * 2); ++i)
        {
            Direction dir;
            int x, y;

            if (i >= ((contraption.Width * 2) + contraption.Height))
            {
                //In from the left.
                dir = Direction::RIGHT;
                x = 0;
                y = i - ((contraption.Width * 2) + contraption.Height);
            }
            else if (i >= (contraption.Width + contraption.Height))
            {
                //Up from the bottom.
                dir = Direction::UP;
                y = (contraption.Height - 1);
                x = i - (contraption.Width + contraption.Height);
            }
            else if (i >= contraption.Width)
            {
                //In from the right.
                dir = Direction::LEFT;
                x = (contraption.Width - 1);
                y = i - contraption.Width;
            }
            else
            {
                //Down from the top.
                dir = Direction::DOWN;
                y = 0;
                x = i;
            }            

            contraption.RunBeam(x, y, dir);

            int numEnergized = 0;

            for (int i = 0; i < contraption.VisitedEntries.size(); ++i)
            {
                if (contraption.VisitedEntries[i])
                {
                    numEnergized++;
                }
            }

            contraption.Reset();

            mostEnergized = std::max(numEnergized, mostEnergized);
        }

        printf("Most Energized: %d\n", mostEnergized);
    }
}

void Day16::Run()
{
    //RunPuzzle1("Day16_Sample.txt");
    //RunPuzzle1("Day16_Puzzle.txt");

    //RunPuzzle2("Day16_Sample.txt");
    RunPuzzle2("Day16_Puzzle.txt");
}
