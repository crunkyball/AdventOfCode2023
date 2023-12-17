#include "Day8.h"
#include "File.h"
#include "Utils.h"
#include <cassert>
#include <algorithm>

namespace
{
    struct Network
    {
        enum class Dir
        {
            LEFT, RIGHT
        };

        struct Node
        {
            char NodeName[4];

            Node* LeftNode = nullptr;
            Node* RightNode = nullptr;

            bool IsStartNode = false;
            bool IsEndNode = false;

            Network::Node* Travel(Dir dir)
            {
                if (dir == Dir::LEFT)
                    return LeftNode;
                return RightNode;
            }
        };

        std::vector<Dir> Instructions;
        std::vector<Node> Nodes;

        Node* StartNode = nullptr;
        Node* EndNode = nullptr;

        std::vector<Node*> StartNodes;
        std::vector<Node*> EndNodes;

        int CurrentInstruction = 0;

        Network::Node* GetNodeFromName(const char* nodeName)
        {
            for (Node& node : Nodes)
            {
                if (strcmp(nodeName, node.NodeName) == 0)
                {
                    return &node;
                }
            }

            return nullptr;
        }
    };

    void ParseNetwork(const char* pDataFilename, Network& outNetwork)
    {
        File file(pDataFilename);

        while (!file.IsLineEmpty())
        {
            char c = file.ReadChar();

            switch (c)
            {
                case 'L': outNetwork.Instructions.push_back(Network::Dir::LEFT); break;
                case 'R': outNetwork.Instructions.push_back(Network::Dir::RIGHT); break;
                default: assert(0);
            }

            file.NextChar();
        }

        for (int i = 0; i < 2; ++i)
        {
            while (!file.IsAtEnd())
            {
                if (!file.IsLineEmpty())
                {
                    char nodeName[4];
                    char leftNodeName[4];
                    char rightNodeName[4];

                    file.ReadLineChars(nodeName, 3);

                    if (i == 0)
                    {
                        //First time through create the nodes.
                        Network::Node node;
                        strcpy(node.NodeName, nodeName);

                        outNetwork.Nodes.push_back(node);
                    }
                    else
                    {
                        //Second time through link them up.
                        file.NextChar(7);
                        file.ReadLineChars(leftNodeName, 3);
                        file.NextChar(5);
                        file.ReadLineChars(rightNodeName, 3);

                        Network::Node* pNode = outNetwork.GetNodeFromName(nodeName);
                        Network::Node* pLeftNode = outNetwork.GetNodeFromName(leftNodeName);
                        Network::Node* pRightNode = outNetwork.GetNodeFromName(rightNodeName);

                        assert(pNode != nullptr && pLeftNode != nullptr && pRightNode != nullptr);

                        pNode->LeftNode = pLeftNode;
                        pNode->RightNode = pRightNode;

                        if (outNetwork.StartNode == nullptr && strcmp(nodeName, "AAA") == 0)
                        {
                            outNetwork.StartNode = pNode;
                            pNode->IsStartNode = true;
                        }
                        else if (outNetwork.EndNode == nullptr && strcmp(nodeName, "ZZZ") == 0)
                        {
                            outNetwork.EndNode = pNode;
                            pNode->IsEndNode = true;
                        }
                        
                        if (nodeName[2] == 'A')
                        {
                            outNetwork.StartNodes.push_back(pNode);
                            pNode->IsStartNode = true;
                        }
                        else if (nodeName[2] == 'Z')
                        {
                            outNetwork.EndNodes.push_back(pNode);
                            pNode->IsEndNode = true;
                        }
                    }
                }

                file.NextLine();
            }

            file.SetLine(2);
        }
    }

    void RunPuzzle1(const char* pDataFileName)
    {
        Network network;
        ParseNetwork(pDataFileName, network);

        int numSteps = 0;
        
        for (Network::Node* pNode = network.StartNode; pNode != network.EndNode; pNode = pNode->Travel(network.Instructions[network.CurrentInstruction++]))
        {
            numSteps++;

            if (network.CurrentInstruction == network.Instructions.size())
            {
                network.CurrentInstruction = 0;
            }
        }

        printf("Num Steps: %d\n", numSteps);
    }

    void RunPuzzle2(const char* pDataFileName)
    {
        Network network;
        ParseNetwork(pDataFileName, network);

        uint64_t numSteps = 0;

        std::vector<Network::Node*> nodes = network.StartNodes;

        //There's absolutely a better way of doing this but I'm brute forcing it as Baldur's Gate 3 has been released.
        while (1)
        {
            numSteps++;

            assert(numSteps != 0);

            Network::Dir instruction = network.Instructions[network.CurrentInstruction++];

            int numEnd = 0;

            for (Network::Node*& pNode : nodes)
            {
                pNode = pNode->Travel(instruction);

                if (pNode->IsEndNode)
                {
                    numEnd++;
                }
            }

            if (network.CurrentInstruction == network.Instructions.size())
            {
                network.CurrentInstruction = 0;
            }

            if (numEnd == nodes.size())
            {
                break;
            }
        }

        printf("Num Steps: %llu\n", numSteps);
    }
}

void Day8::Run()
{
    //RunPuzzle1("Day8_Sample2.txt");
    //RunPuzzle1("Day8_Puzzle.txt");

    //RunPuzzle2("Day8_Sample3.txt");
    RunPuzzle2("Day8_Puzzle.txt");
}
