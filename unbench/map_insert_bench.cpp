/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/flat.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "bench_base.hpp"
using namespace flat_hpp_unbench;

#include <flat.hpp/flat_map.hpp>
using namespace flat_hpp;

#ifdef BOOST_CONTAINER_FOUND
#  include <boost/container/flat_map.hpp>
#endif

namespace
{
    template < typename Value >
    void flat_map_insert(benchmark::State& state) {
        std::vector<int> v;
        generate_random_vector(state.range(), v);
        for ( auto _ : state ) {
            flat_map<int, Value> s;
            for ( auto e : v ) {
                s.emplace(e, e);
            }
        }
    }

#ifdef BOOST_CONTAINER_FOUND
    template < typename Value >
    void boost_flat_map_insert(benchmark::State& state) {
        std::vector<int> v;
        generate_random_vector(state.range(), v);
        for ( auto _ : state ) {
            boost::container::flat_map<int, Value> s;
            for ( auto e : v ) {
                s.emplace(e, e);
            }
        }
    }
#endif

    template < typename Value >
    void std_map_insert(benchmark::State& state) {
        std::vector<int> v;
        generate_random_vector(state.range(), v);
        for ( auto _ : state ) {
            std::map<int, Value> s;
            for ( auto e : v ) {
                s.emplace(e, e);
            }
        }
    }

    template < typename Value >
    void std_unordered_map_insert(benchmark::State& state) {
        std::vector<int> v;
        generate_random_vector(state.range(), v);
        for ( auto _ : state ) {
            std::unordered_map<int, Value> s;
            for ( auto e : v ) {
                s.emplace(e, e);
            }
        }
    }
}

BENCHMARK_TEMPLATE(flat_map_insert, vec4)
    ->ComputeStatistics("min", min_bench_statistics)
    ->DenseRange(1,401,50);

#ifdef BOOST_CONTAINER_FOUND
    BENCHMARK_TEMPLATE(boost_flat_map_insert, vec4)
        ->ComputeStatistics("min", min_bench_statistics)
        ->DenseRange(1,401,50);
#endif

BENCHMARK_TEMPLATE(std_map_insert, vec4)
    ->ComputeStatistics("min", min_bench_statistics)
    ->DenseRange(1,401,50);

BENCHMARK_TEMPLATE(std_unordered_map_insert, vec4)
    ->ComputeStatistics("min", min_bench_statistics)
    ->DenseRange(1,401,50);
