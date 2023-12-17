#include "Day4.h"
#include "File.h"
#include "Utils.h"
#include <cassert>

namespace
{
    struct Card
    {
        std::vector<int> WinningNumbers;
        std::vector<int> CardNumbers;

        uint64_t Amount = 1; //How many of this card do we have?
    };

    void ParseCard(const char* pDataLine, Card& outCard)
    {
        const int kCardNameBufSize = 16;
        char cardNameBuf[kCardNameBufSize];

        const int kCardDataBufSize = 256;
        char cardDataBuf[kCardDataBufSize];

        Utils::SplitString(pDataLine, ": ", cardNameBuf, kCardNameBufSize, cardDataBuf, kCardDataBufSize);

        const int kWinningNumbersBufSize = 128;
        char winningNumbersBuf[kWinningNumbersBufSize];

        const int kCardNumbersBufSize = 128;
        char cardNumbersBuf[kCardNumbersBufSize];

        Utils::SplitString(cardDataBuf, " | ", winningNumbersBuf, kWinningNumbersBufSize, cardNumbersBuf, kCardNumbersBufSize);

        const int kNumberBufSize = 8;
        const int kMaxWinningNumbers = 32;
        const int kMaxCardNumbers = 32;
        char winningNumbers[kMaxWinningNumbers][kNumberBufSize];
        char cardNumbers[kMaxCardNumbers][kNumberBufSize];

        int numWinningNumbers = Utils::Tokenise(winningNumbersBuf, " ", &winningNumbers[0][0], kMaxWinningNumbers, kNumberBufSize);
        int numCardNumbers = Utils::Tokenise(cardNumbersBuf, " ", &cardNumbers[0][0], kMaxCardNumbers, kNumberBufSize);

        for (int i = 0; i < numWinningNumbers; ++i)
        {
            if (strlen(winningNumbers[i]) > 0)
            {
                outCard.WinningNumbers.push_back(atoi(winningNumbers[i]));
            }            
        }

        for (int i = 0; i < numCardNumbers; ++i)
        {
            if (strlen(cardNumbers[i]) > 0)
            {
                outCard.CardNumbers.push_back(atoi(cardNumbers[i]));
            }
        }
    }

    void ParseCards(const char* pDataFileName, std::vector<Card>& outCards)
    {
        File file(pDataFileName);

        while (!file.IsAtEnd())
        {
            if (!file.IsLineEmpty())
            {
                const int kLineBufSize = 256;
                char lineBuf[kLineBufSize];

                file.ReadLine(lineBuf, kLineBufSize);

                Card card;
                ParseCard(lineBuf, card);

                outCards.push_back(card);
            }

            file.NextLine();
        }
    }

    int FindWinningNumbers(const Card& card)
    {
        int winningNumbers = 0;

        for (int winningNum : card.WinningNumbers)
        {
            for (int cardNum : card.CardNumbers)
            {
                if (cardNum == winningNum)
                {
                    winningNumbers++;
                    break;
                }
            }
        }

        return winningNumbers;
    }

    int FindCardPoints(const Card& card)
    {
        int numPoints = (1 << (FindWinningNumbers(card) - 1));

        return numPoints;
    }

    void RunPuzzle1(const char* pDataFileName)
    {
        std::vector<Card> cards;
        ParseCards(pDataFileName, cards);

        int sumPoints = 0;

        for (const Card& card : cards)
        {
            sumPoints += FindCardPoints(card);
        }

        printf("Sum Points: %d\n", sumPoints);
    }

    void RunPuzzle2(const char* pDataFileName)
    {
        std::vector<Card> cards;
        ParseCards(pDataFileName, cards);

        int numCards = cards.size();

        for (int cardIdx = 0; cardIdx < cards.size(); ++cardIdx)
        {
            int winningNumbers = FindWinningNumbers(cards[cardIdx]);

            for (int i = cardIdx + 1; i < std::min(cardIdx + 1 + winningNumbers, numCards); ++i)
            {
                cards[i].Amount += cards[cardIdx].Amount;
            }
        }

        uint64_t totalCards = 0;

        for (const Card& card : cards)
        {
            totalCards += card.Amount;
        }

        printf("Total Cards: %llu\n", totalCards);
    }
}


void Day4::Run()
{
    //RunPuzzle1("Day4_Sample.txt");
    //RunPuzzle1("Day4_Puzzle.txt");

    //RunPuzzle2("Day4_Sample.txt");
    RunPuzzle2("Day4_Puzzle.txt");
}
