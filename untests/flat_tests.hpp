/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/flat.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019-2023, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <doctest/doctest.h>

#define STATIC_REQUIRE(...)\
    static_assert(__VA_ARGS__, #__VA_ARGS__);\
    REQUIRE(__VA_ARGS__)

#define STATIC_REQUIRE_FALSE(...)\
    static_assert(!(__VA_ARGS__), "!(" #__VA_ARGS__ ")");\
    REQUIRE(!(__VA_ARGS__))
