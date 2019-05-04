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
             , typename Value
             , typename Compare = std::less<Key>
             , typename Allocator = std::allocator<std::pair<Key, Value>> >
    class flat_map final {
        using data_type = std::vector<std::pair<Key, Value>, Allocator>;
    public:
        using key_type = Key;
        using mapped_type = Value;
        using value_type = typename data_type::value_type;

        using size_type = typename data_type::size_type;
        using difference_type = typename data_type::difference_type;

        using key_compare = Compare;
        using allocator_type = Allocator;

        using reference = typename data_type::reference;
        using const_reference = typename data_type::const_reference;
        using pointer = typename data_type::pointer;
        using const_pointer = typename data_type::const_pointer;

        using iterator = typename data_type::iterator;
        using const_iterator = typename data_type::const_iterator;
        using reverse_iterator = typename data_type::reverse_iterator;
        using const_reverse_iterator = typename data_type::const_reverse_iterator;

        class value_compare final : public std::binary_function<value_type, value_type, bool> {
        public:
            bool operator()(const value_type& l, const value_type& r) const {
                return compare_(l.first, r.first);
            }
        private:
            friend class flat_map;
            explicit value_compare(const key_compare& compare)
            : compare_(compare) {}
        private:
            key_compare compare_;
        };

        static_assert(
            std::is_same<typename allocator_type::value_type, value_type>::value,
            "Allocator::value_type must be same type as value_type");
    public:
        explicit flat_map(
            const Allocator& a)
        : data_(a) {}

        explicit flat_map(
            const Compare& c = Compare(),
            const Allocator& a = Allocator())
        : data_(a)
        , compare_(c) {}

        template < typename InputIter >
        flat_map(
            InputIter first,
            InputIter last,
            const Allocator& a)
        : data_(a) {
            insert(first, last);
        }

        template < typename InputIter >
        flat_map(
            InputIter first,
            InputIter last,
            const Compare& c = Compare(),
            const Allocator& a = Allocator())
        : data_(a)
        , compare_(c) {
            insert(first, last);
        }

        flat_map(
            std::initializer_list<value_type> il,
            const Allocator& a)
        : data_(a) {
            insert(il.begin(), il.end());
        }

        flat_map(
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

        bool empty() const noexcept {
            return data_.empty();
        }

        size_type size() const noexcept {
            return data_.size();
        }

        size_type max_size() const noexcept {
            return data_.max_size();
        }

        mapped_type& operator[](key_type&& key) {
            return insert(value_type(std::move(key), mapped_type())).first->second;
        }

        mapped_type& operator[](const key_type& key) {
            return insert(value_type(key, mapped_type())).first->second;
        }

        mapped_type& at(const key_type& key) {
            const auto iter = find(key);
            if ( iter != end() ) {
                return iter->second;
            }
            throw std::out_of_range("flat_map::at: key not found");
        }

        const mapped_type& at(const key_type& key) const {
            const auto iter = find(key);
            if ( iter != end() ) {
                return iter->second;
            }
            throw std::out_of_range("flat_map::at: key not found");
        }

        std::pair<iterator, bool> insert(const value_type& value) {
            bool found = true;
            iterator iter = lower_bound(value.first);
            if ( iter == end() || compare_(value.first, iter->first) ) {
                iter = data_.insert(iter, value);
                found = false;
            }
            return std::make_pair(iter, !found);
        }

        std::pair<iterator, bool> insert(const_iterator hint, const value_type& value) {
            //TODO(BlackMat): implme
            return insert(value);
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

        void clear() noexcept {
            data_.clear();
        }

        iterator erase(const_iterator iter) {
            //TODO(BlackMat): implme
            return end();
        }

        iterator erase(const_iterator first, const_iterator last) {
            //TODO(BlackMat): implme
            return end();
        }

        iterator erase(const key_type& key) {
            //TODO(BlackMat): implme
            return end();
        }

        void swap(flat_map& other) {
            //TODO(BlackMat): implme
        }

        size_type count(const key_type& key) const {
            const auto iter = find(key);
            return iter != end() ? 1 : 0;
        }

        iterator find(const key_type& key) {
            iterator iter = lower_bound(key);
            if ( iter != end() && compare_(key, iter->first) ) {
                iter = end();
            }
            return iter;
        }

        const_iterator find(const key_type& key) const {
            const_iterator iter = lower_bound(key);
            if ( iter != end() && compare_(key, iter->first) ) {
                iter = end();
            }
            return iter;
        }

        std::pair<iterator, iterator> equal_range(const key_type& key) {
            //TODO(BlackMat): implme
            return {end(), end()};
        }

        std::pair<const_iterator, const_iterator> equal_range(const key_type& key) const {
            //TODO(BlackMat): implme
            return {end(), end()};
        }

        iterator lower_bound(const key_type& key) {
            //TODO(BlackMat): implme
            return std::lower_bound(begin(), end(), key, [this](const value_type& l, const key_type& r){
                return compare_(l.first, r);
            });
        }

        const_iterator lower_bound(const key_type& key) const {
            //TODO(BlackMat): implme
            return std::lower_bound(begin(), end(), key, [this](const value_type& l, const key_type& r){
                return compare_(l.first, r);
            });
        }

        iterator upper_bound(const key_type& key) {
            //TODO(BlackMat): implme
            return std::upper_bound(begin(), end(), key, [this](const key_type& l, const value_type& r){
                return compare_(l, r.first);
            });
        }

        const_iterator upper_bound(const key_type& key) const {
            //TODO(BlackMat): implme
            return std::upper_bound(begin(), end(), key, [this](const key_type& l, const value_type& r){
                return compare_(l, r.first);
            });
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
    template < typename K, typename V, typename C, typename A >
    void swap(flat_map<K, V, C, A>& l, flat_map<K, V, C, A>& r) {
        l.swap(r);
    }

    template < typename K, typename V, typename C, typename A >
    bool operator==(const flat_map<K, V, C, A>& l, const flat_map<K, V, C, A>& r) {
        return l.size() == r.size()
            && std::equal(l.begin(), l.end(), r.begin(), r.end());
    }

    template < typename K, typename V, typename C, typename A >
    bool operator!=(const flat_map<K, V, C, A>& l, const flat_map<K, V, C, A>& r) {
        return !(l == r);
    }

    template < typename K, typename V, typename C, typename A >
    bool operator<(const flat_map<K, V, C, A>& l, const flat_map<K, V, C, A>& r) {
        return std::lexicographical_compare(l.begin(), l.end(), r.begin(), r.end());
    }

    template < typename K, typename V, typename C, typename A >
    bool operator>(const flat_map<K, V, C, A>& l, const flat_map<K, V, C, A>& r) {
        return r < l;
    }

    template < typename K, typename V, typename C, typename A >
    bool operator<=(const flat_map<K, V, C, A>& l, const flat_map<K, V, C, A>& r) {
        return !(r < l);
    }

    template < typename K, typename V, typename C, typename A >
    bool operator>=(const flat_map<K, V, C, A>& l, const flat_map<K, V, C, A>& r) {
        return !(l < r);
    }
}
