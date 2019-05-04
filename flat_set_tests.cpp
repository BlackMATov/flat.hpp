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

        dummy_allocator() = default;
        dummy_allocator(int i) : i(i) {}

        template < typename U >
        dummy_allocator(const dummy_allocator<U>& o) noexcept {
            i = o.i;
        }

        T* allocate(std::size_t n) noexcept {
            return static_cast<T*>(std::malloc(sizeof(T) * n));
        }

        void deallocate(T* p, std::size_t n) noexcept {
            (void)n;
            std::free(p);
        }

        int i = 0;
    };

    template < typename T, typename U >
    bool operator==(const dummy_allocator<T>&, const dummy_allocator<U>&) noexcept {
        return true;
    }

    template < typename T, typename U >
    bool operator!=(const dummy_allocator<T>& l, const dummy_allocator<U>& r) noexcept {
        return !(l == r);
    }

    template < typename T >
    constexpr std::add_const_t<T>& my_as_const(T& t) noexcept {
        return t;
    }
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
        using set2_t = flat_set<int, std::greater<int>, alloc_t>;
        using vec_t = std::vector<int>;

        {
            auto s0 = set_t();
            auto s1 = set2_t(alloc_t());
            auto s2 = set_t(std::less<int>());
            auto s3 = set2_t(std::greater<int>(), alloc_t());
        }

        {
            vec_t v{1,2,3};
            auto s0 = set_t(v.cbegin(), v.cend());
            auto s1 = set2_t(v.cbegin(), v.cend(), alloc_t());
            auto s2 = set_t(v.cbegin(), v.cend(), std::less<int>());
            auto s3 = set2_t(v.cbegin(), v.cend(), std::greater<int>(), alloc_t());

            REQUIRE(vec_t(s0.begin(), s0.end()) == vec_t({1,2,3}));
            REQUIRE(vec_t(s1.begin(), s1.end()) == vec_t({3,2,1}));
            REQUIRE(vec_t(s2.begin(), s2.end()) == vec_t({1,2,3}));
            REQUIRE(vec_t(s3.begin(), s3.end()) == vec_t({3,2,1}));
        }

        {
            auto s0 = set_t({0,1,2});
            auto s1 = set2_t({0,1,2}, alloc_t());
            auto s2 = set_t({0,1,2}, std::less<int>());
            auto s3 = set2_t({0,1,2}, std::greater<int>(), alloc_t());

            REQUIRE(vec_t(s0.begin(), s0.end()) == vec_t({0,1,2}));
            REQUIRE(vec_t(s1.begin(), s1.end()) == vec_t({2,1,0}));
            REQUIRE(vec_t(s2.begin(), s2.end()) == vec_t({0,1,2}));
            REQUIRE(vec_t(s3.begin(), s3.end()) == vec_t({2,1,0}));
        }

        {
            auto s0 = set_t();
            auto s1 = set_t(alloc_t(42));
            REQUIRE(s0.get_allocator().i == 0);
            REQUIRE(s1.get_allocator().i == 42);
        }

        {
            auto s0 = set_t{0,1,2};
            auto s1 = s0;
            REQUIRE(s0 == set_t{0,1,2});
            REQUIRE(s1 == set_t{0,1,2});
            auto s2 = std::move(s1);
            REQUIRE(s1.empty());
            REQUIRE(s2 == set_t{0,1,2});
        }

        {
            auto s0 = set_t{0,1,2};
            set_t s1;
            s1 = s0;
            REQUIRE(s0 == set_t{0,1,2});
            REQUIRE(s1 == set_t{0,1,2});
            set_t s2;
            s2 = std::move(s1);
            REQUIRE(s0 == set_t{0,1,2});
            REQUIRE(s1.empty());
            REQUIRE(s2 == set_t{0,1,2});
            set_t s3;
            s3 = {1,2,3};
            REQUIRE(s3 == set_t{1,2,3});
        }
    }
    SECTION("capacity") {
        using set_t = flat_set<int>;
        set_t s0;

        REQUIRE(s0.empty());
        REQUIRE_FALSE(s0.size());
        REQUIRE(s0.max_size() == std::allocator<int>().max_size());

        s0.insert(42);

        REQUIRE_FALSE(s0.empty());
        REQUIRE(s0.size() == 1u);
        REQUIRE(s0.max_size() == std::allocator<int>().max_size());

        s0.insert(42);
        REQUIRE(s0.size() == 1u);

        s0.insert(84);
        REQUIRE(s0.size() == 2u);

        s0.clear();

        REQUIRE(s0.empty());
        REQUIRE_FALSE(s0.size());
        REQUIRE(s0.max_size() == std::allocator<int>().max_size());
    }
    SECTION("inserts") {
        struct obj_t {
            obj_t(int i) : i(i) {}
            int i;

            bool operator<(const obj_t& o) const {
                return i < o.i;
            }

            bool operator==(const obj_t& o) const {
                return i == o.i;
            }
        };

        using set_t = flat_set<obj_t>;

        {
            set_t s0;

            auto i0 = s0.insert(1);
            REQUIRE(s0 == set_t{1});
            REQUIRE(i0 == std::make_pair(s0.begin(), true));

            auto i1 = s0.insert(obj_t(1));
            REQUIRE(s0 == set_t{1});
            REQUIRE(i1 == std::make_pair(s0.begin(), false));

            auto i2 = s0.insert(obj_t(2));
            REQUIRE(s0 == set_t{1,2});
            REQUIRE(i2 == std::make_pair(s0.begin() + 1, true));

            auto o2 = obj_t(2);
            auto i3 = s0.insert(o2);
            REQUIRE(i3 == std::make_pair(s0.begin() + 1, false));

            s0.insert(s0.cbegin(), 1);
            s0.insert(s0.cbegin(), 2);
            s0.insert(s0.cend(), 1);
            s0.insert(s0.cend(), 2);
            REQUIRE(s0 == set_t{1,2});

            s0.insert(s0.cbegin(), 0);
            REQUIRE(s0 == set_t{0,1,2});
            s0.insert(s0.cend(), 3);
            REQUIRE(s0 == set_t{0,1,2,3});
            s0.insert(s0.cbegin(), 4);
            s0.insert(s0.cend(), -1);
            REQUIRE(s0 == set_t{-1,0,1,2,3,4});

            s0.insert(s0.cbegin() + 2, obj_t(5));
            REQUIRE(s0 == set_t{-1,0,1,2,3,4,5});
            s0.insert(s0.cbegin(), obj_t(-2));
            REQUIRE(s0 == set_t{-2,-1,0,1,2,3,4,5});
        }

        {
            set_t s0;

            auto e0 = s0.emplace(3);
            REQUIRE(s0 == set_t{3});
            REQUIRE(e0 == std::make_pair(s0.begin(), true));

            auto e1 = s0.emplace(obj_t(3));
            REQUIRE(e1 == std::make_pair(s0.begin(), false));

            auto e2 = s0.emplace(4);
            REQUIRE(s0 == set_t{3,4});
            REQUIRE(e2 == std::make_pair(s0.begin() + 1, true));

            auto e3 = s0.emplace_hint(s0.cbegin(), 1);
            REQUIRE(e3 == s0.begin());
            auto e4 = s0.emplace_hint(s0.cend(), 2);
            REQUIRE(e4 == s0.begin() + 1);
            s0.emplace_hint(s0.cbegin(), 5);
            s0.emplace_hint(s0.cend(), 6);
            REQUIRE(s0 == set_t{1,2,3,4,5,6});
        }
    }
    SECTION("erasers") {
        using set_t = flat_set<int>;
        {
            set_t s0{1,2,3,4,5};
            s0.clear();
            REQUIRE(s0.empty());
        }
        {
            set_t s0{1,2,3,4,5};
            auto i = s0.erase(s0.find(3));
            REQUIRE(i == s0.begin() + 2);
            REQUIRE(s0 == set_t{1,2,4,5});
        }
        {
            set_t s0{1,2,3,4,5};
            auto i = s0.erase(s0.begin() + 2, s0.end());
            REQUIRE(i == s0.end());
            REQUIRE(s0 == set_t{1,2});
        }
        {
            set_t s0{1,2,3,4,5};
            REQUIRE(s0.erase(2) == 1);
            REQUIRE(s0.erase(6) == 0);
            REQUIRE(s0 == set_t{1,3,4,5});
        }
        {
            set_t s0{1,2,3};
            set_t s1{3,4,5};
            s0.swap(s1);
            REQUIRE(s0 == set_t{3,4,5});
            REQUIRE(s1 == set_t{1,2,3});
            swap(s1, s0);
            REQUIRE(s0 == set_t{1,2,3});
            REQUIRE(s1 == set_t{3,4,5});
        }
    }
    SECTION("lookup") {
        using set_t = flat_set<int>;
        {
            set_t s0{1,2,3,4,5};
            REQUIRE(s0.count(3));
            REQUIRE_FALSE(s0.count(6));
            REQUIRE(my_as_const(s0).count(5));
            REQUIRE_FALSE(my_as_const(s0).count(0));
        }
        {
            set_t s0{1,2,3,4,5};
            REQUIRE(s0.find(2) == s0.begin() + 1);
            REQUIRE(my_as_const(s0).find(3) == s0.cbegin() + 2);
            REQUIRE(s0.find(6) == s0.end());
            REQUIRE(my_as_const(s0).find(0) == s0.cend());
        }
        {
            set_t s0{1,2,3,4,5};
            REQUIRE(s0.equal_range(3) == std::make_pair(s0.begin() + 2, s0.begin() + 3));
            REQUIRE(s0.equal_range(6) == std::make_pair(s0.end(), s0.end()));
            REQUIRE(my_as_const(s0).equal_range(3) == std::make_pair(s0.cbegin() + 2, s0.cbegin() + 3));
            REQUIRE(my_as_const(s0).equal_range(0) == std::make_pair(s0.cbegin(), s0.cbegin()));
        }
        {
            set_t s0{0,3,6,9};
            REQUIRE(s0.lower_bound(0) == s0.begin());
            REQUIRE(s0.lower_bound(1) == s0.begin() + 1);
            REQUIRE(s0.lower_bound(10) == s0.end());
            REQUIRE(my_as_const(s0).lower_bound(-1) == s0.cbegin());
            REQUIRE(my_as_const(s0).lower_bound(7) == s0.cbegin() + 3);
        }
    }
    SECTION("observers") {
        struct my_less {
            int i;
            my_less(int i) : i(i) {}
            bool operator()(int l, int r) const {
                return l < r;
            }
        };
        using set_t = flat_set<int, my_less>;
        set_t s0(my_less(42));
        REQUIRE(my_as_const(s0).key_comp().i == 42);
        REQUIRE(my_as_const(s0).value_comp().i == 42);
    }
    SECTION("operators") {
        using set_t = flat_set<int>;

        REQUIRE(set_t{1,2,3} == set_t{3,2,1});
        REQUIRE_FALSE(set_t{1,2,3} == set_t{3,2,4});
        REQUIRE_FALSE(set_t{1,2,3} == set_t{1,2,3,4});

        REQUIRE(set_t{1,2,3} != set_t{3,2,4});
        REQUIRE_FALSE(set_t{1,2,3} != set_t{3,2,1});

        REQUIRE(set_t{2,3,4,6} < set_t{2,3,5});
        REQUIRE(set_t{2,3,4,6} <= set_t{2,3,5});
        REQUIRE_FALSE(set_t{2,3,5} < set_t{2,3,4,6});
        REQUIRE_FALSE(set_t{2,3,5} <= set_t{2,3,4,6});

        REQUIRE_FALSE(set_t{2,3,4,6} > set_t{2,3,5});
        REQUIRE_FALSE(set_t{2,3,4,6} >= set_t{2,3,5});
        REQUIRE(set_t{2,3,5} > set_t{2,3,4,6});
        REQUIRE(set_t{2,3,5} >= set_t{2,3,4,6});

        REQUIRE_FALSE(set_t{1,2,3} < set_t{1,2,3});
        REQUIRE(set_t{1,2,3} <= set_t{1,2,3});
        REQUIRE_FALSE(set_t{1,2,3} > set_t{1,2,3});
        REQUIRE(set_t{1,2,3} >= set_t{1,2,3});
    }
}
