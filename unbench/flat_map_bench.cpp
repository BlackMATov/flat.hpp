/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/flat.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <benchmark/benchmark.h>

#include <flat_hpp/flat_map.hpp>
using namespace flat_hpp;

namespace
{
    void flat_map_default_ctor(benchmark::State& state) {
        for ( auto _ : state ) {
            flat_map<int, unsigned> s0;
        }
    }
}

BENCHMARK(flat_map_default_ctor);
