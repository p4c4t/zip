#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "zip.hpp"

#include <forward_list>
#include <sstream>

TEST_CASE("Zip2_read") {
    const int a[]{1, 2, 3, 4};
    const std::forward_list<std::string> b = {"one", "two", "three"};
    std::stringstream stream;

    for (const auto &[first, second]: Zip(a, b)) {
        stream << first;
        stream << ":" << second;
        stream << " ";
    }

    REQUIRE("1:one 2:two 3:three " == stream.str());
}

TEST_CASE("Zip3_read") {
    const int a[]{1, 2, 3, 4};
    const std::forward_list<std::string> b = {"one", "two", "three"};
    const int c[]{-1, -2, -3, -4};
    std::stringstream stream2;

    for (const auto &[first, second, third]: Zip(a, b, c)) {
        stream2 << first;
        stream2 << ":" << second;
        stream2 << ":" << third;
        stream2 << " ";
    }

    REQUIRE("1:one:-1 2:two:-2 3:three:-3 " == stream2.str());
}

TEST_CASE("Zip1_read") {
    std::vector<int> a = {1, 2, 3, 4, 5};
    std::stringstream stream;

    for (const auto &[first]: Zip(a)) {
        stream << first;
        stream << " ";
    }

    REQUIRE("1 2 3 4 5 " == stream.str());
}

TEST_CASE("Zip1_write") {
    std::vector<int> a = {1, 2, 3, 4, 5};
    std::array<char, 3> c = {'a', 'b', 'c'};

    for (auto &&[num, ch]: Zip(a, c)) {
        num++;
        ch = ch + 1;
    }

    REQUIRE(a == std::vector<int>{2, 3, 4, 4, 5});
    REQUIRE(c == std::array<char, 3>{'b', 'c', 'd'});
}

TEST_CASE("Zip1_partial_write") {
    std::vector<int> a = {1, 2, 3, 4, 5};
    const std::array<char, 3> c = {'a', 'b', 'c'};

    for (auto &&[num, ch]: Zip(a, c)) {
        num++;
    }

    REQUIRE(a == std::vector<int>{2, 3, 4, 4, 5});
    REQUIRE(c == std::array<char, 3>{'a', 'b', 'c'});
}

TEST_CASE("AnyEqual") {
    REQUIRE(AnyEqual(
            std::make_tuple(1), std::make_tuple(1),
            std::make_index_sequence<1>()
    ));
    REQUIRE(AnyEqual(
            std::make_tuple(1, 2), std::make_tuple(1, 0),
            std::make_index_sequence<2>()
    ));
    REQUIRE(AnyEqual(
            std::make_tuple(2, 2), std::make_tuple(1, 2),
            std::make_index_sequence<2>()
    ));
    REQUIRE(!AnyEqual(
            std::make_tuple(2, 2), std::make_tuple(1, 1),
            std::make_index_sequence<2>()
    ));
}