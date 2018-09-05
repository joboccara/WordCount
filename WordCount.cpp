#include "WordCount.hpp"
#include <boost/algorithm/string.hpp>
#include <cctype>
#include <iostream>
#include <iomanip>
#include <iterator>
#include <map>
#include <vector>

bool isDelimiter(char c)
{
    auto const isAllowedInName = isalnum(c) || c == '_';
    return !isAllowedInName;
}

template<typename Key, typename Value>
void print(std::vector<std::pair<Key, Value>> const& entries)
{
    for (auto const& entry : entries)
    {
        std::cout << std::setw(30) << std::left << entry.first << '|' << std::setw(10) << std::right << entry.second << '\n';
    }
}

std::vector<std::string> getWordsFromCamelCase(std::string const& symbol)
{
    auto words = std::vector<std::string>{};
    auto beginWord = begin(symbol);
    while (beginWord != end(symbol))
    {
        auto endWord = std::find_if(std::next(beginWord), end(symbol), [](char c){ return std::isupper(c); });
        if (beginWord != endWord)
        {
            words.push_back(std::string(beginWord, endWord));
        }
        beginWord = endWord;
    }
    return words;
}


std::vector<std::string> getWordsFromCamelCase(std::vector<std::string> const& symbols)
{
    auto words = std::vector<std::string>{};
    for (auto const& symbol : symbols)
    {
        auto const symbolWords = getWordsFromCamelCase(symbol);
        words.insert(end(words), begin(symbolWords), end(symbolWords));
    }
    return words;
}

std::string toLowerString(std::string const& s)
{
    std::string lower;
    lower.reserve(s.size());
    std::transform(begin(s), end(s), back_inserter(lower), ::tolower);
    return lower;
}

template<typename EndOfWordPredicate>
std::vector<std::string> getWordsFromCode(std::string const& code, EndOfWordPredicate isEndOfWord)
{
    auto words = std::vector<std::string>{};
    auto beginWord = std::find_if_not(begin(code), end(code), isDelimiter);
    while (beginWord != end(code))
    {
        auto endWord = std::find_if(std::next(beginWord), end(code), isEndOfWord);
        words.emplace_back(beginWord, endWord);
        beginWord = std::find_if_not(endWord, end(code), isDelimiter);
    }
    return words;
}

template<typename TypeOfWords>
std::vector<std::string> getWordsFromCode(std::string const& code);

template<>
std::vector<std::string> getWordsFromCode<EntireWords>(std::string const& code)
{
    return getWordsFromCode(code, isDelimiter);
}

template<>
std::vector<std::string> getWordsFromCode<WordsInCamelCase>(std::string const& code)
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

std::map<std::string, size_t> countWords(std::vector<std::string> const& words)
{
    auto wordCount = std::map<std::string, size_t>{};
    for (auto const& word : words)
    {
        ++wordCount[toLowerString(word)];
    }
    return wordCount;
}

std::vector<std::pair<std::string, size_t>> getWordCount(std::string const& code)
{
    auto const words = getWordsFromCode<WordsInCamelCase>(code);
    auto const wordCount = countWords(words);
    
    auto sortedWordCount = std::vector<std::pair<std::string, size_t>>(begin(wordCount), end(wordCount));
    std::sort(begin(sortedWordCount), end(sortedWordCount), [](auto const& p1, auto const& p2){ return p1.second > p2.second; });
    
    return sortedWordCount;
}
