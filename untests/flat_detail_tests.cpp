/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/flat.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019-2023, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <flat.hpp/flat.hpp>
#include "flat_tests.hpp"

namespace
{
    using namespace flat_hpp;
}

TEST_CASE("flat_map_detail") {
    SUBCASE("is_sorted") {
        auto i1 = {1,2,3,4};
        REQUIRE(detail::is_sorted(i1.begin(), i1.end(), std::less<int>()));
        REQUIRE_FALSE(detail::is_sorted(i1.begin(), i1.end(), std::greater<int>()));

        auto i2 = {4,3,2,1};
        REQUIRE(detail::is_sorted(i2.begin(), i2.end(), std::greater<int>()));
        REQUIRE_FALSE(detail::is_sorted(i2.begin(), i2.end(), std::less<int>()));

        auto i3 = {2,1,3,4};
        REQUIRE_FALSE(detail::is_sorted(i3.begin(), i3.end(), std::less<int>()));
        REQUIRE_FALSE(detail::is_sorted(i3.begin(), i3.end(), std::greater<int>()));
    }
    SUBCASE("is_sorted_unique") {
        auto i1 = {1,2,3,4};
        auto i1d = {1,2,2,3,4};
        REQUIRE(detail::is_sorted_unique(i1.begin(), i1.end(), std::less<int>()));
        REQUIRE_FALSE(detail::is_sorted_unique(i1d.begin(), i1d.end(), std::less<int>()));
        REQUIRE_FALSE(detail::is_sorted_unique(i1.begin(), i1.end(), std::greater<int>()));
        REQUIRE_FALSE(detail::is_sorted_unique(i1d.begin(), i1d.end(), std::greater<int>()));

        auto i2 = {4,3,2,1};
        auto i2d = {4,3,2,1,1};
        REQUIRE(detail::is_sorted_unique(i2.begin(), i2.end(), std::greater<int>()));
        REQUIRE_FALSE(detail::is_sorted_unique(i2d.begin(), i2d.end(), std::greater<int>()));
        REQUIRE_FALSE(detail::is_sorted_unique(i2.begin(), i2.end(), std::less<int>()));
        REQUIRE_FALSE(detail::is_sorted_unique(i2d.begin(), i2d.end(), std::less<int>()));
    }
}
