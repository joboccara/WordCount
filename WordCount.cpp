#include "WordCount.hpp"
#include <algorithm>
#include <cctype>
#include <iterator>
#include <map>
#include <vector>

WordStats::WordStats() : nbOccurrences_(){}

size_t WordStats::nbOccurrences() const
{
    return nbOccurrences_;
}

void WordStats::addOneOccurrence(size_t lineNumber)
{
    ++nbOccurrences_;
    
    if (nbOccurrences_ == 1)
    {
        lowestOccurringLine_ = highestOccurringLine_ = lineNumber;
    }
    else
    {
        lowestOccurringLine_ = std::min(lowestOccurringLine_, lineNumber);
        highestOccurringLine_ = std::max(highestOccurringLine_, lineNumber);
    }
}

size_t WordStats::span() const
{
    if (nbOccurrences_ == 0)
    {
        return 0;
    }
    else
    {
        return highestOccurringLine_ - lowestOccurringLine_ + 1;
    }
}

double WordStats::proportion() const
{
    if (totalNumberOfLines() == 0) return 0;
    
    return span() / static_cast<double>(totalNumberOfLines());
}

size_t WordStats::totalNumberOfLines() const
{
    return totalNumberOfLines_;
}

void WordStats::setTotalNumberOfLines(size_t totalNumberOfLines)
{
    totalNumberOfLines_ = totalNumberOfLines;
}

bool operator<(WordStats const& wordStats1, WordStats const& wordStats2)
{
    return wordStats1.nbOccurrences() < wordStats2.nbOccurrences();
}

WordData::WordData(std::string word, size_t lineNumber) : word_(std::move(word)), lineNumber_(lineNumber){}

bool operator<(WordData const& wordData1, WordData const& wordData2)
{
    if (wordData1.word() < wordData2.word()) return true;
    if (wordData2.word() < wordData1.word()) return false;
    return wordData1.lineNumber() < wordData2.lineNumber();
}

std::string const& WordData::word() const
{
    return word_;
}

bool isDelimiter(char c)
{
    auto const isAllowedInName = isalnum(c) || c == '_';
    return !isAllowedInName;
}

template<typename EndOfWordPredicate>
std::vector<WordData> getWordDataFromCode(std::string const& code, EndOfWordPredicate isEndOfWord)
{
    auto words = std::vector<WordData>{};
    auto endWord = begin(code);
    auto beginWord = std::find_if_not(begin(code), end(code), isDelimiter);
    size_t line = 0;

    while (beginWord != end(code))
    {
        auto const linesBetweenWords = std::count(endWord, beginWord, '\n');
        line += linesBetweenWords;
        endWord = std::find_if(std::next(beginWord), end(code), isEndOfWord);
        words.emplace_back(std::string(beginWord, endWord), line);
        beginWord = std::find_if_not(endWord, end(code), isDelimiter);
    }
    return words;
}

template<HowToDelimitWords>
std::vector<WordData> getWordDataFromCode(std::string const& code);

template<>
std::vector<WordData> getWordDataFromCode<HowToDelimitWords::EntireWords>(std::string const& code)
{
    return getWordDataFromCode(code, isDelimiter);
}

template<>
std::vector<WordData> getWordDataFromCode<HowToDelimitWords::WordsInCamelCase>(std::string const& code)
{
    return getWordDataFromCode(code, [](char c){ return isDelimiter(c) || isupper(c); });
}

std::map<std::string, WordStats> wordStats(std::vector<WordData> const& wordData, size_t numberOfLines)
{
    auto wordStats = std::map<std::string, WordStats>{};
    for (auto const& oneWordData : wordData)
    {
        auto& oneWordStats = wordStats[oneWordData.word()];
        oneWordStats.setTotalNumberOfLines(numberOfLines);
        oneWordStats.addOneOccurrence(oneWordData.lineNumber());
    }
    return wordStats;
}

size_t numberOfLines(std::string const& code)
{
    return std::count(begin(code), end(code), '\n') + 1;
}

WordCount getWordCount(std::string const& code)
{
    auto const words = getWordDataFromCode<HowToDelimitWords::WordsInCamelCase>(code);
    auto const wordCount = wordStats(words, numberOfLines(code));
    
    auto sortedWordCount = WordCount(begin(wordCount), end(wordCount));
    std::sort(begin(sortedWordCount), end(sortedWordCount), [](auto const& p1, auto const& p2){ return p1.second > p2.second; });
    
    return sortedWordCount;
}
