#pragma once

class File
{
public:
    File(const char* fileName);
    virtual ~File();

    int ReadLineAsNumber() const;
    char ReadChar() const;
    void ReadLine(char buffer[], int bufferSize) const;
    void ReadLineChars(char buffer[], int numChars, bool addNTC = true) const;

    void Rewind();

    void NextLine(int stepSize = 1);
    void NextChar(int stepSize = 1, bool lineOnly = true);

    void PreviousLine(int stepSize = 1);

    void SetLine(int lineNum);

    bool IsLineEmpty() const;
    bool IsAtEnd() const;
    bool LineContains(char c) const;
    bool LineStartsWith(const char* pStr) const;
    int LineFind(const char* pStr) const;

private:
    char* m_Data = nullptr;
    int m_Size = 0;

    const char* m_pFileEnd;
    const char* m_pFilePos;
};
