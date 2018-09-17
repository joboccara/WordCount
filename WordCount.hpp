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

template<HowToDelimitWords>
std::vector<std::string> getWordsFromCode(std::string const& code);

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

bool operator==(WordStats const& wordData1, WordStats const& wordData2);
bool operator!=(WordStats const& wordData1, WordStats const& wordData2);
bool operator<(WordStats const& wordData1, WordStats const& wordData2);
bool operator>(WordStats const& wordData1, WordStats const& wordData2);
bool operator<=(WordStats const& wordData1, WordStats const& wordData2);
bool operator>=(WordStats const& wordData1, WordStats const& wordData2);

using WordCount = std::vector<std::pair<std::string, WordStats>>;
WordCount getWordCount(std::string const& code);

#endif /* WordCount_hpp */
