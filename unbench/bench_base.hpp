/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/flat.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019-2021, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include <set>
#include <map>
#include <deque>
#include <vector>
#include <random>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <unordered_set>
#include <unordered_map>

#include <benchmark/benchmark.h>

namespace flat_hpp_unbench
{
    struct vec2 {
        int x = 0;
        int y = 0;

        vec2(int v) noexcept
        : x(v), y(v) {}

        bool operator<(const vec2& o) const noexcept {
            return
                (x < o.x) ||
                (x == o.x && y < o.y);
        }

        bool operator==(const vec2& o) const noexcept {
            return x == o.x
                && y == o.y;
        }

        std::size_t hash() const noexcept {
            std::hash<int> hasher;
            std::size_t seed = hasher(x);
            seed ^= hasher(y) + 0x9e3779b9 + (seed<<6) + (seed>>2);
            return seed;
        }
    };

    struct vec4 {
        int x = 0;
        int y = 0;
        int z = 0;
        int w = 0;

        vec4(int v) noexcept
        : x(v), y(v), z(v), w(v) {}

        bool operator<(const vec4& o) const noexcept {
            return
                (x < o.x) ||
                (x == o.x && y < o.y) ||
                (x == o.x && y == o.y && z < o.z) ||
                (x == o.x && y == o.y && z == o.z && w < o.w);
        }

        bool operator==(const vec4& o) const noexcept {
            return x == o.x
                && y == o.y
                && z == o.z
                && w == o.w;
        }

        std::size_t hash() const noexcept {
            std::hash<int> hasher;
            std::size_t seed = hasher(x);
            seed ^= hasher(y) + 0x9e3779b9 + (seed<<6) + (seed>>2);
            seed ^= hasher(z) + 0x9e3779b9 + (seed<<6) + (seed>>2);
            seed ^= hasher(w) + 0x9e3779b9 + (seed<<6) + (seed>>2);
            return seed;
        }
    };

    inline void generate_random_vector(std::size_t n, std::vector<int>& v) {
        std::mt19937 engine(n);
        std::uniform_int_distribution<int> dist;

        std::vector<int> nv(n);
        for ( std::size_t i = 0; i < n; ++i ) {
            nv[i] = dist(engine);
        }

        v = std::move(nv);
    }

    inline void generate_random_vector(std::size_t n, std::vector<std::pair<int,int>>& v) {
        std::mt19937 engine(n);
        std::uniform_int_distribution<int> dist;

        std::vector<std::pair<int,int>> nv(n);
        for ( std::size_t i = 0; i < n; ++i ) {
            nv[i] = std::make_pair(dist(engine), dist(engine));
        }

        v = std::move(nv);
    }

    inline double min_bench_statistics(const std::vector<double>& v) {
        return v.empty()
            ? 0.0
            : *(std::min_element(v.begin(), v.end()));
    };
}

namespace std
{
    template <>
    struct hash<flat_hpp_unbench::vec2> {
        size_t operator()(const flat_hpp_unbench::vec2& v) const {
            return v.hash();
        }
    };

    template <>
    struct hash<flat_hpp_unbench::vec4> {
        size_t operator()(const flat_hpp_unbench::vec4& v) const {
            return v.hash();
        }
    };
}
