/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/flat.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#define CATCH_CONFIG_FAST_COMPILE
#include "catch.hpp"

#include <deque>

#include "flat_map.hpp"
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

TEST_CASE("flat_map") {
    SECTION("types") {
        using map_t = flat_map<int, unsigned>;

        static_assert(
            std::is_same<map_t::key_type, int>::value,
            "unit test static error");
        static_assert(
            std::is_same<map_t::mapped_type, unsigned>::value,
            "unit test static error");
        static_assert(
            std::is_same<map_t::value_type, std::pair<int, unsigned>>::value,
            "unit test static error");

        static_assert(
            std::is_same<map_t::size_type, std::size_t>::value,
            "unit test static error");
        static_assert(
            std::is_same<map_t::difference_type, std::ptrdiff_t>::value,
            "unit test static error");

        static_assert(
            std::is_same<map_t::reference, std::pair<int, unsigned>&>::value,
            "unit test static error");
        static_assert(
            std::is_same<map_t::const_reference, const std::pair<int, unsigned>&>::value,
            "unit test static error");

        static_assert(
            std::is_same<map_t::pointer, std::pair<int, unsigned>*>::value,
            "unit test static error");
        static_assert(
            std::is_same<map_t::const_pointer, const std::pair<int, unsigned>*>::value,
            "unit test static error");
    }
    SECTION("ctors") {
        using alloc_t = dummy_allocator<
            std::pair<int,unsigned>>;

        using map_t = flat_map<
            int,
            unsigned,
            std::less<int>,
            alloc_t>;

        using map2_t = flat_map<
            int,
            unsigned,
            std::greater<int>,
            alloc_t>;

        using vec_t = std::vector<
            std::pair<int,unsigned>>;

        {
            auto s0 = map_t();
            auto s1 = map2_t(alloc_t());
            auto s2 = map_t(std::less<int>());
            auto s3 = map2_t(std::greater<int>(), alloc_t());
        }

        {
            vec_t v{{1,30},{2,20},{3,10}};
            auto s0 = map_t(v.cbegin(), v.cend());
            auto s1 = map2_t(v.cbegin(), v.cend(), alloc_t());
            auto s2 = map_t(v.cbegin(), v.cend(), std::less<int>());
            auto s3 = map2_t(v.cbegin(), v.cend(), std::greater<int>(), alloc_t());

            REQUIRE(vec_t(s0.begin(), s0.end()) == vec_t({{1,30},{2,20},{3,10}}));
            REQUIRE(vec_t(s1.begin(), s1.end()) == vec_t({{3,10},{2,20},{1,30}}));
            REQUIRE(vec_t(s2.begin(), s2.end()) == vec_t({{1,30},{2,20},{3,10}}));
            REQUIRE(vec_t(s3.begin(), s3.end()) == vec_t({{3,10},{2,20},{1,30}}));
        }

        {
            auto s0 = map_t({{0,1}, {1,2}});
            auto s1 = map_t({{0,1}, {1,2}}, alloc_t());
            auto s2 = map_t({{0,1}, {1,2}}, std::less<int>());
            auto s3 = map_t({{0,1}, {1,2}}, std::less<int>(), alloc_t());

            REQUIRE(vec_t(s0.begin(), s0.end()) == vec_t({{0,1},{1,2}}));
            REQUIRE(vec_t(s1.begin(), s1.end()) == vec_t({{0,1},{1,2}}));
            REQUIRE(vec_t(s2.begin(), s2.end()) == vec_t({{0,1},{1,2}}));
            REQUIRE(vec_t(s3.begin(), s3.end()) == vec_t({{0,1},{1,2}}));
        }

        {
            auto s0 = map_t();
            auto s1 = map_t(alloc_t(42));
            REQUIRE(s0.get_allocator().i == 0);
            REQUIRE(s1.get_allocator().i == 42);
        }

        {
            auto s0 = map_t{{0,1}, {1,2}};
            auto s1 = s0;
            REQUIRE(s0 == map_t{{0,1}, {1,2}});
            REQUIRE(s1 == map_t{{0,1}, {1,2}});
            auto s2 = std::move(s1);
            REQUIRE(s1.empty());
            REQUIRE(s2 == map_t{{0,1}, {1,2}});
        }

        {
            auto s0 = map_t{{0,1}, {1,2}};
            map_t s1;
            s1 = s0;
            REQUIRE(s0 == map_t{{0,1}, {1,2}});
            REQUIRE(s1 == map_t{{0,1}, {1,2}});
            map_t s2;
            s2 = std::move(s1);
            REQUIRE(s0 == map_t{{0,1}, {1,2}});
            REQUIRE(s1.empty());
            REQUIRE(s2 == map_t{{0,1}, {1,2}});
            map_t s3;
            s3 = {{0,1}, {1,2}};
            REQUIRE(s3 == map_t{{0,1}, {1,2}});
        }
    }
    SECTION("capacity") {
        using map_t = flat_map<int, unsigned>;

        {
            map_t s0;

            REQUIRE(s0.empty());
            REQUIRE_FALSE(s0.size());
            REQUIRE(s0.max_size() == std::allocator<std::pair<int,unsigned>>().max_size());

            s0.insert({2,42});

            REQUIRE_FALSE(s0.empty());
            REQUIRE(s0.size() == 1u);
            REQUIRE(s0.max_size() == std::allocator<std::pair<int,unsigned>>().max_size());

            s0.insert({2,84});
            REQUIRE(s0.size() == 1u);

            s0.insert({3,84});
            REQUIRE(s0.size() == 2u);

            s0.clear();

            REQUIRE(s0.empty());
            REQUIRE_FALSE(s0.size());
            REQUIRE(s0.max_size() == std::allocator<std::pair<int,unsigned>>().max_size());
        }

        {
            map_t s0;

            REQUIRE(s0.capacity() == 0);
            s0.reserve(42);
            REQUIRE(s0.capacity() == 42);
            s0.insert({{1,2},{2,3},{3,4}});
            REQUIRE(s0.capacity() == 42);
            s0.shrink_to_fit();
            REQUIRE(s0.size() == 3);
            REQUIRE(s0.capacity() == 3);
            REQUIRE(s0 == map_t{{1,2},{2,3},{3,4}});

            using alloc2_t = dummy_allocator<
                std::pair<int, unsigned>>;

            using map2_t = flat_map<
                int,
                unsigned,
                std::less<int>,
                alloc2_t,
                std::deque<std::pair<int, unsigned>, alloc2_t>>;

            map2_t s1;
            s1.insert({{1,2},{2,3},{3,4}});
            REQUIRE(s1 == map2_t{{1,2},{2,3},{3,4}});
        }
    }
    SECTION("access") {
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

        using map_t = flat_map<obj_t, unsigned>;
        map_t s0;

        obj_t k1(1);

        s0[k1] = 42;
        REQUIRE(s0[k1] == 42);
        REQUIRE(s0 == map_t{{1,42}});

        s0[1] = 84;
        REQUIRE(s0[1] == 84);
        REQUIRE(s0 == map_t{{1,84}});

        s0[2] = 21;
        REQUIRE(s0[2] == 21);
        REQUIRE(s0 == map_t{{1,84},{2,21}});

        REQUIRE(s0.at(1) == 84);
        REQUIRE(my_as_const(s0).at(k1) == 84);
        REQUIRE_THROWS_AS(s0.at(0), std::out_of_range);
        REQUIRE_THROWS_AS(my_as_const(s0).at(0), std::out_of_range);
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

        using map_t = flat_map<obj_t, obj_t>;

        {
            map_t s0;

            auto k1_42 = std::make_pair(1, 42);
            auto k3_84 = std::make_pair(3, 84);

            auto i0 = s0.insert(k1_42);
            REQUIRE(s0 == map_t{{1,42}});
            REQUIRE(i0 == std::make_pair(s0.begin(), true));

            auto i1 = s0.insert(std::make_pair(1, obj_t(42)));
            REQUIRE(s0 == map_t{{1,42}});
            REQUIRE(i1 == std::make_pair(s0.begin(), false));

            auto i2 = s0.insert(std::make_pair(2, obj_t(42)));
            REQUIRE(s0 == map_t{{1,42},{2,42}});
            REQUIRE(i2 == std::make_pair(s0.begin() + 1, true));

            auto i3 = s0.insert(s0.cend(), k3_84);
            REQUIRE(i3 == s0.begin() + 2);

            s0.insert(s0.cend(), std::make_pair(4, obj_t(84)));
            auto i4 = s0.insert(s0.cend(), std::make_pair(0, obj_t(21)));
            REQUIRE(i4 == s0.begin());

            auto i5 = s0.emplace(5, 100500);
            REQUIRE(i5 == std::make_pair(s0.end() - 1, true));
            REQUIRE(s0 == map_t{{0,21},{1,42},{2,42},{3,84},{4,84},{5,100500}});

            auto i6 = s0.emplace_hint(s0.cend(), 6, 100500);
            REQUIRE(i6 == s0.end() - 1);
            REQUIRE(s0 == map_t{{0,21},{1,42},{2,42},{3,84},{4,84},{5,100500},{6,100500}});
        }
    }
    SECTION("erasers") {
        using map_t = flat_map<int, unsigned>;
        {
            map_t s0{{1,2},{2,3},{3,4}};
            s0.clear();
            REQUIRE(s0.empty());
        }
        {
            map_t s0{{1,2},{2,3},{3,4}};
            auto i = s0.erase(s0.find(2));
            REQUIRE(i == s0.begin() + 1);
            REQUIRE(s0 == map_t{{1,2},{3,4}});
        }
        {
            map_t s0{{1,2},{2,3},{3,4}};
            auto i = s0.erase(s0.begin() + 1, s0.end());
            REQUIRE(i == s0.end());
            REQUIRE(s0 == map_t{{1,2}});
        }
        {
            map_t s0{{1,2},{2,3},{3,4}};
            REQUIRE(s0.erase(1) == 1);
            REQUIRE(s0.erase(6) == 0);
            REQUIRE(s0 == map_t{{2,3},{3,4}});
        }
        {
            map_t s0{{1,2},{2,3},{3,4}};
            map_t s1{{2,3},{3,4},{5,6}};
            s0.swap(s1);
            REQUIRE(s0 == map_t{{2,3},{3,4},{5,6}});
            REQUIRE(s1 == map_t{{1,2},{2,3},{3,4}});
            swap(s1, s0);
            REQUIRE(s0 == map_t{{1,2},{2,3},{3,4}});
            REQUIRE(s1 == map_t{{2,3},{3,4},{5,6}});
        }
    }
    SECTION("lookup") {
        using map_t = flat_map<int, unsigned>;
        {
            map_t s0{{1,2},{2,3},{3,4},{4,5},{5,6}};
            REQUIRE(s0.count(3));
            REQUIRE_FALSE(s0.count(6));
            REQUIRE(my_as_const(s0).count(5));
            REQUIRE_FALSE(my_as_const(s0).count(0));
        }
        {
            map_t s0{{1,2},{2,3},{3,4},{4,5},{5,6}};
            REQUIRE(s0.find(2) == s0.begin() + 1);
            REQUIRE(my_as_const(s0).find(3) == s0.cbegin() + 2);
            REQUIRE(s0.find(6) == s0.end());
            REQUIRE(my_as_const(s0).find(0) == s0.cend());
        }
        {
            map_t s0{{1,2},{2,3},{3,4},{4,5},{5,6}};
            REQUIRE(s0.equal_range(3) == std::make_pair(s0.begin() + 2, s0.begin() + 3));
            REQUIRE(s0.equal_range(6) == std::make_pair(s0.end(), s0.end()));
            REQUIRE(my_as_const(s0).equal_range(3) == std::make_pair(s0.cbegin() + 2, s0.cbegin() + 3));
            REQUIRE(my_as_const(s0).equal_range(0) == std::make_pair(s0.cbegin(), s0.cbegin()));
        }
        {
            map_t s0{{0,1},{3,2},{6,3}};
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
        using map_t = flat_map<int, unsigned, my_less>;
        map_t s0(my_less(42));
        REQUIRE(my_as_const(s0).key_comp().i == 42);
        REQUIRE(my_as_const(s0).value_comp()({2,50},{4,20}));
    }
    SECTION("operators") {
        using map_t = flat_map<int, unsigned>;

        REQUIRE(map_t{{1,2},{3,4}} == map_t{{3,4},{1,2}});
        REQUIRE_FALSE(map_t{{1,2},{3,4}} == map_t{{2,4},{1,2}});
        REQUIRE_FALSE(map_t{{1,2},{3,4}} == map_t{{1,3},{1,2}});
        REQUIRE_FALSE(map_t{{1,2},{3,4}} == map_t{{3,4},{1,2},{0,0}});

        REQUIRE_FALSE(map_t{{1,2},{3,4}} != map_t{{3,4},{1,2}});
        REQUIRE(map_t{{1,2},{3,4}} != map_t{{2,4},{1,2}});
        REQUIRE(map_t{{1,2},{3,4}} != map_t{{1,3},{1,2}});
        REQUIRE(map_t{{1,2},{3,4}} != map_t{{3,4},{1,2},{0,0}});

        REQUIRE(map_t{{0,2},{3,4}} < map_t{{1,2},{3,4}});
        REQUIRE(map_t{{1,1},{3,4}} < map_t{{1,2},{3,4}});
        REQUIRE(map_t{{1,2},{3,4}} < map_t{{1,2},{3,4},{5,6}});

        REQUIRE(map_t{{0,2},{3,4}} <= map_t{{1,2},{3,4}});
        REQUIRE(map_t{{1,1},{3,4}} <= map_t{{1,2},{3,4}});
        REQUIRE(map_t{{1,2},{3,4}} <= map_t{{1,2},{3,4},{5,6}});

        REQUIRE(map_t{{1,2},{3,4}} > map_t{{0,2},{3,4}});
        REQUIRE(map_t{{1,2},{3,4}} > map_t{{1,1},{3,4}});
        REQUIRE(map_t{{1,2},{3,4},{5,6}} > map_t{{1,2},{3,4}});

        REQUIRE(map_t{{1,2},{3,4}} >= map_t{{0,2},{3,4}});
        REQUIRE(map_t{{1,2},{3,4}} >= map_t{{1,1},{3,4}});
        REQUIRE(map_t{{1,2},{3,4},{5,6}} >= map_t{{1,2},{3,4}});

        REQUIRE_FALSE(map_t{{1,2},{3,4}} < map_t{{1,2},{3,4}});
        REQUIRE(map_t{{1,2},{3,4}} <= map_t{{1,2},{3,4}});
        REQUIRE_FALSE(map_t{{1,2},{3,4}} > map_t{{1,2},{3,4}});
        REQUIRE(map_t{{1,2},{3,4}} >= map_t{{1,2},{3,4}});

        const map_t s0;
        REQUIRE(s0 == s0);
        REQUIRE_FALSE(s0 != s0);
        REQUIRE_FALSE(s0 < s0);
        REQUIRE_FALSE(s0 > s0);
        REQUIRE(s0 <= s0);
        REQUIRE(s0 >= s0);
    }
}
