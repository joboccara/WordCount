#ifndef WordCount_hpp
#define WordCount_hpp

#include <string>
#include <utility>
#include <vector>

enum class HowToDelimitWords
{
    EntireWords,
    WordsInCamelCase
};

class WordData
{
public:
    explicit WordData(std::string word);
    std::string const& word() const;
private:
    std::string word_;
};

template<HowToDelimitWords>
std::vector<WordData> getWordDataFromCode(std::string const& code);

class WordStats
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

bool operator==(WordStats const& wordStats1, WordStats const& wordStats2);
bool operator!=(WordStats const& wordStats1, WordStats const& wordStats2);
bool operator<(WordStats const& wordStats1, WordStats const& wordStats2);
bool operator>(WordStats const& wordStats1, WordStats const& wordStats2);
bool operator<=(WordStats const& wordStats1, WordStats const& wordStats2);
bool operator>=(WordStats const& wordStats1, WordStats const& wordStats2);

using WordCount = std::vector<std::pair<std::string, WordStats>>;
WordCount getWordCount(std::string const& code);

#endif /* WordCount_hpp */
