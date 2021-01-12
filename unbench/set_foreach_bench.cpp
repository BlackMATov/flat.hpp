/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/flat.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019-2021, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "bench_base.hpp"
using namespace flat_hpp_unbench;

#include <flat.hpp/flat_set.hpp>
using namespace flat_hpp;

#ifdef BOOST_CONTAINER_FOUND
#  include <boost/container/flat_set.hpp>
#endif

namespace
{
    template < typename Value >
    void flat_set_foreach(benchmark::State& state) {
        std::vector<int> v;
        generate_random_vector(state.range(), v);
        flat_set<Value> s(v.begin(), v.end());
        for ( auto _ : state ) {
            int acc = 0;
            for ( const auto& e : s ) {
                acc += e.x;
            }
            benchmark::DoNotOptimize(acc);
        }
    }

#ifdef BOOST_CONTAINER_FOUND
    template < typename Value >
    void boost_flat_set_foreach(benchmark::State& state) {
        std::vector<int> v;
        generate_random_vector(state.range(), v);
        boost::container::flat_set<Value> s(v.begin(), v.end());
        for ( auto _ : state ) {
            int acc = 0;
            for ( const auto& e : s ) {
                acc += e.x;
            }
            benchmark::DoNotOptimize(acc);
        }
    }
#endif

    template < typename Value >
    void std_set_foreach(benchmark::State& state) {
        std::vector<int> v;
        generate_random_vector(state.range(), v);
        std::set<Value> s(v.begin(), v.end());
        for ( auto _ : state ) {
            int acc = 0;
            for ( const auto& e : s ) {
                acc += e.x;
            }
            benchmark::DoNotOptimize(acc);
        }
    }

    template < typename Value >
    void std_unordered_set_foreach(benchmark::State& state) {
        std::vector<int> v;
        generate_random_vector(state.range(), v);
        std::unordered_set<Value> s(v.begin(), v.end());
        for ( auto _ : state ) {
            int acc = 0;
            for ( const auto& e : s ) {
                acc += e.x;
            }
            benchmark::DoNotOptimize(acc);
        }
    }
}

BENCHMARK_TEMPLATE(flat_set_foreach, vec4)
    ->ComputeStatistics("min", min_bench_statistics)
    ->DenseRange(1,401,50);

#ifdef BOOST_CONTAINER_FOUND
    BENCHMARK_TEMPLATE(boost_flat_set_foreach, vec4)
        ->ComputeStatistics("min", min_bench_statistics)
        ->DenseRange(1,401,50);
#endif

BENCHMARK_TEMPLATE(std_set_foreach, vec4)
    ->ComputeStatistics("min", min_bench_statistics)
    ->DenseRange(1,401,50);

BENCHMARK_TEMPLATE(std_unordered_set_foreach, vec4)
    ->ComputeStatistics("min", min_bench_statistics)
    ->DenseRange(1,401,50);
