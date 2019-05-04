/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/flat.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include <vector>
#include <memory>
#include <utility>
#include <iterator>
#include <algorithm>
#include <functional>
#include <type_traits>
#include <initializer_list>

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

        static_assert(
            std::is_same<typename allocator_type::value_type, value_type>::value,
            "Allocator::value_type must be same type as value_type");
    public:
        explicit flat_set(
            const Allocator& a)
        : data_(a) {}

        explicit flat_set(
            const Compare& c = Compare(),
            const Allocator& a = Allocator())
        : data_(a)
        , compare_(c) {}

        template < typename InputIter >
        flat_set(
            InputIter first,
            InputIter last,
            const Allocator& a)
        : data_(a) {
            insert(first, last);
        }

        template < typename InputIter >
        flat_set(
            InputIter first,
            InputIter last,
            const Compare& c = Compare(),
            const Allocator& a = Allocator())
        : data_(a)
        , compare_(c) {
            insert(first, last);
        }

        flat_set(
            std::initializer_list<value_type> il,
            const Allocator& a)
        : data_(a) {
            insert(il.begin(), il.end());
        }

        flat_set(
            std::initializer_list<value_type> il,
            const Compare& c = Compare(),
            const Allocator& a = Allocator())
        : data_(a)
        , compare_(c) {
            insert(il.begin(), il.end());
        }

        iterator begin() noexcept { return data_.begin(); }
        const_iterator begin() const noexcept { return data_.begin(); }
        const_iterator cbegin() const noexcept { return data_.cbegin(); }

        iterator end() noexcept { return data_.end(); }
        const_iterator end() const noexcept { return data_.end(); }
        const_iterator cend() const noexcept { return data_.cend(); }

        reverse_iterator rbegin() noexcept { return data_.rbegin(); }
        const_reverse_iterator rbegin() const noexcept { return data_.rbegin(); }
        const_reverse_iterator crbegin() const noexcept { return data_.crbegin(); }

        reverse_iterator rend() noexcept { return data_.rend(); }
        const_reverse_iterator rend() const noexcept { return data_.rend(); }
        const_reverse_iterator crend() const noexcept { return data_.crend(); }

        template < typename P
                 , typename = std::enable_if_t<std::is_constructible<value_type, P>::value> >
        std::pair<iterator, bool> insert(P&& p) {
            //TODO(BlackMat): implme
            return std::make_pair(end(), false);
        }

        template < typename P
                 , typename = std::enable_if_t<std::is_constructible<value_type, P>::value> >
        std::pair<iterator, bool> insert(const_iterator hint, P&& p) {
            //TODO(BlackMat): implme
            return std::make_pair(end(), false);
        }

        template < typename InputIter >
        void insert(InputIter first, InputIter last) {
            for ( auto iter = first; iter != last; ++iter ) {
                insert(*iter);
            }
        }

        template < typename... Args >
        std::pair<iterator, bool> emplace(Args&&... args) {
            //TODO(BlackMat): implme
            return insert(value_type(std::forward<Args>(args)...));
        }

        template < typename... Args >
        std::pair<iterator, bool> emplace_hint(const_iterator hint, Args&&... args) {
            //TODO(BlackMat): implme
            return insert(hint, value_type(std::forward<Args>(args)...));
        }
    private:
        data_type data_;
        key_compare compare_;
    };
}
