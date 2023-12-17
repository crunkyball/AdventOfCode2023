#include "Day7.h"
#include "File.h"
#include "Utils.h"
#include <cassert>
#include <algorithm>

namespace
{
    enum class Card
    {
        TWO,
        THREE,
        FOUR,
        FIVE,
        SIX,
        SEVEN,
        EIGHT,
        NINE,
        TEN,
        JACK_JOKER,
        QUEEN,
        KING,
        ACE,

        MAX
    };

    enum class HandType
    {
        HIGH_CARD,
        ONE_PAIR,
        TWO_PAIR,
        THREE_OF_A_KIND,
        FULL_HOUSE,
        FOUR_OF_A_KIND,
        FIVE_OF_A_KIND
    };

    struct Hand
    {
        static const int kNumCards = 5;
        Card Cards[kNumCards] { Card::ACE, Card::ACE, Card::ACE, Card::ACE, Card::ACE };
        int NumOfEachCard[static_cast<int>(Card::MAX)] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

        HandType Type;

        int Bid = 0;

        bool Beats(const Hand& otherHand, bool jokerLow) const
        {
            if (Type == otherHand.Type)
            {
                for (int i = 0; i < kNumCards; ++i)
                {
                    if (Cards[i] != otherHand.Cards[i])
                    {
                        if (jokerLow && otherHand.Cards[i] == Card::JACK_JOKER)
                        {
                            return true;
                        }
                        else if (jokerLow && Cards[i] == Card::JACK_JOKER)
                        {
                            return false;
                        }

                        return Cards[i] > otherHand.Cards[i];
                    }
                }
            }

            return Type > otherHand.Type;
        }
    };

    using HandList = std::vector<Hand>;

    Card CharToCard(char c)
    {
        switch (c)
        {
            case '2': return Card::TWO;
            case '3': return Card::THREE;
            case '4': return Card::FOUR;
            case '5': return Card::FIVE;
            case '6': return Card::SIX;
            case '7': return Card::SEVEN;
            case '8': return Card::EIGHT;
            case '9': return Card::NINE;
            case 'T': return Card::TEN;
            case 'J': return Card::JACK_JOKER;
            case 'Q': return Card::QUEEN;
            case 'K': return Card::KING;
            case 'A': return Card::ACE;
        }

        assert(0);
        return Card::MAX;
    }

    void ParseHands(const char* pDataFileName, HandList& outHands)
    {
        File file(pDataFileName);

        while (!file.IsAtEnd())
        {
            if (!file.IsLineEmpty())
            {
                const int kLineBufSize = 64;
                char lineBuf[kLineBufSize];

                file.ReadLine(lineBuf, kLineBufSize);

                const int kCardBufSize = 6;
                char cardBuf[kCardBufSize];

                const int kBidBufSize = 5;
                char bidBuf[kBidBufSize];

                Utils::SplitString(lineBuf, " ", cardBuf, kCardBufSize, bidBuf, kBidBufSize);

                Hand hand;

                for (int i = 0; i < 5; ++i)
                {
                    hand.Cards[i] = CharToCard(cardBuf[i]);
                    hand.NumOfEachCard[static_cast<int>(hand.Cards[i])]++;
                }

                hand.Bid = atoi(bidBuf);

                outHands.push_back(hand);
            }

            file.NextLine();
        }
    }

    bool HasFiveOfAKind(const Hand& hand, bool replaceJokers)
    {
        int wildcards = replaceJokers ? hand.NumOfEachCard[static_cast<int>(Card::JACK_JOKER)] : 0;

        for (int i = 0; i < static_cast<int>(Card::MAX); ++i)
        {
            if (replaceJokers && i == static_cast<int>(Card::JACK_JOKER))
            {
                continue;
            }

            if (hand.NumOfEachCard[i] + wildcards == 5)
            {
                return true;
            }
        }

        return false;
    }

    bool HasFourOfAKind(const Hand& hand, bool replaceJokers)
    {
        int wildcards = replaceJokers ? hand.NumOfEachCard[static_cast<int>(Card::JACK_JOKER)] : 0;

        for (int i = 0; i < static_cast<int>(Card::MAX); ++i)
        {
            if (replaceJokers && i == static_cast<int>(Card::JACK_JOKER))
            {
                continue;
            }

            if (hand.NumOfEachCard[i] + wildcards == 4)
            {
                return true;
            }
        }

        return false;
    }

    bool HasFullHouse(const Hand& hand, bool replaceJokers)
    {
        int wildcards = replaceJokers ? hand.NumOfEachCard[static_cast<int>(Card::JACK_JOKER)] : 0;

        int val = 5;

        for (int i = 0; i < static_cast<int>(Card::MAX); ++i)
        {
            if (replaceJokers && i == static_cast<int>(Card::JACK_JOKER))
            {
                continue;
            }

            if (hand.NumOfEachCard[i] == 2 || hand.NumOfEachCard[i] == 3)
            {
                val -= (hand.NumOfEachCard[i]);
            }
        }

        val -= wildcards;

        return val == 0;
    }

    bool HasThreeOfAKind(const Hand& hand, bool replaceJokers)
    {
        int wildcards = replaceJokers ? hand.NumOfEachCard[static_cast<int>(Card::JACK_JOKER)] : 0;

        for (int i = 0; i < static_cast<int>(Card::MAX); ++i)
        {
            if (replaceJokers && i == static_cast<int>(Card::JACK_JOKER))
            {
                continue;
            }

            if (hand.NumOfEachCard[i] + wildcards == 3)
            {
                return true;
            }
        }

        return false;
    }

    bool HasTwoPair(const Hand& hand, bool replaceJokers)
    {
        int wildcards = replaceJokers ? hand.NumOfEachCard[static_cast<int>(Card::JACK_JOKER)] : 0;

        int val = 4;

        for (int i = 0; i < static_cast<int>(Card::MAX); ++i)
        {
            if (replaceJokers && i == static_cast<int>(Card::JACK_JOKER))
            {
                continue;
            }

            if (hand.NumOfEachCard[i] == 2)
            {
                val -= hand.NumOfEachCard[i];
            }
        }

        val -= wildcards;

        return val == 0;
    }

    bool HasOnePair(const Hand& hand, bool replaceJokers)
    {
        int wildcards = replaceJokers ? hand.NumOfEachCard[static_cast<int>(Card::JACK_JOKER)] : 0;

        for (int i = 0; i < static_cast<int>(Card::MAX); ++i)
        {
            if (replaceJokers && i == static_cast<int>(Card::JACK_JOKER))
            {
                continue;
            }

            if (hand.NumOfEachCard[i] + wildcards == 2)
            {
                return true;
            }
        }

        return false;
    }

    HandType GetHandType(const Hand& hand, bool replaceJokers)
    {
        if (HasFiveOfAKind(hand, replaceJokers))
        {
            return HandType::FIVE_OF_A_KIND;
        }

        if (HasFourOfAKind(hand, replaceJokers))
        {
            return HandType::FOUR_OF_A_KIND;
        }

        if (HasFullHouse(hand, replaceJokers))
        {
            return HandType::FULL_HOUSE;
        }

        if (HasThreeOfAKind(hand, replaceJokers))
        {
            return HandType::THREE_OF_A_KIND;
        }

        if (HasTwoPair(hand, replaceJokers))
        {
            return HandType::TWO_PAIR;
        }

        if (HasOnePair(hand, replaceJokers))
        {
            return HandType::ONE_PAIR;
        }

        return HandType::HIGH_CARD;
    }

    void FindHandTypes(HandList& handList, bool replaceJokers)
    {
        for (Hand& hand : handList)
        {
            hand.Type = GetHandType(hand, replaceJokers);
        }
    }

    void RunPuzzle(const char* pDataFileName, bool replaceJokers)
    {
        HandList hands;
        ParseHands(pDataFileName, hands);

        FindHandTypes(hands, replaceJokers);

        std::vector<int> handRanks;
        for (int i = 0; i < hands.size(); ++i)
        {
            handRanks.push_back(i);
        }

        std::sort(handRanks.begin(), handRanks.end(), [hands, replaceJokers](int a, int b)
        {
            return hands[b].Beats(hands[a], replaceJokers);
        });

        int winnings = 0;

        for (int i = 0; i < handRanks.size(); ++i)
        {
            int win = hands[handRanks[i]].Bid * (i + 1);
            winnings += win;
        }

        printf("Winnings: %d\n", winnings);
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

void Day7::Run()
{
    //RunPuzzle1("Day7_Sample.txt");
    //RunPuzzle1("Day7_Puzzle.txt");

    //RunPuzzle2("Day7_Sample.txt");
    RunPuzzle2("Day7_Puzzle.txt");
}
