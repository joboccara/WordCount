#ifndef WordCount_hpp
#define WordCount_hpp

#include <string>
#include <vector>

struct EntireWords{};
struct WordsInCamelCase{};

template<typename HowToDelimitWords>
std::vector<std::string> getWordsFromCode(std::string const& code);

std::vector<std::pair<std::string, size_t>> getWordCount(std::string const& code);

#endif /* WordCount_hpp */
