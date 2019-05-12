/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/flat.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#define CATCH_CONFIG_FAST_COMPILE
#include <catch2/catch.hpp>

#include <deque>

#include <flat_hpp/flat_multimap.hpp>
using namespace flat_hpp;

namespace
{
    template < typename T >
    class dummy_less {
    public:
        dummy_less() = default;
        dummy_less(int i) noexcept : i(i) {}
        bool operator()(const T& l, const T& r) const {
            return l < r;
        }
        int i = 0;
    };

    template < typename T >
    class dummy_less2 {
        dummy_less2() = default;
        dummy_less2(dummy_less2&&) noexcept(false) {}
        bool operator()(const T& l, const T& r) const {
            return l < r;
        }
    };

    template < typename T >
    void swap(dummy_less2<T>&, dummy_less2<T>&) noexcept {
    }

    template < typename T >
    class dummy_less3 {
        dummy_less3() = default;
        dummy_less3(dummy_less3&&) noexcept(false) {}
        bool operator()(const T& l, const T& r) const {
            return l < r;
        }
    };

    template < typename T >
    constexpr std::add_const_t<T>& my_as_const(T& t) noexcept {
        return t;
    }
}

TEST_CASE("flat_multimap") {
    SECTION("sizeof") {
        REQUIRE(sizeof(flat_multimap<int, unsigned>) == sizeof(std::vector<std::pair<int, unsigned>>));

        struct vc : flat_multimap<int, unsigned>::value_compare {
            int i;
        };
        REQUIRE(sizeof(vc) == sizeof(int));
    }
    SECTION("noexcept") {
        using alloc_t = std::allocator<std::pair<int,unsigned>>;
        using map_t = flat_multimap<int, unsigned, dummy_less<int>, std::vector<std::pair<int,unsigned>, alloc_t>>;
        using map2_t = flat_multimap<int, unsigned, dummy_less2<int>>;
        using map3_t = flat_multimap<int, unsigned, dummy_less3<int>>;

        STATIC_REQUIRE(std::is_nothrow_default_constructible_v<map_t>);
        STATIC_REQUIRE(std::is_nothrow_move_constructible_v<map_t>);
        STATIC_REQUIRE(std::is_nothrow_move_assignable_v<map_t>);
        STATIC_REQUIRE(std::is_nothrow_swappable_v<map_t>);
        STATIC_REQUIRE(std::is_nothrow_swappable_v<map2_t>);
        STATIC_REQUIRE(!std::is_nothrow_swappable_v<map3_t>);
    }
    SECTION("types") {
        using map_t = flat_multimap<int, unsigned>;

        STATIC_REQUIRE(std::is_same_v<map_t::key_type, int>);
        STATIC_REQUIRE(std::is_same_v<map_t::mapped_type, unsigned>);
        STATIC_REQUIRE(std::is_same_v<map_t::value_type, std::pair<int, unsigned>>);

        STATIC_REQUIRE(std::is_same_v<map_t::size_type, std::size_t>);
        STATIC_REQUIRE(std::is_same_v<map_t::difference_type, std::ptrdiff_t>);

        STATIC_REQUIRE(std::is_same_v<map_t::reference, std::pair<int, unsigned>&>);
        STATIC_REQUIRE(std::is_same_v<map_t::const_reference, const std::pair<int, unsigned>&>);

        STATIC_REQUIRE(std::is_same_v<map_t::pointer, std::pair<int, unsigned>*>);
        STATIC_REQUIRE(std::is_same_v<map_t::const_pointer, const std::pair<int, unsigned>*>);
    }
    SECTION("ctors") {
        using alloc_t = std::allocator<
            std::pair<int,unsigned>>;

        using map_t = flat_multimap<
            int,
            unsigned,
            std::less<int>,
            std::vector<std::pair<int,unsigned>, alloc_t>>;

        using map2_t = flat_multimap<
            int,
            unsigned,
            std::greater<int>,
            std::vector<std::pair<int,unsigned>, alloc_t>>;

        using vec_t = std::vector<
            std::pair<int,unsigned>, alloc_t>;

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
            auto s0 = map_t{{0,1}, {1,2}};
            auto s1 = s0;
            REQUIRE(s0 == map_t{{0,1}, {1,2}});
            REQUIRE(s1 == map_t{{0,1}, {1,2}});
            auto s2 = std::move(s1);
            REQUIRE(s1.empty());
            REQUIRE(s2 == map_t{{0,1}, {1,2}});
            auto s3 = map_t(s2, alloc_t());
            REQUIRE(s2 == s3);
            auto s4 = map_t(std::move(s3), alloc_t());
            REQUIRE(s3.empty());
            REQUIRE(s4 == map_t{{0,1}, {1,2}});
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
        using map_t = flat_multimap<int, unsigned>;

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
            REQUIRE(s0.size() == 2u);

            s0.insert({3,84});
            REQUIRE(s0.size() == 3u);

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

            using alloc2_t = std::allocator<
                std::pair<int, unsigned>>;

            using map2_t = flat_multimap<
                int,
                unsigned,
                std::less<int>,
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

        using map_t = flat_multimap<obj_t, unsigned>;
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

        using map_t = flat_multimap<obj_t, obj_t>;

        {
            map_t s0;

            auto k1_42 = std::make_pair(1, 42);
            auto k3_84 = std::make_pair(3, 84);

            auto i0 = s0.insert(k1_42);
            REQUIRE(s0 == map_t{{1,42}});
            REQUIRE(i0 == s0.begin());

            auto i1 = s0.insert(std::make_pair(1, obj_t(21)));
            REQUIRE(s0 == map_t{{1,42},{1,21}});
            REQUIRE(i1 == s0.begin() + 1);

            auto i2 = s0.insert(std::make_pair(2, obj_t(42)));
            REQUIRE(s0 == map_t{{1,42},{1,21},{2,42}});
            REQUIRE(i2 == s0.begin() + 2);

            auto i3 = s0.insert(s0.cend(), k3_84);
            REQUIRE(i3 == s0.begin() + 3);

            s0.insert(s0.cend(), std::make_pair(4, obj_t(84)));
            auto i4 = s0.insert(s0.cend(), std::make_pair(0, obj_t(21)));
            REQUIRE(i4 == s0.begin());

            auto i5 = s0.emplace(5, 100500);
            REQUIRE(i5 == s0.end() - 1);
            REQUIRE(s0 == map_t{{0,21},{1,42},{1,21},{2,42},{3,84},{4,84},{5,100500}});

            auto i6 = s0.emplace_hint(s0.cend(), 6, 100500);
            REQUIRE(i6 == s0.end() - 1);
            REQUIRE(s0 == map_t{{0,21},{1,42},{1,21},{2,42},{3,84},{4,84},{5,100500},{6,100500}});
        }
    }
    SECTION("erasers") {
        using map_t = flat_multimap<int, unsigned>;
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
            map_t s0{{1,2},{2,3},{2,1},{3,4}};
            REQUIRE(s0.erase(1) == 1);
            REQUIRE(s0.erase(2) == 2);
            REQUIRE(s0.erase(6) == 0);
            REQUIRE(s0 == map_t{{3,4}});
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
        using map_t = flat_multimap<int, unsigned>;
        {
            map_t s0{{1,2},{2,3},{2,1},{3,4},{4,5},{5,6}};
            REQUIRE(s0.count(3) == 1);
            REQUIRE(s0.count(2) == 2);
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
            map_t s0{{1,2},{2,3},{2,1},{3,4},{4,5},{5,6}};
            REQUIRE(s0.equal_range(2) == std::make_pair(s0.begin() + 1, s0.begin() + 3));
            REQUIRE(s0.equal_range(6) == std::make_pair(s0.end(), s0.end()));
            REQUIRE(my_as_const(s0).equal_range(2) == std::make_pair(s0.cbegin() + 1, s0.cbegin() + 3));
            REQUIRE(my_as_const(s0).equal_range(0) == std::make_pair(s0.cbegin(), s0.cbegin()));
        }
        {
            map_t s0{{0,1},{0,0},{3,2},{6,3}};
            REQUIRE(s0.lower_bound(0) == s0.begin());
            REQUIRE(s0.lower_bound(1) == s0.begin() + 2);
            REQUIRE(s0.lower_bound(10) == s0.end());
            REQUIRE(my_as_const(s0).lower_bound(-1) == s0.cbegin());
            REQUIRE(my_as_const(s0).lower_bound(7) == s0.cbegin() + 4);
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
        using map_t = flat_multimap<int, unsigned, my_less>;
        map_t s0(my_less(42));
        REQUIRE(my_as_const(s0).key_comp().i == 42);
        REQUIRE(my_as_const(s0).value_comp()({2,50},{4,20}));
    }
    SECTION("custom_less") {
        using map_t = flat_multimap<int, unsigned, dummy_less<int>>;
        auto s0 = map_t(dummy_less<int>(42));
        auto s1 = map_t(dummy_less<int>(21));
        REQUIRE(s0.key_comp().i == 42);
        REQUIRE(s1.key_comp().i == 21);
        s0.swap(s1);
        REQUIRE(s0.key_comp().i == 21);
        REQUIRE(s1.key_comp().i == 42);
    }
    SECTION("operators") {
        using map_t = flat_multimap<int, unsigned>;

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
