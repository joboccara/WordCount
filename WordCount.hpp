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

class WordData
{
public:
    WordData();
    size_t nbOccurrences() const;
    void addOneOccurrence();
private:
    size_t nbOccurrences_;

public: // reserved for testing
    explicit WordData(size_t nbOccurrences);
};

bool operator==(WordData const& wordData1, WordData const& wordData2);
bool operator!=(WordData const& wordData1, WordData const& wordData2);
bool operator<(WordData const& wordData1, WordData const& wordData2);
bool operator>(WordData const& wordData1, WordData const& wordData2);
bool operator<=(WordData const& wordData1, WordData const& wordData2);
bool operator>=(WordData const& wordData1, WordData const& wordData2);

using WordCount = std::vector<std::pair<std::string, WordData>>;
WordCount getWordCount(std::string const& code);

#endif /* WordCount_hpp */
