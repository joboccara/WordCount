#include "catch.hpp"
#include "DomainWords.hpp"

TEST_CASE("Clean code ratio")
{
    auto const domainWords = std::vector<std::string>{"Invoice","Paid","Send","Customer"};
    auto const code = R"(public static void main(String args...) {
                            Invoice inv = new Invoice(input)
                            boolean wasPaid = inv.wasPaid
                            if (boolean) {
                                inv.sendToCustomer()
                            }
                        })";

    // tests for empty data
    REQUIRE(cleanCodeRatio("", {}) == 0);
    REQUIRE(cleanCodeRatio(code, {}) == 0);
    REQUIRE(cleanCodeRatio("", domainWords) == 0);

    auto const expectedRatio = 6 / 23.;
    REQUIRE(cleanCodeRatio(code, domainWords) == expectedRatio);
}
