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
            std::initializer_list<value_type> ilist,
            const Allocator& a)
        : data_(a) {
            insert(ilist);
        }

        flat_set(
            std::initializer_list<value_type> ilist,
            const Compare& c = Compare(),
            const Allocator& a = Allocator())
        : data_(a)
        , compare_(c) {
            insert(ilist);
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

        bool empty() const noexcept {
            return data_.empty();
        }

        size_type size() const noexcept {
            return data_.size();
        }

        size_type max_size() const noexcept {
            return data_.max_size();
        }

        std::pair<iterator, bool> insert(value_type&& value) {
            const iterator iter = lower_bound(value);
            return iter == end() || compare_(value, *iter)
                ? std::make_pair(data_.insert(iter, std::move(value)), true)
                : std::make_pair(iter, false);
        }

        std::pair<iterator, bool> insert(const value_type& value) {
            const iterator iter = lower_bound(value);
            return iter == end() || compare_(value, *iter)
                ? std::make_pair(data_.insert(iter, value), true)
                : std::make_pair(iter, false);
        }

        iterator insert(const_iterator hint, value_type&& value) {
            return (hint == begin() || compare_(*(hint - 1), value))
                && (hint == end() || compare_(value, *hint))
                ? data_.insert(hint, std::move(value))
                : insert(std::move(value)).first;
        }

        iterator insert(const_iterator hint, const value_type& value) {
            return (hint == begin() || compare_(*(hint - 1), value))
                && (hint == end() || compare_(value, *hint))
                ? data_.insert(hint, value)
                : insert(value).first;
        }

        template < typename InputIter >
        void insert(InputIter first, InputIter last) {
            while ( first != last ) {
                insert(*first++);
            }
        }

        void insert(std::initializer_list<value_type> ilist) {
            insert(ilist.begin(), ilist.end());
        }

        template < typename... Args >
        std::pair<iterator, bool> emplace(Args&&... args) {
            return insert(value_type(std::forward<Args>(args)...));
        }

        template < typename... Args >
        iterator emplace_hint(const_iterator hint, Args&&... args) {
            return insert(hint, value_type(std::forward<Args>(args)...));
        }

        void clear() noexcept {
            data_.clear();
        }

        iterator erase(const_iterator iter) {
            return data_.erase(iter);
        }

        iterator erase(const_iterator first, const_iterator last) {
            return data_.erase(first, last);
        }

        size_type erase(const key_type& key) {
            const iterator iter = find(key);
            return iter != end()
                ? (erase(iter), 1)
                : 0;
        }

        void swap(flat_set& other) {
            using std::swap;
            swap(data_, other.data_);
            swap(compare_, other.compare_);
        }

        size_type count(const key_type& key) const {
            const const_iterator iter = find(key);
            return iter != end() ? 1 : 0;
        }

        iterator find(const key_type& key) {
            const iterator iter = lower_bound(key);
            return iter != end() && !compare_(key, *iter)
                ? iter
                : end();
        }

        const_iterator find(const key_type& key) const {
            const const_iterator iter = lower_bound(key);
            return iter != end() && !compare_(key, *iter)
                ? iter
                : end();
        }

        std::pair<iterator, iterator> equal_range(const key_type& key) {
            return std::equal_range(begin(), end(), key, compare_);
        }

        std::pair<const_iterator, const_iterator> equal_range(const key_type& key) const {
            return std::equal_range(begin(), end(), key, compare_);
        }

        iterator lower_bound(const key_type& key) {
            return std::lower_bound(begin(), end(), key, compare_);
        }

        const_iterator lower_bound(const key_type& key) const {
            return std::lower_bound(begin(), end(), key, compare_);
        }

        iterator upper_bound(const key_type& key) {
            return std::upper_bound(begin(), end(), key, compare_);
        }

        const_iterator upper_bound(const key_type& key) const {
            return std::upper_bound(begin(), end(), key, compare_);
        }

        key_compare key_comp() const {
            return compare_;
        }

        value_compare value_comp() const {
            return value_compare(compare_);
        }
    private:
        data_type data_;
        key_compare compare_;
    };
}

namespace flat_hpp
{
    template < typename K, typename C, typename A >
    void swap(flat_set<K, C, A>& l, flat_set<K, C, A>& r) {
        l.swap(r);
    }

    template < typename K, typename C, typename A >
    bool operator==(const flat_set<K, C, A>& l, const flat_set<K, C, A>& r) {
        return l.size() == r.size()
            && std::equal(l.begin(), l.end(), r.begin(), r.end());
    }

    template < typename K, typename C, typename A >
    bool operator!=(const flat_set<K, C, A>& l, const flat_set<K, C, A>& r) {
        return !(l == r);
    }

    template < typename K, typename C, typename A >
    bool operator<(const flat_set<K, C, A>& l, const flat_set<K, C, A>& r) {
        return std::lexicographical_compare(l.begin(), l.end(), r.begin(), r.end());
    }

    template < typename K, typename C, typename A >
    bool operator>(const flat_set<K, C, A>& l, const flat_set<K, C, A>& r) {
        return r < l;
    }

    template < typename K, typename C, typename A >
    bool operator<=(const flat_set<K, C, A>& l, const flat_set<K, C, A>& r) {
        return !(r < l);
    }

    template < typename K, typename C, typename A >
    bool operator>=(const flat_set<K, C, A>& l, const flat_set<K, C, A>& r) {
        return !(l < r);
    }
}
