#include "WordCount.hpp"
#include <boost/algorithm/string.hpp>
#include <cctype>
#include <iterator>
#include <map>
#include <vector>

WordData::WordData() : nbOccurrences_(){}

size_t WordData::nbOccurrences() const
{
    return nbOccurrences_;
}

void WordData::addOneOccurrence()
{
    ++nbOccurrences_;
}

bool operator<(WordData const& wordData1, WordData const& wordData2)
{
    return wordData1.nbOccurrences() < wordData2.nbOccurrences();
}

bool operator>=(WordData const& wordData1, WordData const& wordData2) { return !(wordData1 < wordData2); }
bool operator<=(WordData const& wordData1, WordData const& wordData2) { return !(wordData2 < wordData1); }
bool operator>(WordData const& wordData1, WordData const& wordData2) { return !(wordData1 <= wordData2); }
bool operator==(WordData const& wordData1, WordData const& wordData2) { return !(wordData1 < wordData2) && !(wordData2 < wordData1); }
bool operator!=(WordData const& wordData1, WordData const& wordData2) { return !(wordData1 == wordData2); }

WordData::WordData(size_t nbOccurrences) : nbOccurrences_(nbOccurrences){}

bool isDelimiter(char c)
{
    auto const isAllowedInName = isalnum(c) || c == '_';
    return !isAllowedInName;
}

template<typename EndOfWordPredicate>
std::vector<std::string> getWordsFromCode(std::string const& code, EndOfWordPredicate isEndOfWord)
{
    auto words = std::vector<std::string>{};
    auto beginWord = std::find_if_not(begin(code), end(code), isDelimiter);
    while (beginWord != end(code))
    {
        auto const endWord = std::find_if(std::next(beginWord), end(code), isEndOfWord);
        words.emplace_back(beginWord, endWord);
        beginWord = std::find_if_not(endWord, end(code), isDelimiter);
    }
    return words;
}

template<HowToDelimitWords>
std::vector<std::string> getWordsFromCode(std::string const& code);

template<>
std::vector<std::string> getWordsFromCode<HowToDelimitWords::EntireWords>(std::string const& code)
{
    return getWordsFromCode(code, isDelimiter);
}

template<>
std::vector<std::string> getWordsFromCode<HowToDelimitWords::WordsInCamelCase>(std::string const& code)
{
    return getWordsFromCode(code, [](char c){ return isDelimiter(c) || isupper(c); });
}

std::vector<std::string> getSymbols(std::string const& code)
{
    auto symbols = std::vector<std::string>{};
    boost::split(symbols, code, isDelimiter);
    symbols.erase(std::remove(begin(symbols), end(symbols), ""), end(symbols));
    return symbols;
}

std::map<std::string, WordData> countWords(std::vector<std::string> const& words)
{
    auto wordCount = std::map<std::string, WordData>{};
    for (auto const& word : words)
    {
        wordCount[word].addOneOccurrence();
    }
    return wordCount;
}

WordCount getWordCount(std::string const& code)
{
    auto const words = getWordsFromCode<HowToDelimitWords::WordsInCamelCase>(code);
    auto const wordCount = countWords(words);
    
    auto sortedWordCount = WordCount(begin(wordCount), end(wordCount));
    std::sort(begin(sortedWordCount), end(sortedWordCount), [](auto const& p1, auto const& p2){ return p1.second > p2.second; });
    
    return sortedWordCount;
}
