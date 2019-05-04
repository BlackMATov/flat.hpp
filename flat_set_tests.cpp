/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/flat.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#define CATCH_CONFIG_FAST_COMPILE
#include "catch.hpp"

#include "flat_set.hpp"
namespace flat = flat_hpp;

namespace
{
}

TEST_CASE("flat_set") {
    {
        using set_t = flat::flat_set<int>;

        static_assert(
            std::is_same<set_t::key_type, int>::value,
            "unit test static error");
        static_assert(
            std::is_same<set_t::value_type, int>::value,
            "unit test static error");

        static_assert(
            std::is_same<set_t::size_type, std::size_t>::value,
            "unit test static error");
        static_assert(
            std::is_same<set_t::difference_type, std::ptrdiff_t>::value,
            "unit test static error");

        static_assert(
            std::is_same<set_t::reference, int&>::value,
            "unit test static error");
        static_assert(
            std::is_same<set_t::const_reference, const int&>::value,
            "unit test static error");

        static_assert(
            std::is_same<set_t::pointer, int*>::value,
            "unit test static error");
        static_assert(
            std::is_same<set_t::const_pointer, const int*>::value,
            "unit test static error");
    }
}
