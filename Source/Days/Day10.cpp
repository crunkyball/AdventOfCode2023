#include "Day10.h"
#include "File.h"
#include "Utils.h"
#include <cassert>
#include <algorithm>

namespace
{
    enum class Block
    {
        INVALID,
        GROUND,
        START,
        VERTICAL,
        HORIZONTAL,
        NORTH_EAST,
        NORTH_WEST,
        SOUTH_WEST,
        SOUTH_EAST
    };

    enum Direction
    {
        ANY,
        NORTH,
        SOUTH,
        EAST,
        WEST
    };

    struct Maze
    {
        int Width = 0;
        int StartIdx = -1;
        std::vector<Block> Grid;

        std::vector<int> Loop;

        int GetBlockIdx(int x, int y)
        {
            return (y * Width) + x;
        }

        void GetBlockPos(int idx, int& outX, int& outY)
        {
            outX = idx % Width;
            outY = idx / Width;
        }

        Block GetBlock(int x, int y)
        {
            int idx = GetBlockIdx(x, y);

            if (idx >= 0 && idx < Grid.size())
            {
                return Grid[idx];
            }
            
            return Block::INVALID;
        }

        bool CanBlockConnectNorth(Block block)
        {
            if (block == Block::VERTICAL || block == Block::NORTH_EAST || block == Block::NORTH_WEST)
            {
                return true;
            }

            return false;
        }

        bool CanBlockConnectSouth(Block block)
        {
            if (block == Block::VERTICAL || block == Block::SOUTH_EAST || block == Block::SOUTH_WEST)
            {
                return true;
            }

            return false;
        }

        bool CanBlockConnectEast(Block block)
        {
            if (block == Block::HORIZONTAL || block == Block::NORTH_EAST || block == Block::SOUTH_EAST)
            {
                return true;
            }

            return false;
        }

        bool CanBlockConnectWest(Block block)
        {
            if (block == Block::HORIZONTAL || block == Block::NORTH_WEST || block == Block::SOUTH_WEST)
            {
                return true;
            }

            return false;
        }

        Direction GetFollowingDir(Block block, Direction dir)
        {
            switch (block)
            {
                case Block::VERTICAL:
                    if (dir == Direction::ANY || dir == Direction::NORTH) return Direction::NORTH;
                    if (dir == Direction::SOUTH) return Direction::SOUTH;
                    break;
                case Block::HORIZONTAL:
                    if (dir == Direction::ANY || dir == Direction::EAST) return Direction::EAST;
                    if (dir == Direction::WEST) return Direction::WEST;
                    break;
                case Block::NORTH_EAST:
                    if (dir == Direction::ANY || dir == Direction::SOUTH) return Direction::EAST;
                    if (dir == Direction::WEST) return Direction::NORTH;
                    break;
                case Block::NORTH_WEST:
                    if (dir == Direction::ANY || dir == Direction::SOUTH) return Direction::WEST;
                    if (dir == Direction::EAST) return Direction::NORTH;
                    break;
                case Block::SOUTH_WEST:
                    if (dir == Direction::ANY || dir == Direction::NORTH) return Direction::WEST;
                    if (dir == Direction::EAST) return Direction::SOUTH;
                    break;
                case Block::SOUTH_EAST:
                    if (dir == Direction::ANY || dir == Direction::NORTH) return Direction::EAST;
                    if (dir == Direction::WEST) return Direction::SOUTH;
                    break;
            }

            assert(0);
            return Direction::ANY;
        }

        void BlockTravel(int& gridIdx, Direction& dir)
        {
            Direction nextDir = GetFollowingDir(Grid[gridIdx], dir);

            dir = nextDir;

            int x, y;
            GetBlockPos(gridIdx, x, y);

            if (nextDir == EAST)
                x++;
            else if (nextDir == WEST)
                x--;
            else if (nextDir == NORTH)
                y--;
            else if (nextDir == SOUTH)
                y++;

            gridIdx = GetBlockIdx(x, y);
        }

        void CloseLoop()
        {
            //Convert start to pipe to complete loop.
            int startX;
            int startY;
            GetBlockPos(StartIdx, startX, startY);

            Block northBlock = GetBlock(startX, startY - 1);
            Block southBlock = GetBlock(startX, startY + 1);
            Block westBlock = GetBlock(startX - 1, startY);
            Block eastBlock = GetBlock(startX + 1, startY);

            bool bCanBeVertical = CanBlockConnectSouth(northBlock) && CanBlockConnectNorth(southBlock);
            bool bCanBeHorizontal = CanBlockConnectEast(westBlock) && CanBlockConnectWest(eastBlock);
            bool bCanBeNorthEast = CanBlockConnectSouth(northBlock) && CanBlockConnectWest(eastBlock);
            bool bCanBeNorthWest = CanBlockConnectSouth(northBlock) && CanBlockConnectEast(westBlock);
            bool bCanBeSouthWest = CanBlockConnectNorth(southBlock) && CanBlockConnectEast(westBlock);
            bool bCanBeSouthEast = CanBlockConnectNorth(southBlock) && CanBlockConnectWest(eastBlock);

            assert((bCanBeVertical + bCanBeHorizontal + bCanBeNorthEast + bCanBeNorthWest + bCanBeSouthWest + bCanBeSouthEast) == 1);

            if (bCanBeVertical)
                Grid[StartIdx] = Block::VERTICAL;
            else if (bCanBeHorizontal)
                Grid[StartIdx] = Block::HORIZONTAL;
            else if (bCanBeNorthEast)
                Grid[StartIdx] = Block::NORTH_EAST;
            else if (bCanBeNorthWest)
                Grid[StartIdx] = Block::NORTH_WEST;
            else if (bCanBeSouthWest)
                Grid[StartIdx] = Block::SOUTH_WEST;
            else if (bCanBeSouthEast)
                Grid[StartIdx] = Block::SOUTH_EAST;

            int nextIdx = StartIdx;
            Direction dir = Direction::ANY;

            Loop.push_back(nextIdx);

            while(1)
            {
                BlockTravel(nextIdx, dir);
                
                if (nextIdx == StartIdx)
                {
                    break;
                }

                Loop.push_back(nextIdx);
            }
        }

        bool IsLoopBlock(int x, int y)
        {
            int idx = GetBlockIdx(x, y);

            for (int loopIdx : Loop)
            {
                if (loopIdx == idx)
                {
                    return true;
                }
            }

            return false;
        }
    };

    void ParseMaze(const char* pDataFileName, Maze& outMaze)
    {
        File file(pDataFileName);

        while (!file.IsAtEnd())
        {
            int width = 0;

            while (!file.IsLineEmpty() && !file.IsAtEnd())
            {
                char blockChar = file.ReadChar();

                switch (blockChar)
                {
                    case '|': outMaze.Grid.push_back(Block::VERTICAL); break;
                    case '-': outMaze.Grid.push_back(Block::HORIZONTAL); break;
                    case 'L': outMaze.Grid.push_back(Block::NORTH_EAST); break;
                    case 'J': outMaze.Grid.push_back(Block::NORTH_WEST); break;
                    case '7': outMaze.Grid.push_back(Block::SOUTH_WEST); break;
                    case 'F': outMaze.Grid.push_back(Block::SOUTH_EAST); break;
                    case '.': outMaze.Grid.push_back(Block::GROUND); break;
                    case 'S':
                        assert(outMaze.StartIdx == -1);
                        outMaze.StartIdx = outMaze.Grid.size();
                        outMaze.Grid.push_back(Block::START); 
                        break;
                    default: assert(0);
                }

                width++;

                file.NextChar();
            }

            assert(outMaze.Width == 0 || outMaze.Width == width);
            outMaze.Width = width;

            file.NextLine();
        }
    }

    void RunPuzzle1(const char* pDataFileName)
    {
        Maze maze;
        ParseMaze(pDataFileName, maze);
        maze.CloseLoop();

        int furthestAway = maze.Loop.size() / 2;

        printf("Furthest step away: %d\n", furthestAway);
    }

    void RunPuzzle2(const char* pDataFileName)
    {
        Maze maze;
        ParseMaze(pDataFileName, maze);
        maze.CloseLoop();

        int loopX1, loopY1, loopX2, loopY2;
        loopX1 = loopY1 = INT_MAX;
        loopX2 = loopY2 = INT_MIN;

        for (int idx : maze.Loop)
        {
            int x, y;
            maze.GetBlockPos(idx, x, y);

            loopX1 = std::min(loopX1, x);
            loopY1 = std::min(loopY1, y);
            loopX2 = std::max(loopX2, x);
            loopY2 = std::max(loopY2, y);
        }

        std::vector<std::pair<int,int>> rowDimensions;

        for (int y = loopY1; y <= loopY2; ++y)
        {
            int start = -1;
            int end = -1;

            for (int x = loopX1; x <= loopX2; ++x)
            {
                if (maze.IsLoopBlock(x, y))
                {
                    if (start == -1)
                    {
                        start = x;
                    }

                    end = x;
                }
            }

            rowDimensions.push_back({start,end});
        }

        int numInside = 0;
        
        for (int y = loopY1; y <= loopY2; ++y)
        {
            bool bInsideLoop = false;

            std::pair<int, int>& dimensions = rowDimensions[y - loopY1];

            Block lastVertBlock = Block::INVALID;

            for (int x = dimensions.first; x <= dimensions.second; ++x)
            {
                bool bLoopBlock = maze.IsLoopBlock(x, y);

                if (bLoopBlock)
                {
                    Block block = maze.GetBlock(x, y);

                    if (block == Block::HORIZONTAL)
                    {
                    }
                    else if (block == Block::VERTICAL)
                    {
                        bInsideLoop = !bInsideLoop;
                        lastVertBlock = Block::VERTICAL;
                    }
                    else if (block == Block::NORTH_EAST)
                    {
                        bInsideLoop = !bInsideLoop;
                        lastVertBlock = Block::NORTH_EAST;
                    }
                    else if (block == Block::SOUTH_EAST)
                    {
                        bInsideLoop = !bInsideLoop;
                        lastVertBlock = Block::SOUTH_EAST;
                    }
                    else if (block == Block::NORTH_WEST)
                    {
                        if (lastVertBlock == Block::NORTH_EAST)
                        {
                            bInsideLoop = !bInsideLoop;
                        }
                    }
                    else if (block == Block::SOUTH_WEST)
                    {
                        if (lastVertBlock == Block::SOUTH_EAST)
                        {
                            bInsideLoop = !bInsideLoop;
                        }
                    }
                }
                else if (bInsideLoop)
                {
                    numInside++;
                }
            }
        }

        printf("Num Inside: %d\n", numInside);
    }
}

void Day10::Run()
{
    //RunPuzzle1("Day10_Sample.txt");
    //RunPuzzle1("Day10_Puzzle.txt");

    //RunPuzzle2("Day10_Sample4.txt");
    RunPuzzle2("Day10_Puzzle.txt");
}
