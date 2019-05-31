/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/flat.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

namespace flat_hpp::detail
{
    template < typename Iter, typename Compare >
    bool is_sorted(Iter first, Iter last, Compare comp) {
        (void)first;
        (void)last;
        (void)comp;
        return true;
    }

    template < typename Iter, typename Compare >
    bool is_sorted_unique(Iter first, Iter last, Compare comp) {
        (void)first;
        (void)last;
        (void)comp;
        return true;
    }
}
