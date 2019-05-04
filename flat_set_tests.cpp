/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/flat.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#define CATCH_CONFIG_FAST_COMPILE
#include "catch.hpp"

#include "flat_set.hpp"
using namespace flat_hpp;

namespace
{
    template < typename T >
    class dummy_allocator {
    public:
        using value_type = T;

        T* allocate(std::size_t n) {
            (void)n;
            return nullptr;
        }

        void deallocate(T* p, std::size_t n) {
            (void)p;
            (void)n;
        }
    };
}

TEST_CASE("flat_set") {
    SECTION("types") {
        using set_t = flat_set<int>;

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
    SECTION("ctors") {
        using alloc_t = dummy_allocator<int>;
        using set_t = flat_set<int, std::less<int>, alloc_t>;

        {
            auto s0 = set_t();
            auto s1 = set_t(alloc_t());
            auto s2 = set_t(std::less<int>());
            auto s3 = set_t(std::less<int>(), alloc_t());
        }

        {
            std::vector<int> v;
            auto s0 = set_t(v.cbegin(), v.cend());
            auto s1 = set_t(v.cbegin(), v.cend(), alloc_t());
            auto s2 = set_t(v.cbegin(), v.cend(), std::less<int>());
            auto s3 = set_t(v.cbegin(), v.cend(), std::less<int>(), alloc_t());
        }

        {
            auto s0 = set_t({0,1,2});
            auto s1 = set_t({0,1,2}, alloc_t());
            auto s2 = set_t({0,1,2}, std::less<int>());
            auto s3 = set_t({0,1,2}, std::less<int>(), alloc_t());
        }
    }
    SECTION("capacity") {
        using set_t = flat_set<int>;
        set_t s0;
        s0.empty();
        s0.size();
        s0.max_size();
    }
    SECTION("inserts") {
        struct obj_t {
            obj_t(int i) : i(i) {}
            int i;
        };

        using set_t = flat_set<obj_t>;

        {
            set_t s0;
            s0.insert(42);
            s0.insert(obj_t(42));
            s0.insert(s0.cend(), 84);
            s0.insert(s0.cend(), obj_t(84));
            s0.emplace(100500);
            s0.emplace_hint(s0.cend(), 100500);
        }
    }
    SECTION("erasers") {
        using set_t = flat_set<int>;
        set_t s0;
        s0.clear();
        s0.erase(s0.begin());
        s0.erase(s0.cbegin());
        s0.erase(s0.begin(), s0.end());
        s0.erase(s0.cbegin(), s0.cend());
        s0.erase(42);
        set_t s1;
        s0.swap(s1);
        swap(s0, s1);
    }
}
