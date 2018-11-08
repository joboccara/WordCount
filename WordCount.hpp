#ifndef WordCount_hpp
#define WordCount_hpp

#include "helpers.hpp"

#include <string>
#include <utility>
#include <vector>

enum class HowToDelimitWords
{
    EntireWords,
    WordsInCamelCase
};

class WordData : public Comparable<WordData>
{
public:
    WordData(std::string word, size_t lineNumber);
    std::string const& word() const;
    size_t lineNumber() const { return lineNumber_; }
private:
    std::string word_;
    size_t lineNumber_;
};

bool operator<(WordData const& wordStats1, WordData const& wordStats2);

template<HowToDelimitWords>
std::vector<WordData> getWordDataFromCode(std::string const& code);

class WordStats : public Comparable<WordStats>
{
public:
    WordStats();
    void setTotalNumberOfLines(size_t totalNumberOfLines);
    size_t nbOccurrences() const;
    void addOneOccurrence(size_t lineNumber);
    size_t span() const;
    double proportion() const;
private:
    size_t totalNumberOfLines_ = 0;
    size_t totalNumberOfLines() const;
    size_t nbOccurrences_ = 0;
    size_t lowestOccurringLine_ = 0;
    size_t highestOccurringLine_ = 0;
};

bool operator<(WordStats const& wordStats1, WordStats const& wordStats2);

using WordCount = std::vector<std::pair<std::string, WordStats>>;
WordCount getWordCount(std::string const& code);

#endif /* WordCount_hpp */
