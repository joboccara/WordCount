#ifndef WordCount_hpp
#define WordCount_hpp

#include <string>
#include <vector>

std::vector<std::string> getWordsFromCamelCase(std::string const& symbol);
std::vector<std::string> getCamelCaseWordsFromCode(std::string const& code);
std::vector<std::pair<std::string, size_t>> getWordCount(std::string const& code);

#endif /* WordCount_hpp */
