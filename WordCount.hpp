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
    WordData(std::string word, size_t position);
    std::string const& word() const;
    size_t position() const { return position_; }
private:
    std::string word_;
    size_t position_;
};

bool operator<(WordData const& wordStats1, WordData const& wordStats2);

template<HowToDelimitWords>
std::vector<WordData> getWordDataFromCode(std::string const& code);

class WordStats : public Comparable<WordStats>
{
public:
    WordStats();
    size_t nbOccurrences() const;
    void addOneOccurrence();
private:
    size_t nbOccurrences_;

public: // reserved for testing
    explicit WordStats(size_t nbOccurrences);
};

bool operator<(WordStats const& wordStats1, WordStats const& wordStats2);

using WordCount = std::vector<std::pair<std::string, WordStats>>;
WordCount getWordCount(std::string const& code);

#endif /* WordCount_hpp */
