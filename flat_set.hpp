/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/flat.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include <vector>
#include <memory>
#include <utility>
#include <functional>
#include <type_traits>

namespace flat_hpp
{
    template < typename Key
             , typename Compare = std::less<Key>
             , typename Allocator = std::allocator<Key> >
    class flat_set final {
        using data_type = std::vector<Key, Allocator>;
    public:
        using key_type = Key;
        using value_type = Key;

        using size_type = typename data_type::size_type;
        using difference_type = typename data_type::difference_type;

        using key_compare = Compare;
        using value_compare = Compare;
        using allocator_type = Allocator;

        using reference = typename data_type::reference;
        using const_reference = typename data_type::const_reference;
        using pointer = typename data_type::pointer;
        using const_pointer = typename data_type::const_pointer;

        using iterator = typename data_type::iterator;
        using const_iterator = typename data_type::const_iterator;
        using reverse_iterator = typename data_type::reverse_iterator;
        using const_reverse_iterator = typename data_type::const_reverse_iterator;
    };
}
