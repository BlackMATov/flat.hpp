/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/flat.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#define CATCH_CONFIG_FAST_COMPILE
#include "catch.hpp"

#include "flat_map.hpp"
namespace flat = flat_hpp;

namespace
{
}

TEST_CASE("flat_map") {
    {
        using map_t = flat::flat_map<int, unsigned>;

        static_assert(
            std::is_same<map_t::key_type, int>::value,
            "unit test static error");
        static_assert(
            std::is_same<map_t::mapped_type, unsigned>::value,
            "unit test static error");
        static_assert(
            std::is_same<map_t::value_type, std::pair<const int, unsigned>>::value,
            "unit test static error");

        static_assert(
            std::is_same<map_t::size_type, std::size_t>::value,
            "unit test static error");
        static_assert(
            std::is_same<map_t::difference_type, std::ptrdiff_t>::value,
            "unit test static error");

        static_assert(
            std::is_same<map_t::reference, std::pair<const int, unsigned>&>::value,
            "unit test static error");
        static_assert(
            std::is_same<map_t::const_reference, const std::pair<const int, unsigned>&>::value,
            "unit test static error");

        static_assert(
            std::is_same<map_t::pointer, std::pair<const int, unsigned>*>::value,
            "unit test static error");
        static_assert(
            std::is_same<map_t::const_pointer, const std::pair<const int, unsigned>*>::value,
            "unit test static error");
    }
}
