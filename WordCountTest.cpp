#include "catch.hpp"
#include "helpers.hpp"
#include "WordCount.hpp"
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

void print(WordCount const& entries)
{
    if (entries.empty()) return;
    auto const longestWord = *std::max_element(begin(entries), end(entries), [](auto const& p1, auto const& p2){ return p1.first.size() < p2.first.size(); });
    auto const longestWordSize = (int)longestWord.first.size();
    
    for (auto const& entry : entries)
    {
        std::cout << std::setw(longestWordSize + 1) << std::left << entry.first << '|' << std::setw(10) << std::right << entry.second.nbOccurrences() << '\n';
    }
}

TEST_CASE("Word count")
{
    static constexpr auto input = R"(
Integer key = block.getStart();
InsertionMarker insertionMarker = insertionMap.get(key);
if (insertionMarker == null) {
    insertionMarker = new InsertionMarker(block.getStart(), block.getLength());
    insertionMap.put(key, insertionMarker);
    positions.add(block.getStart());
    )";

    auto const expected = WordCount {
        {"insertion", WordStats(6)},
        {"Marker",WordStats(6)},
        {"get",WordStats(5)},
        {"block",WordStats(4)},
        {"key",WordStats(3)},
        {"Start",WordStats(3)},
        {"Insertion",WordStats(2)},
        {"Map",WordStats(2)},
        {"add",WordStats(1)},
        {"if",WordStats(1)},
        {"Length",WordStats(1)},
        {"Integer",WordStats(1)},
        {"new",WordStats(1)},
        {"null",WordStats(1)},
        {"positions",WordStats(1)},
        {"put",WordStats(1)} };
    REQUIRE(getWordCount(input) == expected);
}

std::vector<std::string> words(std::vector<WordData> const& wordData)
{
    auto words = std::vector<std::string>{};
    std::transform(begin(wordData), end(wordData), back_inserter(words), callMethod(&::WordData::word));
    return words;
}
    
TEST_CASE("Extract words from code")
{
    REQUIRE(words(getWordDataFromCode<HowToDelimitWords::EntireWords>("helloWorld")) == std::vector<std::string>{ "helloWorld" });
    REQUIRE(words(getWordDataFromCode<HowToDelimitWords::EntireWords>("helloWorld howAreYou")) == std::vector<std::string>{ "helloWorld", "howAreYou" });
    REQUIRE(words(getWordDataFromCode<HowToDelimitWords::EntireWords>("  helloWorld howAreYou")) == std::vector<std::string>{ "helloWorld", "howAreYou" });
    REQUIRE(words(getWordDataFromCode<HowToDelimitWords::EntireWords>("   helloWorld howAreYou  ")) == std::vector<std::string>{ "helloWorld", "howAreYou" });
    REQUIRE(words(getWordDataFromCode<HowToDelimitWords::EntireWords>("helloWorld->howAreYou")) == std::vector<std::string>{ "helloWorld", "howAreYou" });
    REQUIRE(words(getWordDataFromCode<HowToDelimitWords::EntireWords>("   ")) == std::vector<std::string>{});
    REQUIRE(words(getWordDataFromCode<HowToDelimitWords::EntireWords>("")) == std::vector<std::string>{});
}
    
TEST_CASE("Extract camel case words from code")
{
    REQUIRE(words(getWordDataFromCode<HowToDelimitWords::WordsInCamelCase>("helloWorld")) == std::vector<std::string>{ "hello", "World" });
    REQUIRE(words(getWordDataFromCode<HowToDelimitWords::WordsInCamelCase>("helloWorldX")) == std::vector<std::string>{ "hello", "World", "X" });
    REQUIRE(words(getWordDataFromCode<HowToDelimitWords::WordsInCamelCase>("helloWorld howAreYou")) == std::vector<std::string>{ "hello", "World", "how", "Are", "You" });
    REQUIRE(words(getWordDataFromCode<HowToDelimitWords::WordsInCamelCase>("  helloWorld howAreYou")) == std::vector<std::string>{ "hello", "World", "how", "Are", "You" });
    REQUIRE(words(getWordDataFromCode<HowToDelimitWords::WordsInCamelCase>("   helloWorld howAreYou  ")) == std::vector<std::string>{ "hello", "World", "how", "Are", "You" });
    REQUIRE(words(getWordDataFromCode<HowToDelimitWords::WordsInCamelCase>("helloWorld->howAreYou")) == std::vector<std::string>{ "hello", "World", "how", "Are", "You" });
    REQUIRE(words(getWordDataFromCode<HowToDelimitWords::WordsInCamelCase>("   ")) == std::vector<std::string>{});
    REQUIRE(words(getWordDataFromCode<HowToDelimitWords::WordsInCamelCase>("")) == std::vector<std::string>{});
}

