#include "catch.hpp"
#include "helpers.hpp"
#include "WordCount.hpp"
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

void print(WordCount const& entries)
{
    if (entries.empty()) return;
    auto const longestWord = *std::max_element(begin(entries), end(entries), [](auto const& p1, auto const& p2){ return p1.first.size() < p2.first.size(); });
    auto const longestWordSize = (int)longestWord.first.size();
    
    std::cout
    << std::setw(longestWordSize + 1) << std::left << "Word"
    << '|'
    << std::setw(4) << std::right << "#"
    << '|'
    << std::setw(4) << std::right << "span"
    << '|'
    << std::setw(11) << std::right << "proportion"
    << '\n';
    std::cout << std::string(longestWordSize + 1 + 1 + 4 + 1 + 4, '-') << '\n';
    
    for (auto const& entry : entries)
    {
        std::cout
        << std::setw(longestWordSize + 1) << std::left << entry.first
        << '|'
        << std::setw(4) << std::right << entry.second.nbOccurrences()
        << '|'
        << std::setw(4) << std::right << entry.second.span()
        << '|'
        << std::setw(10) << std::right << std::round(entry.second.proportion() * 100 * 100) / 100. << '%'
        << '\n';
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

    auto const getResults = project([](auto const& entry){ return std::make_tuple(entry.first, entry.second.nbOccurrences(), entry.second.span(), entry.second.proportion()); });

    // print(getWordCount(input));

    auto const expected = std::vector<std::tuple<std::string, size_t, size_t, double>> {
        {"insertion", 6, 4, 0.5},
        {"Marker", 6, 4, 0.5},
        {"get", 5, 6, 0.75},
        {"block", 4, 6, 0.75},
        {"key", 3, 5, 0.625},
        {"Start", 3, 6, 0.75},
        {"Insertion", 2, 3, 0.375},
        {"Map", 2, 4, 0.5},
        {"add", 1, 1, 0.125},
        {"if", 1, 1, 0.125},
        {"Length", 1, 1, 0.125},
        {"Integer", 1, 1, 0.125},
        {"new", 1, 1, 0.125},
        {"null", 1, 1, 0.125},
        {"positions", 1, 1, 0.125},
        {"put", 1, 1, 0.125} };
    REQUIRE(getResults(getWordCount(input)) == expected);
    
}

auto const words = projectOnMember(&WordData::word);
    
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
    
auto lineNumbers = projectOnMember(&WordData::lineNumber);

TEST_CASE("Extract positions from code")
{
    REQUIRE(lineNumbers(getWordDataFromCode<HowToDelimitWords::EntireWords>("helloWorld")) == std::vector<size_t>{ 0 });
    REQUIRE(lineNumbers(getWordDataFromCode<HowToDelimitWords::EntireWords>("helloWorld howAreYou")) == std::vector<size_t>{ 0, 0 });
    REQUIRE(lineNumbers(getWordDataFromCode<HowToDelimitWords::EntireWords>("  helloWorld\n howAreYou")) == std::vector<size_t>{ 0, 1 });
    REQUIRE(lineNumbers(getWordDataFromCode<HowToDelimitWords::EntireWords>(" \n  helloWorld \n howAreYou  \n")) == std::vector<size_t>{ 1, 2 });
    REQUIRE(lineNumbers(getWordDataFromCode<HowToDelimitWords::EntireWords>("helloWorld\n\nhowAreYou")) == std::vector<size_t>{ 0, 2 });

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

TEST_CASE("Extract camel case psitions from code")
{
    REQUIRE(lineNumbers(getWordDataFromCode<HowToDelimitWords::WordsInCamelCase>("helloWorld")) == std::vector<size_t>{ 0,0 });
    REQUIRE(lineNumbers(getWordDataFromCode<HowToDelimitWords::WordsInCamelCase>("helloWorld howAreYou")) == std::vector<size_t>{ 0,0,0,0,0 });
    REQUIRE(lineNumbers(getWordDataFromCode<HowToDelimitWords::WordsInCamelCase>("  helloWorld\n howAreYou")) == std::vector<size_t>{ 0,0, 1,1,1 });
    REQUIRE(lineNumbers(getWordDataFromCode<HowToDelimitWords::WordsInCamelCase>(" \n  helloWorld \n howAreYou  \n")) == std::vector<size_t>{ 1,1, 2,2,2 });
    REQUIRE(lineNumbers(getWordDataFromCode<HowToDelimitWords::WordsInCamelCase>("helloWorld\n\nhowAreYou")) == std::vector<size_t>{ 0,0, 2,2,2 });
}

