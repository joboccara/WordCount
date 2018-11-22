#include "DomainWords.hpp"
#include "WordCount.hpp"

#include <algorithm>
#include <numeric>

static size_t numberOfWords(WordCount const& wordCount)
{
    return std::accumulate(begin(wordCount), end(wordCount), 0,
                           [](size_t countSoFar, std::pair<std::string, WordStats> const& wordStats)
                           {
                               return countSoFar + wordStats.second.nbOccurrences();
                           });
}

struct CompareWord
{
    bool operator()(std::pair<std::string, WordStats> const& wordStats, std::string const& word)
    {
        return wordStats.first < word;
    }
    bool operator()(std::string const& word, std::pair<std::string, WordStats> const& wordStats)
    {
        return (*this)(wordStats, word);
    }
};

double cleanCodeRatio(std::string const& code, std::vector<std::string> const& inputDomainWords)
{
    auto wordCount = getWordCount(code);
    if (wordCount.empty()) return 0;
    
    std::sort(begin(wordCount), end(wordCount), [](auto const& p1, auto const& p2){ return p1.first < p2.first; });
    auto domainWords = inputDomainWords;
    std::sort(begin(domainWords), end(domainWords));
    
    auto domainWordCount = WordCount{};
    std::set_intersection(begin(wordCount), end(wordCount),
                          begin(domainWords), end(domainWords),
                          back_inserter(domainWordCount),
                          CompareWord{}
                          );
    
    return 1. * numberOfWords(domainWordCount) / numberOfWords(wordCount);
}
