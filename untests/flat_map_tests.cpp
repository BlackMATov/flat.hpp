/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/flat.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019-2021, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <flat.hpp/flat_map.hpp>
#include "doctest/doctest.hpp"

#include <deque>
#include <string>
#include <string_view>

namespace
{
    using namespace flat_hpp;

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

TEST_CASE("flat_map") {
    SUBCASE("guides") {
        {
            std::vector<std::pair<int,unsigned>> vs;

            auto s0 = flat_map(vs.begin(), vs.end());
            auto s1 = flat_map(flat_hpp::sorted_range, vs.begin(), vs.end());
            auto s2 = flat_map(flat_hpp::sorted_unique_range, vs.begin(), vs.end());

            auto s3 = flat_map(vs.begin(), vs.end(), std::less<int>());
            auto s4 = flat_map(flat_hpp::sorted_range, vs.begin(), vs.end(), std::less<int>());
            auto s5 = flat_map(flat_hpp::sorted_unique_range, vs.begin(), vs.end(), std::less<int>());

            auto s6 = flat_map(vs.begin(), vs.end(), std::allocator<int>());
            auto s7 = flat_map(flat_hpp::sorted_range, vs.begin(), vs.end(), std::allocator<int>());
            auto s8 = flat_map(flat_hpp::sorted_unique_range, vs.begin(), vs.end(), std::allocator<int>());

            auto s9 = flat_map(vs.begin(), vs.end(), std::less<int>(), std::allocator<int>());
            auto s10 = flat_map(flat_hpp::sorted_range, vs.begin(), vs.end(), std::less<int>(), std::allocator<int>());
            auto s11 = flat_map(flat_hpp::sorted_unique_range, vs.begin(), vs.end(), std::less<int>(), std::allocator<int>());

            STATIC_REQUIRE(std::is_same_v<decltype(s0)::key_type, int>);
            STATIC_REQUIRE(std::is_same_v<decltype(s1)::key_type, int>);
            STATIC_REQUIRE(std::is_same_v<decltype(s2)::key_type, int>);
            STATIC_REQUIRE(std::is_same_v<decltype(s3)::key_type, int>);
            STATIC_REQUIRE(std::is_same_v<decltype(s4)::key_type, int>);
            STATIC_REQUIRE(std::is_same_v<decltype(s5)::key_type, int>);
            STATIC_REQUIRE(std::is_same_v<decltype(s6)::key_type, int>);
            STATIC_REQUIRE(std::is_same_v<decltype(s7)::key_type, int>);
            STATIC_REQUIRE(std::is_same_v<decltype(s8)::key_type, int>);
            STATIC_REQUIRE(std::is_same_v<decltype(s9)::key_type, int>);
            STATIC_REQUIRE(std::is_same_v<decltype(s10)::key_type, int>);
            STATIC_REQUIRE(std::is_same_v<decltype(s11)::key_type, int>);

            STATIC_REQUIRE(std::is_same_v<decltype(s0)::mapped_type, unsigned>);
            STATIC_REQUIRE(std::is_same_v<decltype(s1)::mapped_type, unsigned>);
            STATIC_REQUIRE(std::is_same_v<decltype(s2)::mapped_type, unsigned>);
            STATIC_REQUIRE(std::is_same_v<decltype(s3)::mapped_type, unsigned>);
            STATIC_REQUIRE(std::is_same_v<decltype(s4)::mapped_type, unsigned>);
            STATIC_REQUIRE(std::is_same_v<decltype(s5)::mapped_type, unsigned>);
            STATIC_REQUIRE(std::is_same_v<decltype(s6)::mapped_type, unsigned>);
            STATIC_REQUIRE(std::is_same_v<decltype(s7)::mapped_type, unsigned>);
            STATIC_REQUIRE(std::is_same_v<decltype(s8)::mapped_type, unsigned>);
            STATIC_REQUIRE(std::is_same_v<decltype(s9)::mapped_type, unsigned>);
            STATIC_REQUIRE(std::is_same_v<decltype(s10)::mapped_type, unsigned>);
            STATIC_REQUIRE(std::is_same_v<decltype(s11)::mapped_type, unsigned>);
        }

        {
            auto s1 = flat_map({std::pair{1,1u}, std::pair{2,2u}});
            auto s2 = flat_map(flat_hpp::sorted_range, {std::pair{1,1u}, std::pair{2,2u}});
            auto s3 = flat_map(flat_hpp::sorted_unique_range, {std::pair{1,1u}, std::pair{2,2u}});

            STATIC_REQUIRE(std::is_same_v<decltype(s1)::key_type, int>);
            STATIC_REQUIRE(std::is_same_v<decltype(s2)::key_type, int>);
            STATIC_REQUIRE(std::is_same_v<decltype(s3)::key_type, int>);

            STATIC_REQUIRE(std::is_same_v<decltype(s1)::mapped_type, unsigned>);
            STATIC_REQUIRE(std::is_same_v<decltype(s2)::mapped_type, unsigned>);
            STATIC_REQUIRE(std::is_same_v<decltype(s3)::mapped_type, unsigned>);
        }

        {
            auto s1 = flat_map({std::pair{1,1u}, std::pair{2,2u}}, std::less<int>());
            auto s2 = flat_map(flat_hpp::sorted_range, {std::pair{1,1u}, std::pair{2,2u}}, std::less<int>());
            auto s3 = flat_map(flat_hpp::sorted_unique_range, {std::pair{1,1u}, std::pair{2,2u}}, std::less<int>());

            STATIC_REQUIRE(std::is_same_v<decltype(s1)::key_type, int>);
            STATIC_REQUIRE(std::is_same_v<decltype(s2)::key_type, int>);
            STATIC_REQUIRE(std::is_same_v<decltype(s3)::key_type, int>);

            STATIC_REQUIRE(std::is_same_v<decltype(s1)::mapped_type, unsigned>);
            STATIC_REQUIRE(std::is_same_v<decltype(s2)::mapped_type, unsigned>);
            STATIC_REQUIRE(std::is_same_v<decltype(s3)::mapped_type, unsigned>);
        }

        {
            auto s1 = flat_map({std::pair{1,1u}, std::pair{2,2u}}, std::allocator<int>());
            auto s2 = flat_map(flat_hpp::sorted_range, {std::pair{1,1u}, std::pair{2,2u}}, std::allocator<int>());
            auto s3 = flat_map(flat_hpp::sorted_unique_range, {std::pair{1,1u}, std::pair{2,2u}}, std::allocator<int>());

            STATIC_REQUIRE(std::is_same_v<decltype(s1)::key_type, int>);
            STATIC_REQUIRE(std::is_same_v<decltype(s2)::key_type, int>);
            STATIC_REQUIRE(std::is_same_v<decltype(s3)::key_type, int>);

            STATIC_REQUIRE(std::is_same_v<decltype(s1)::mapped_type, unsigned>);
            STATIC_REQUIRE(std::is_same_v<decltype(s2)::mapped_type, unsigned>);
            STATIC_REQUIRE(std::is_same_v<decltype(s3)::mapped_type, unsigned>);
        }

        {
            auto s1 = flat_map({std::pair{1,1u}, std::pair{2,2u}}, std::less<int>(), std::allocator<int>());
            auto s2 = flat_map(flat_hpp::sorted_range, {std::pair{1,1u}, std::pair{2,2u}}, std::less<int>(), std::allocator<int>());
            auto s3 = flat_map(flat_hpp::sorted_unique_range, {std::pair{1,1u}, std::pair{2,2u}}, std::less<int>(), std::allocator<int>());

            STATIC_REQUIRE(std::is_same_v<decltype(s1)::key_type, int>);
            STATIC_REQUIRE(std::is_same_v<decltype(s2)::key_type, int>);
            STATIC_REQUIRE(std::is_same_v<decltype(s3)::key_type, int>);

            STATIC_REQUIRE(std::is_same_v<decltype(s1)::mapped_type, unsigned>);
            STATIC_REQUIRE(std::is_same_v<decltype(s2)::mapped_type, unsigned>);
            STATIC_REQUIRE(std::is_same_v<decltype(s3)::mapped_type, unsigned>);
        }
    }
    SUBCASE("detail") {
        STATIC_REQUIRE(detail::is_transparent<std::less<>, int>::value);
        STATIC_REQUIRE_FALSE(detail::is_transparent<std::less<int>, int>::value);
    }
    SUBCASE("sizeof") {
        REQUIRE(sizeof(flat_map<int, unsigned>) == sizeof(std::vector<std::pair<int, unsigned>>));

        struct vc : flat_map<int, unsigned>::value_compare {
            int i;
        };
        REQUIRE(sizeof(vc) == sizeof(int));
    }
    SUBCASE("noexcept") {
        using alloc_t = std::allocator<std::pair<int,unsigned>>;
        using map_t = flat_map<int, unsigned, dummy_less<int>, std::vector<std::pair<int,unsigned>, alloc_t>>;
        using map2_t = flat_map<int, unsigned, dummy_less2<int>>;
        using map3_t = flat_map<int, unsigned, dummy_less3<int>>;

        STATIC_REQUIRE(std::is_nothrow_default_constructible_v<map_t>);
        STATIC_REQUIRE(std::is_nothrow_move_constructible_v<map_t>);
        STATIC_REQUIRE(std::is_nothrow_move_assignable_v<map_t>);
        STATIC_REQUIRE(std::is_nothrow_swappable_v<map_t>);
        STATIC_REQUIRE(std::is_nothrow_swappable_v<map2_t>);
        STATIC_REQUIRE(!std::is_nothrow_swappable_v<map3_t>);

        STATIC_REQUIRE(noexcept(std::declval<map_t&>().begin()));
        STATIC_REQUIRE(noexcept(std::declval<const map_t&>().begin()));
        STATIC_REQUIRE(noexcept(std::declval<const map_t&>().cbegin()));
        STATIC_REQUIRE(noexcept(std::declval<map_t&>().end()));
        STATIC_REQUIRE(noexcept(std::declval<const map_t&>().end()));
        STATIC_REQUIRE(noexcept(std::declval<const map_t&>().cend()));

        STATIC_REQUIRE(noexcept(std::declval<map_t&>().rbegin()));
        STATIC_REQUIRE(noexcept(std::declval<const map_t&>().rbegin()));
        STATIC_REQUIRE(noexcept(std::declval<const map_t&>().crbegin()));
        STATIC_REQUIRE(noexcept(std::declval<map_t&>().rend()));
        STATIC_REQUIRE(noexcept(std::declval<const map_t&>().rend()));
        STATIC_REQUIRE(noexcept(std::declval<const map_t&>().crend()));

        STATIC_REQUIRE(noexcept(std::declval<const map_t&>().empty()));
        STATIC_REQUIRE(noexcept(std::declval<const map_t&>().size()));
        STATIC_REQUIRE(noexcept(std::declval<const map_t&>().max_size()));
        STATIC_REQUIRE(noexcept(std::declval<const map_t&>().capacity()));

        STATIC_REQUIRE(noexcept(std::declval<map_t&>().clear()));
    }
    SUBCASE("types") {
        using map_t = flat_map<int, unsigned>;

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
    SUBCASE("ctors") {
        using alloc_t = std::allocator<
            std::pair<int,unsigned>>;

        using map_t = flat_map<
            int,
            unsigned,
            std::less<int>,
            std::vector<std::pair<int,unsigned>, alloc_t>>;

        using map2_t = flat_map<
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

        {
            auto s0 = map_t(sorted_range, {{1,4},{2,3},{2,2},{3,1}});
            REQUIRE(s0 == map_t{{1,4},{2,3},{3,1}});

            vec_t v1({{1,4},{2,3},{2,2},{3,1}});
            auto s1 = map_t(sorted_range, v1.begin(), v1.end());
            REQUIRE(s1 == map_t{{1,4},{2,3},{3,1}});
        }
    }
    SUBCASE("capacity") {
        using map_t = flat_map<int, unsigned>;

        {
            map_t s0;

            REQUIRE(s0.empty());
            REQUIRE_FALSE(s0.size());
            REQUIRE(s0.max_size() == std::vector<std::pair<int,unsigned>>().max_size());

            s0.insert({2,42});

            REQUIRE_FALSE(s0.empty());
            REQUIRE(s0.size() == 1u);
            REQUIRE(s0.max_size() == std::vector<std::pair<int,unsigned>>().max_size());

            s0.insert({2,84});
            REQUIRE(s0.size() == 1u);

            s0.insert({3,84});
            REQUIRE(s0.size() == 2u);

            s0.clear();

            REQUIRE(s0.empty());
            REQUIRE_FALSE(s0.size());
            REQUIRE(s0.max_size() == std::vector<std::pair<int,unsigned>>().max_size());
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

            using map2_t = flat_map<
                int,
                unsigned,
                std::less<int>,
                std::deque<std::pair<int, unsigned>, alloc2_t>>;

            map2_t s1;
            s1.insert({{1,2},{2,3},{3,4}});
            REQUIRE(s1 == map2_t{{1,2},{2,3},{3,4}});
        }
    }
    SUBCASE("access") {
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
    SUBCASE("inserts") {
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

        {
            map_t s0;
            s0.insert({{6,2},{4,6},{2,4},{4,2}});
            REQUIRE(s0 == map_t{{2,4},{4,6},{6,2}});
            s0.insert({{9,3},{7,5},{3,9},{5,3},{5,3}});
            REQUIRE(s0 == map_t{{2,4},{3,9},{4,6},{5,3},{6,2},{7,5},{9,3}});
        }

        {
            map_t s0;
            s0.insert(sorted_unique_range, {{1,3},{2,2},{3,1}});
            REQUIRE(s0 == map_t{{1,3},{2,2},{3,1}});

            map_t s1;
            s1.insert(sorted_range, {{1,3},{2,2},{2,2},{3,1}});
            REQUIRE(s1 == map_t{{1,3},{2,2},{3,1}});
        }

        {
            map_t s0;
            auto i0 = s0.insert_or_assign(1, 4);
            REQUIRE(i0 == std::make_pair(s0.begin(), true));
            REQUIRE(s0 == map_t{{1,4}});
            auto i1 = s0.insert_or_assign(1, 8);
            REQUIRE(i1 == std::make_pair(s0.begin(), false));
            REQUIRE(s0 == map_t{{1,8}});

            const obj_t k0{2};
            auto i2 = s0.insert_or_assign(k0, 6);
            REQUIRE(i2 == std::make_pair(s0.begin() + 1, true));
            REQUIRE(s0 == map_t{{1,8}, {2,6}});
            auto i3 = s0.insert_or_assign(k0, 2);
            REQUIRE(i3 == std::make_pair(s0.begin() + 1, false));
            REQUIRE(s0 == map_t{{1,8}, {2,2}});
        }

        {
            map_t s0;
            auto i0 = s0.try_emplace(1, 4);
            REQUIRE(i0 == std::make_pair(s0.begin(), true));
            REQUIRE(s0 == map_t{{1,4}});
            auto i1 = s0.try_emplace(1, 8);
            REQUIRE(i1 == std::make_pair(s0.begin(), false));
            REQUIRE(s0 == map_t{{1,4}});
        }
    }
    SUBCASE("erasers") {
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
    SUBCASE("lookup") {
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
            REQUIRE(my_as_const(s0).contains(1));
            REQUIRE(my_as_const(s0).contains(3));
            REQUIRE_FALSE(my_as_const(s0).contains(0));
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
    SUBCASE("heterogeneous") {
        flat_map<std::string, int, std::less<>> s0{{"hello", 42}, {"world", 84}};
        REQUIRE(s0.find(std::string_view("hello")) == s0.begin());
        REQUIRE(my_as_const(s0).find(std::string_view("world")) == s0.begin() + 1);
        REQUIRE(s0.find(std::string_view("42")) == s0.end());
        REQUIRE(my_as_const(s0).find(std::string_view("42")) == s0.cend());

        REQUIRE(my_as_const(s0).contains(std::string_view("hello")));
        REQUIRE_FALSE(my_as_const(s0).contains(std::string_view("42")));

        REQUIRE(my_as_const(s0).count(std::string_view("hello")) == 1);
        REQUIRE(my_as_const(s0).count(std::string_view("hello_42")) == 0);

        REQUIRE(s0.upper_bound(std::string_view("hello")) == s0.begin() + 1);
        REQUIRE(my_as_const(s0).upper_bound(std::string_view("hello")) == s0.begin() + 1);

        REQUIRE(s0.at(std::string_view("world")) == 84);
        REQUIRE(my_as_const(s0).at(std::string_view("world")) == 84);
    }
    SUBCASE("observers") {
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
    SUBCASE("custom_less") {
        using map_t = flat_map<int, unsigned, dummy_less<int>>;
        auto s0 = map_t(dummy_less<int>(42));
        auto s1 = map_t(dummy_less<int>(21));
        REQUIRE(s0.key_comp().i == 42);
        REQUIRE(s1.key_comp().i == 21);
        s0.swap(s1);
        REQUIRE(s0.key_comp().i == 21);
        REQUIRE(s1.key_comp().i == 42);
    }
    SUBCASE("operators") {
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
