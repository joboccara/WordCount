#include "catch.hpp"
#include "WordCount.hpp"
#include <iostream>
#include <iomanip>

void print(WordCount const& entries)
{
    if (entries.empty()) return;
    auto const longestWord = *std::max_element(begin(entries), end(entries), [](auto const& p1, auto const& p2){ return p1.first.size() < p2.first.size(); });
    auto const longestWordSize = (int)longestWord.first.size();
    
    for (auto const& entry : entries)
    {
        std::cout << std::setw(longestWordSize + 1) << std::left << entry.first << '|' << std::setw(10) << std::right << entry.second << '\n';
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

    auto const expected = WordCount { {"insertion",6},{"Marker",6},{"get",5},{"block",4},
        {"key",3},{"Start",3},{"Insertion",2},{"Map",2},{"add",1},{"if",1},{"Length",1},
        {"Integer",1},{"new",1},{"null",1},{"positions",1},{"put",1} };
    REQUIRE(getWordCount(input) == expected);
}

    
TEST_CASE("Extract words from code")
{
    REQUIRE(getWordsFromCode<HowToDelimitWords::EntireWords>("helloWorld") == std::vector<std::string>{ "helloWorld" });
    REQUIRE(getWordsFromCode<HowToDelimitWords::EntireWords>("helloWorld howAreYou") == std::vector<std::string>{ "helloWorld", "howAreYou" });
    REQUIRE(getWordsFromCode<HowToDelimitWords::EntireWords>("  helloWorld howAreYou") == std::vector<std::string>{ "helloWorld", "howAreYou" });
    REQUIRE(getWordsFromCode<HowToDelimitWords::EntireWords>("   helloWorld howAreYou  ") == std::vector<std::string>{ "helloWorld", "howAreYou" });
    REQUIRE(getWordsFromCode<HowToDelimitWords::EntireWords>("helloWorld->howAreYou") == std::vector<std::string>{ "helloWorld", "howAreYou" });
    REQUIRE(getWordsFromCode<HowToDelimitWords::EntireWords>("   ") == std::vector<std::string>{});
    REQUIRE(getWordsFromCode<HowToDelimitWords::EntireWords>("") == std::vector<std::string>{});
}

    
TEST_CASE("Extract camel case words from code")
{
    REQUIRE(getWordsFromCode<HowToDelimitWords::WordsInCamelCase>("helloWorld") == std::vector<std::string>{ "hello", "World" });
    REQUIRE(getWordsFromCode<HowToDelimitWords::WordsInCamelCase>("helloWorldX") == std::vector<std::string>{ "hello", "World", "X" });
    REQUIRE(getWordsFromCode<HowToDelimitWords::WordsInCamelCase>("helloWorld howAreYou") == std::vector<std::string>{ "hello", "World", "how", "Are", "You" });
    REQUIRE(getWordsFromCode<HowToDelimitWords::WordsInCamelCase>("  helloWorld howAreYou") == std::vector<std::string>{ "hello", "World", "how", "Are", "You" });
    REQUIRE(getWordsFromCode<HowToDelimitWords::WordsInCamelCase>("   helloWorld howAreYou  ") == std::vector<std::string>{ "hello", "World", "how", "Are", "You" });
    REQUIRE(getWordsFromCode<HowToDelimitWords::WordsInCamelCase>("helloWorld->howAreYou") == std::vector<std::string>{ "hello", "World", "how", "Are", "You" });
    REQUIRE(getWordsFromCode<HowToDelimitWords::WordsInCamelCase>("   ") == std::vector<std::string>{});
    REQUIRE(getWordsFromCode<HowToDelimitWords::WordsInCamelCase>("") == std::vector<std::string>{});
}

