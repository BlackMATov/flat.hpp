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
             , typename Container = std::vector<Key> >
    class flat_multiset final {
    public:
        using key_type = Key;
        using value_type = Key;

        using size_type = typename Container::size_type;
        using difference_type = typename Container::difference_type;

        using key_compare = Compare;
        using value_compare = Compare;
        using container_type = Container;

        using reference = typename Container::reference;
        using const_reference = typename Container::const_reference;
        using pointer = typename Container::pointer;
        using const_pointer = typename Container::const_pointer;

        using iterator = typename Container::const_iterator;
        using const_iterator = typename Container::const_iterator;
        using reverse_iterator = typename Container::const_reverse_iterator;
        using const_reverse_iterator = typename Container::const_reverse_iterator;
    public:
        flat_multiset() {}

        explicit flat_multiset(const Compare& c)
        : compare_(c) {}

        template < typename Allocator >
        explicit flat_multiset(const Allocator& a)
        : data_(a) {}

        template < typename Allocator >
        flat_multiset(const Compare& c, const Allocator& a)
        : data_(a)
        , compare_(c) {}

        template < typename InputIter >
        flat_multiset(InputIter first, InputIter last) {
            insert(first, last);
        }

        template < typename InputIter >
        flat_multiset(InputIter first, InputIter last, const Compare& c)
        : compare_(c) {
            insert(first, last);
        }

        template < typename InputIter, typename Allocator >
        flat_multiset(InputIter first, InputIter last, const Allocator& a)
        : data_(a) {
            insert(first, last);
        }

        template < typename InputIter, typename Allocator >
        flat_multiset(InputIter first, InputIter last, const Compare& c, const Allocator& a)
        : data_(a)
        , compare_(c) {
            insert(first, last);
        }

        flat_multiset(std::initializer_list<value_type> ilist) {
            insert(ilist);
        }

        flat_multiset(std::initializer_list<value_type> ilist, const Compare& c)
        : compare_(c) {
            insert(ilist);
        }

        template < typename Allocator >
        flat_multiset(std::initializer_list<value_type> ilist, const Allocator& a)
        : data_(a) {
            insert(ilist);
        }

        template < typename Allocator >
        flat_multiset(std::initializer_list<value_type> ilist, const Compare& c, const Allocator& a)
        : data_(a)
        , compare_(c) {
            insert(ilist);
        }

        template < typename Allocator >
        flat_multiset(flat_multiset&& other, const Allocator& a)
        : data_(std::move(other.data_), a)
        , compare_(std::move(other.compare_)) {}

        template < typename Allocator >
        flat_multiset(const flat_multiset& other, const Allocator& a)
        : data_(other.data_, a)
        , compare_(other.compare_) {}

        flat_multiset(flat_multiset&& other) = default;
        flat_multiset(const flat_multiset& other) = default;

        flat_multiset& operator=(flat_multiset&& other) = default;
        flat_multiset& operator=(const flat_multiset& other) = default;

        flat_multiset& operator=(std::initializer_list<value_type> ilist) {
            flat_multiset(ilist).swap(*this);
            return *this;
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

        size_type capacity() const noexcept {
            return data_.capacity();
        }

        void reserve(size_type ncapacity) {
            data_.reserve(ncapacity);
        }

        void shrink_to_fit() {
            data_.shrink_to_fit();
        }

        iterator insert(value_type&& value) {
            const iterator iter = upper_bound(value);
            return data_.insert(iter, std::move(value));
        }

        iterator insert(const value_type& value) {
            const iterator iter = upper_bound(value);
            return data_.insert(iter, value);
        }

        iterator insert(const_iterator hint, value_type&& value) {
            return (hint == begin() || !compare_(value, *(hint - 1)))
                && (hint == end() || !compare_(*hint, value))
                ? data_.insert(hint, std::move(value))
                : insert(std::move(value));
        }

        iterator insert(const_iterator hint, const value_type& value) {
            return (hint == begin() || !compare_(value, *(hint - 1)))
                && (hint == end() || !compare_(*hint, value))
                ? data_.insert(hint, value)
                : insert(value);
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
        iterator emplace(Args&&... args) {
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
            const auto p = equal_range(key);
            size_type r = std::distance(p.first, p.second);
            erase(p.first, p.second);
            return r;
        }

        void swap(flat_multiset& other) {
            using std::swap;
            swap(data_, other.data_);
            swap(compare_, other.compare_);
        }

        size_type count(const key_type& key) const {
            const auto p = equal_range(key);
            return std::distance(p.first, p.second);
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
        container_type data_;
        Compare compare_;
    };
}

namespace flat_hpp
{
    template < typename Key
             , typename Compare
             , typename Container >
    void swap(
        flat_multiset<Key, Compare, Container>& l,
        flat_multiset<Key, Compare, Container>& r)
    {
        l.swap(r);
    }

    template < typename Key
             , typename Compare
             , typename Container >
    bool operator==(
        const flat_multiset<Key, Compare, Container>& l,
        const flat_multiset<Key, Compare, Container>& r)
    {
        return l.size() == r.size()
            && std::equal(l.begin(), l.end(), r.begin());
    }

    template < typename Key
             , typename Compare
             , typename Container >
    bool operator!=(
        const flat_multiset<Key, Compare, Container>& l,
        const flat_multiset<Key, Compare, Container>& r)
    {
        return !(l == r);
    }

    template < typename Key
             , typename Compare
             , typename Container >
    bool operator<(
        const flat_multiset<Key, Compare, Container>& l,
        const flat_multiset<Key, Compare, Container>& r)
    {
        return std::lexicographical_compare(l.begin(), l.end(), r.begin(), r.end());
    }

    template < typename Key
             , typename Compare
             , typename Container >
    bool operator>(
        const flat_multiset<Key, Compare, Container>& l,
        const flat_multiset<Key, Compare, Container>& r)
    {
        return r < l;
    }

    template < typename Key
             , typename Compare
             , typename Container >
    bool operator<=(
        const flat_multiset<Key, Compare, Container>& l,
        const flat_multiset<Key, Compare, Container>& r)
    {
        return !(r < l);
    }

    template < typename Key
             , typename Compare
             , typename Container >
    bool operator>=(
        const flat_multiset<Key, Compare, Container>& l,
        const flat_multiset<Key, Compare, Container>& r)
    {
        return !(l < r);
    }
}