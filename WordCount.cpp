#include "WordCount.hpp"
#include <boost/algorithm/string.hpp>
#include <cctype>
#include <iterator>
#include <map>
#include <vector>

WordStats::WordStats() : nbOccurrences_(){}

size_t WordStats::nbOccurrences() const
{
    return nbOccurrences_;
}

void WordStats::addOneOccurrence()
{
    ++nbOccurrences_;
}

bool operator<(WordStats const& wordStats1, WordStats const& wordStats2)
{
    return wordStats1.nbOccurrences() < wordStats2.nbOccurrences();
}

WordStats::WordStats(size_t nbOccurrences) : nbOccurrences_(nbOccurrences){}

WordData::WordData(std::string word, size_t position) : word_(std::move(word)), position_(position){}

bool operator<(WordData const& wordData1, WordData const& wordData2)
{
    if (wordData1.word() < wordData2.word()) return true;
    if (wordData2.word() < wordData1.word()) return false;
    return wordData1.position() < wordData2.position();
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
    auto beginWord = std::find_if_not(begin(code), end(code), isDelimiter);
    while (beginWord != end(code))
    {
        auto const endWord = std::find_if(std::next(beginWord), end(code), isEndOfWord);
        auto const wordPosition = std::distance(begin(code), beginWord);
        words.emplace_back(std::string(beginWord, endWord), wordPosition);
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

std::vector<std::string> getSymbols(std::string const& code)
{
    auto symbols = std::vector<std::string>{};
    boost::split(symbols, code, isDelimiter);
    symbols.erase(std::remove(begin(symbols), end(symbols), ""), end(symbols));
    return symbols;
}

std::map<std::string, WordStats> countWords(std::vector<WordData> const& wordData)
{
    auto wordCount = std::map<std::string, WordStats>{};
    for (auto const& oneWordData : wordData)
    {
        wordCount[oneWordData.word()].addOneOccurrence();
    }
    return wordCount;
}

WordCount getWordCount(std::string const& code)
{
    auto const words = getWordDataFromCode<HowToDelimitWords::WordsInCamelCase>(code);
    auto const wordCount = countWords(words);
    
    auto sortedWordCount = WordCount(begin(wordCount), end(wordCount));
    std::sort(begin(sortedWordCount), end(sortedWordCount), [](auto const& p1, auto const& p2){ return p1.second > p2.second; });
    
    return sortedWordCount;
}
