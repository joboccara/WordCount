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

using WordCount = std::vector<std::pair<std::string, size_t>>;
WordCount getWordCount(std::string const& code);

#endif /* WordCount_hpp */
