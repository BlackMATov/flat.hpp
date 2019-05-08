/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/flat.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "bench_base.hpp"
using namespace flat_hpp_unbench;

#include <flat_hpp/flat_set.hpp>
using namespace flat_hpp;

#ifdef BOOST_CONTAINER_FOUND
#  include <boost/container/flat_set.hpp>
#endif

namespace
{
    template < typename Key >
    void flat_set_insert(benchmark::State& state) {
        std::vector<int> v;
        generate_random_vector(state.range(), v);
        for ( auto _ : state ) {
            flat_set<Key> s;
            for ( auto e : v ) {
                s.emplace(e);
            }
        }
    }

#ifdef BOOST_CONTAINER_FOUND
    template < typename Key >
    void boost_flat_set_insert(benchmark::State& state) {
        std::vector<int> v;
        generate_random_vector(state.range(), v);
        for ( auto _ : state ) {
            boost::container::flat_set<Key> s;
            for ( auto e : v ) {
                s.emplace(e);
            }
        }
    }
#endif

    template < typename Key >
    void std_set_insert(benchmark::State& state) {
        std::vector<int> v;
        generate_random_vector(state.range(), v);
        for ( auto _ : state ) {
            std::set<Key> s;
            for ( auto e : v ) {
                s.emplace(e);
            }
        }
    }

    template < typename Key >
    void std_unordered_set_insert(benchmark::State& state) {
        std::vector<int> v;
        generate_random_vector(state.range(), v);
        for ( auto _ : state ) {
            std::unordered_set<Key> s;
            for ( auto e : v ) {
                s.emplace(e);
            }
        }
    }
}

BENCHMARK_TEMPLATE(flat_set_insert, vec4)
    ->ComputeStatistics("min", min_bench_statistics)
    ->DenseRange(1,401,50);

#ifdef BOOST_CONTAINER_FOUND
    BENCHMARK_TEMPLATE(boost_flat_set_insert, vec4)
        ->ComputeStatistics("min", min_bench_statistics)
        ->DenseRange(1,401,50);
#endif

BENCHMARK_TEMPLATE(std_set_insert, vec4)
    ->ComputeStatistics("min", min_bench_statistics)
    ->DenseRange(1,401,50);

BENCHMARK_TEMPLATE(std_unordered_set_insert, vec4)
    ->ComputeStatistics("min", min_bench_statistics)
    ->DenseRange(1,401,50);
