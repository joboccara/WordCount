#include "catch.hpp"
#include "WordCount.hpp"

TEST_CASE("Extract words from camelCase")
{
    REQUIRE(getWordsFromCamelCase("helloWorld") == std::vector<std::string>{ "hello", "World" });
    REQUIRE(getWordsFromCamelCase("helloWorldX") == std::vector<std::string>{ "hello", "World", "X" });
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
    
    auto const expected = std::vector<std::pair<std::string, size_t>> { {"insertion",8},{"marker",6},{"get",5},{"block",4},
        {"start",3},{"key",3},{"map",2},{"add",1},{"integer",1},
        {"if",1},{"new",1},{"null",1},{"positions",1},{"put",1},{"length",1} };
    REQUIRE(getWordCount(input) == expected);
}

TEST_CASE("Extract camelCaseWords from code")
{
    REQUIRE(getCamelCaseWordsFromCode("helloWorld") == std::vector<std::string>{ "hello", "World" });
    REQUIRE(getCamelCaseWordsFromCode("helloWorldX") == std::vector<std::string>{ "hello", "World", "X" });
    REQUIRE(getCamelCaseWordsFromCode("helloWorld howAreYou") == std::vector<std::string>{ "hello", "World", "how", "Are", "You" });
    REQUIRE(getCamelCaseWordsFromCode("  helloWorld howAreYou") == std::vector<std::string>{ "hello", "World", "how", "Are", "You" });
    REQUIRE(getCamelCaseWordsFromCode("   helloWorld howAreYou  ") == std::vector<std::string>{ "hello", "World", "how", "Are", "You" });
    REQUIRE(getCamelCaseWordsFromCode("helloWorld->howAreYou") == std::vector<std::string>{ "hello", "World", "how", "Are", "You" });
    REQUIRE(getCamelCaseWordsFromCode("   ") == std::vector<std::string>{});
    REQUIRE(getCamelCaseWordsFromCode("") == std::vector<std::string>{});
}

