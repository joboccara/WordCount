#include "catch.hpp"
#include "WordCount.hpp"

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
    
    auto const expected = std::vector<std::pair<std::string, size_t>> { {"insertion",8},{"marker",6},{"get",5},{"block",4},
        {"start",3},{"key",3},{"map",2},{"add",1},{"integer",1},
        {"if",1},{"new",1},{"null",1},{"positions",1},{"put",1},{"length",1} };
    REQUIRE(getWordCount(input) == expected);
}

    
TEST_CASE("Extract words from code")
{
    REQUIRE(getWordsFromCode<EntireWords>("helloWorld") == std::vector<std::string>{ "helloWorld" });
    REQUIRE(getWordsFromCode<EntireWords>("helloWorld howAreYou") == std::vector<std::string>{ "helloWorld", "howAreYou" });
    REQUIRE(getWordsFromCode<EntireWords>("  helloWorld howAreYou") == std::vector<std::string>{ "helloWorld", "howAreYou" });
    REQUIRE(getWordsFromCode<EntireWords>("   helloWorld howAreYou  ") == std::vector<std::string>{ "helloWorld", "howAreYou" });
    REQUIRE(getWordsFromCode<EntireWords>("helloWorld->howAreYou") == std::vector<std::string>{ "helloWorld", "howAreYou" });
    REQUIRE(getWordsFromCode<EntireWords>("   ") == std::vector<std::string>{});
    REQUIRE(getWordsFromCode<EntireWords>("") == std::vector<std::string>{});
}

    
TEST_CASE("Extract camel case words from code")
{
    REQUIRE(getWordsFromCode<WordsInCamelCase>("helloWorld") == std::vector<std::string>{ "hello", "World" });
    REQUIRE(getWordsFromCode<WordsInCamelCase>("helloWorldX") == std::vector<std::string>{ "hello", "World", "X" });
    REQUIRE(getWordsFromCode<WordsInCamelCase>("helloWorld howAreYou") == std::vector<std::string>{ "hello", "World", "how", "Are", "You" });
    REQUIRE(getWordsFromCode<WordsInCamelCase>("  helloWorld howAreYou") == std::vector<std::string>{ "hello", "World", "how", "Are", "You" });
    REQUIRE(getWordsFromCode<WordsInCamelCase>("   helloWorld howAreYou  ") == std::vector<std::string>{ "hello", "World", "how", "Are", "You" });
    REQUIRE(getWordsFromCode<WordsInCamelCase>("helloWorld->howAreYou") == std::vector<std::string>{ "hello", "World", "how", "Are", "You" });
    REQUIRE(getWordsFromCode<WordsInCamelCase>("   ") == std::vector<std::string>{});
    REQUIRE(getWordsFromCode<WordsInCamelCase>("") == std::vector<std::string>{});
}

