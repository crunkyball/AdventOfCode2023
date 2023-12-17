#include "Day1.h"
#include "File.h"
#include <cstring>
#include <cstdlib>
#include <cstdio>

namespace
{
    void RunPuzzle1(const char* pDataFileName)
    {
        File file(pDataFileName);

        int sumCalibrationValues = 0;

        while (!file.IsAtEnd())
        {
            if (!file.IsLineEmpty())
            {
                const int kBufSize = 8;
                char numBuf[kBufSize];

                memset(numBuf, 0, kBufSize);

                int numBufLen = 0;

                while (!file.IsLineEmpty() && !file.IsAtEnd())
                {
                    char c = file.ReadChar();
                    file.NextChar();

                    if (c >= '0' && c <= '9')
                    {
                        numBuf[numBufLen++] = c;
                    }
                }

                char calibrationVal[3];
                calibrationVal[0] = numBuf[0];
                calibrationVal[1] = numBuf[numBufLen - 1];
                calibrationVal[2] = 0;

                int calibrationValNum = atoi(calibrationVal);

                sumCalibrationValues += calibrationValNum;
            }

            file.NextLine();
        }

        printf("Sum Calibration Value: %d\n", sumCalibrationValues);
    }

    const int kNumbersLen = 10;
    const char* kNumbers[kNumbersLen] = {
        "zero",
        "one",
        "two",
        "three",
        "four",
        "five",
        "six",
        "seven",
        "eight",
        "nine"
    };

    void RunPuzzle2(const char* pDataFileName)
    {
        File file(pDataFileName);

        int sumCalibrationValues = 0;

        while (!file.IsAtEnd())
        {
            if (!file.IsLineEmpty())
            {
                const int kNumBufSize = 32;
                char numBuf[kNumBufSize];
                memset(numBuf, 0, kNumBufSize);
                int numBufLen = 0;

                while (!file.IsLineEmpty() && !file.IsAtEnd())
                {
                    char c = file.ReadChar();

                    if (c >= '0' && c <= '9')
                    {
                        numBuf[numBufLen++] = c;
                    }
                    else
                    {
                        for (int i = 0; i < kNumbersLen; ++i)
                        {
                            if (file.LineStartsWith(kNumbers[i]))
                            {
                                numBuf[numBufLen++] = '0' + i;
                                break;
                            }
                        }
                    }

                    file.NextChar();
                }

                char calibrationVal[3];
                calibrationVal[0] = numBuf[0];
                calibrationVal[1] = numBuf[numBufLen - 1];
                calibrationVal[2] = 0;

                int calibrationValNum = atoi(calibrationVal);

                sumCalibrationValues += calibrationValNum;
            }

            file.NextLine();
        }

        printf("Sum Calibration Value: %d\n", sumCalibrationValues);
    }
}


void Day1::Run()
{
    //RunPuzzle1("Day1_Sample.txt");
    //RunPuzzle1("Day1_Puzzle.txt");

    //RunPuzzle2("Day1_Sample2.txt");
    RunPuzzle2("Day1_Puzzle.txt");
}
