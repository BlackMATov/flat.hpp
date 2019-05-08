# flat.hpp

[![travis][badge.travis]][travis]
[![appveyor][badge.appveyor]][appveyor]
[![codecov][badge.codecov]][codecov]
[![language][badge.language]][language]
[![license][badge.license]][license]
[![paypal][badge.paypal]][paypal]

[badge.travis]: https://img.shields.io/travis/BlackMATov/flat.hpp/master.svg?logo=travis
[badge.appveyor]: https://img.shields.io/appveyor/ci/BlackMATov/flat-hpp/master.svg?logo=appveyor
[badge.codecov]: https://img.shields.io/codecov/c/github/BlackMATov/flat.hpp/master.svg?logo=codecov
[badge.language]: https://img.shields.io/badge/language-C%2B%2B14-red.svg
[badge.license]: https://img.shields.io/badge/license-MIT-blue.svg
[badge.paypal]: https://img.shields.io/badge/donate-PayPal-orange.svg?logo=paypal&colorA=00457C

[travis]: https://travis-ci.org/BlackMATov/flat.hpp
[appveyor]: https://ci.appveyor.com/project/BlackMATov/flat-hpp
[codecov]: https://codecov.io/gh/BlackMATov/flat.hpp
[language]: https://en.wikipedia.org/wiki/C%2B%2B14
[license]: https://en.wikipedia.org/wiki/MIT_License
[paypal]: https://www.paypal.me/matov

[flat]: https://github.com/BlackMATov/flat.hpp

## Installation

[flat.hpp][flat] is a header-only library. All you need to do is copy the headers files from `headers` directory into your project and include them:

```cpp
#include "flat_hpp/flat_set.hpp"
using namespace flat_hpp;

int main() {
    flat_set<int> s;
    s.insert(42);
    return 0;
}
```

Also, you can add the root repository directory to your [cmake](https://cmake.org) project:

```cmake
add_subdirectory(external/flat.hpp)
target_link_libraries(your_project_target flat.hpp)
```

## API
- [Flat Set](#flat-set)
- [Flat Map](#flat-map)
- [Flat Multiset](#flat-multiset)
- [Flat Multimap](#flat-multimap)

## Flat Set

```cpp
template < typename Key
         , typename Compare = std::less<Key>
         , typename Container = std::vector<Key> >
class flat_set;
```

### Member types

| Member type              | Definition                          |
|--------------------------|-------------------------------------|
| `key_type`               | `Key`                               |
| `value_type`             | `Key`                               |
| `size_type`              | `Container::size_type`              |
| `difference_type`        | `Container::difference_type`        |
| `key_compare`            | `Compare`                           |
| `value_compare`          | `Compare`                           |
| `container_type`         | `Container`                         |
| `reference`              | `Container::reference`              |
| `const_reference`        | `Container::const_reference`        |
| `pointer`                | `Container::pointer`                |
| `const_pointer`          | `Container::const_pointer`          |
| `iterator`               | `Container::iterator`               |
| `const_iterator`         | `Container::const_iterator`         |
| `reverse_iterator`       | `Container::reverse_iterator`       |
| `const_reverse_iterator` | `Container::const_reverse_iterator` |

### Member functions

```cpp
flat_set();

explicit flat_set(const Compare& c);

template < typename Allocator >
explicit flat_set(const Allocator& a);

template < typename Allocator >
flat_set(const Compare& c, const Allocator& a);

template < typename InputIter >
flat_set(InputIter first, InputIter last);

template < typename InputIter >
flat_set(InputIter first, InputIter last, const Compare& c);

template < typename InputIter, typename Allocator >
flat_set(InputIter first, InputIter last, const Allocator& a);

template < typename InputIter, typename Allocator >
flat_set(InputIter first, InputIter last, const Compare& c, const Allocator& a);

flat_set(std::initializer_list<value_type> ilist);

flat_set(std::initializer_list<value_type> ilist, const Compare& c);

template < typename Allocator >
flat_set(std::initializer_list<value_type> ilist, const Allocator& a);

template < typename Allocator >
flat_set(std::initializer_list<value_type> ilist, const Compare& c, const Allocator& a);

template < typename Allocator >
flat_set(flat_set&& other, const Allocator& a);

template < typename Allocator >
flat_set(const flat_set& other, const Allocator& a);

flat_set(flat_set&& other);
flat_set(const flat_set& other);

flat_set& operator=(flat_set&& other);
flat_set& operator=(const flat_set& other);

flat_set& operator=(std::initializer_list<value_type> ilist);
```

### Iterators

```cpp
iterator begin() noexcept;
const_iterator begin() const noexcept;
const_iterator cbegin() const noexcept;

iterator end() noexcept;
const_iterator end() const noexcept;
const_iterator cend() const noexcept;

reverse_iterator rbegin() noexcept;
const_reverse_iterator rbegin() const noexcept;
const_reverse_iterator crbegin() const noexcept;

reverse_iterator rend() noexcept;
const_reverse_iterator rend() const noexcept;
const_reverse_iterator crend() const noexcept;
```

### Capacity

```cpp
bool empty() const noexcept;
size_type size() const noexcept;
size_type max_size() const noexcept;
size_type capacity() const noexcept;
void reserve(size_type ncapacity);
void shrink_to_fit();
```

### Modifiers

```cpp
std::pair<iterator, bool> insert(value_type&& value);
std::pair<iterator, bool> insert(const value_type& value);

iterator insert(const_iterator hint, value_type&& value);
iterator insert(const_iterator hint, const value_type& value);

template < typename InputIter >
void insert(InputIter first, InputIter last);
void insert(std::initializer_list<value_type> ilist);

template < typename... Args >
std::pair<iterator, bool> emplace(Args&&... args);
template < typename... Args >
iterator emplace_hint(const_iterator hint, Args&&... args);

void clear() noexcept;
iterator erase(const_iterator iter);
iterator erase(const_iterator first, const_iterator last);
size_type erase(const key_type& key);

void swap(flat_set& other);
```

### Lookup

```cpp
size_type count(const key_type& key) const;

iterator find(const key_type& key);
const_iterator find(const key_type& key) const;

std::pair<iterator, iterator> equal_range(const key_type& key);
std::pair<const_iterator, const_iterator> equal_range(const key_type& key) const;

iterator lower_bound(const key_type& key);
const_iterator lower_bound(const key_type& key) const;

iterator upper_bound(const key_type& key);
const_iterator upper_bound(const key_type& key) const;
```

### Observers

```cpp
key_compare key_comp() const;
value_compare value_comp() const;
```

### Non-member functions

```cpp
template < typename Key
         , typename Compare
         , typename Container >
void swap(
    flat_set<Key, Compare, Container>& l,
    flat_set<Key, Compare, Container>& r);

template < typename Key
         , typename Compare
         , typename Container >
bool operator==(
    const flat_set<Key, Compare, Container>& l,
    const flat_set<Key, Compare, Container>& r);

template < typename Key
         , typename Compare
         , typename Container >
bool operator!=(
    const flat_set<Key, Compare, Container>& l,
    const flat_set<Key, Compare, Container>& r);

template < typename Key
         , typename Compare
         , typename Container >
bool operator<(
    const flat_set<Key, Compare, Container>& l,
    const flat_set<Key, Compare, Container>& r);

template < typename Key
         , typename Compare
         , typename Container >
bool operator>(
    const flat_set<Key, Compare, Container>& l,
    const flat_set<Key, Compare, Container>& r);

template < typename Key
         , typename Compare
         , typename Container >
bool operator<=(
    const flat_set<Key, Compare, Container>& l,
    const flat_set<Key, Compare, Container>& r);

template < typename Key
         , typename Compare
         , typename Container >
bool operator>=(
    const flat_set<Key, Compare, Container>& l,
    const flat_set<Key, Compare, Container>& r);
```

## Flat Map

```cpp
template < typename Key
         , typename Value
         , typename Compare = std::less<Key>
         , typename Container = std::vector<std::pair<Key, Value>> >
class flat_map;
```

### Member types

| Member type              | Definition                          |
|--------------------------|-------------------------------------|
| `key_type`               | `Key`                               |
| `mapped_type`            | `Value`                             |
| `value_type`             | `Container::value_type`             |
| `size_type`              | `Container::size_type`              |
| `difference_type`        | `Container::difference_type`        |
| `key_compare`            | `Compare`                           |
| `container_type`         | `Container`                         |
| `reference`              | `Container::reference`              |
| `const_reference`        | `Container::const_reference`        |
| `pointer`                | `Container::pointer`                |
| `const_pointer`          | `Container::const_pointer`          |
| `iterator`               | `Container::iterator`               |
| `const_iterator`         | `Container::const_iterator`         |
| `reverse_iterator`       | `Container::reverse_iterator`       |
| `const_reverse_iterator` | `Container::const_reverse_iterator` |

### Member classes

```cpp
class value_compare;
```

### Member functions

```cpp
flat_map();

explicit flat_map(const Compare& c);

template < typename Allocator >
explicit flat_map(const Allocator& a);

template < typename Allocator >
flat_map(const Compare& c, const Allocator& a);

template < typename InputIter >
flat_map(InputIter first, InputIter last);

template < typename InputIter >
flat_map(InputIter first, InputIter last, const Compare& c);

template < typename InputIter, typename Allocator >
flat_map(InputIter first, InputIter last, const Allocator& a);

template < typename InputIter , typename Allocator >
flat_map(InputIter first, InputIter last, const Compare& c, const Allocator& a);

flat_map(std::initializer_list<value_type> ilist);

flat_map(std::initializer_list<value_type> ilist, const Compare& c);

template < typename Allocator >
flat_map(std::initializer_list<value_type> ilist, const Allocator& a);

template < typename Allocator >
flat_map(std::initializer_list<value_type> ilist, const Compare& c, const Allocator& a);

template < typename Allocator >
flat_map(flat_map&& other, const Allocator& a);

template < typename Allocator >
flat_map(const flat_map& other, const Allocator& a);

flat_map(flat_map&& other);
flat_map(const flat_map& other);

flat_map& operator=(flat_map&& other);
flat_map& operator=(const flat_map& other);

flat_map& operator=(std::initializer_list<value_type> ilist);
```

### Iterators

```cpp
iterator begin() noexcept;
const_iterator begin() const noexcept;
const_iterator cbegin() const noexcept;

iterator end() noexcept;
const_iterator end() const noexcept;
const_iterator cend() const noexcept;

reverse_iterator rbegin() noexcept;
const_reverse_iterator rbegin() const noexcept;
const_reverse_iterator crbegin() const noexcept;

reverse_iterator rend() noexcept;
const_reverse_iterator rend() const noexcept;
const_reverse_iterator crend() const noexcept;
```

### Capacity

```cpp
bool empty() const noexcept;
size_type size() const noexcept;
size_type max_size() const noexcept;
size_type capacity() const noexcept;
void reserve(size_type ncapacity);
void shrink_to_fit();
```

### Element access

```cpp
mapped_type& operator[](key_type&& key);
mapped_type& operator[](const key_type& key);

mapped_type& at(const key_type& key);
const mapped_type& at(const key_type& key) const;
```

### Modifiers

```cpp
std::pair<iterator, bool> insert(value_type&& value);
std::pair<iterator, bool> insert(const value_type& value);

iterator insert(const_iterator hint, value_type&& value);
iterator insert(const_iterator hint, const value_type& value);

template < typename InputIter >
void insert(InputIter first, InputIter last);
void insert(std::initializer_list<value_type> ilist);

template < typename... Args >
std::pair<iterator, bool> emplace(Args&&... args);
template < typename... Args >
iterator emplace_hint(const_iterator hint, Args&&... args);

void clear() noexcept;
iterator erase(const_iterator iter);
iterator erase(const_iterator first, const_iterator last);
size_type erase(const key_type& key);

void swap(flat_map& other)
```

### Lookup

```cpp
size_type count(const key_type& key) const;

iterator find(const key_type& key);
const_iterator find(const key_type& key) const;

std::pair<iterator, iterator> equal_range(const key_type& key);
std::pair<const_iterator, const_iterator> equal_range(const key_type& key) const;

iterator lower_bound(const key_type& key);
const_iterator lower_bound(const key_type& key) const;

iterator upper_bound(const key_type& key);
const_iterator upper_bound(const key_type& key) const;
```

### Observers

```cpp
key_compare key_comp() const;
value_compare value_comp() const;
```

### Non-member functions

```cpp
template < typename Key
         , typename Value
         , typename Compare
         , typename Container >
void swap(
    flat_map<Key, Value, Compare, Container>& l,
    flat_map<Key, Value, Compare, Container>& r);

template < typename Key
         , typename Value
         , typename Compare
         , typename Container >
bool operator==(
    const flat_map<Key, Value, Compare, Container>& l,
    const flat_map<Key, Value, Compare, Container>& r);

template < typename Key
         , typename Value
         , typename Compare
         , typename Container >
bool operator!=(
    const flat_map<Key, Value, Compare, Container>& l,
    const flat_map<Key, Value, Compare, Container>& r);

template < typename Key
         , typename Value
         , typename Compare
         , typename Container >
bool operator<(
    const flat_map<Key, Value, Compare, Container>& l,
    const flat_map<Key, Value, Compare, Container>& r);

template < typename Key
         , typename Value
         , typename Compare
         , typename Container >
bool operator>(
    const flat_map<Key, Value, Compare, Container>& l,
    const flat_map<Key, Value, Compare, Container>& r);

template < typename Key
         , typename Value
         , typename Compare
         , typename Container >
bool operator<=(
    const flat_map<Key, Value, Compare, Container>& l,
    const flat_map<Key, Value, Compare, Container>& r);

template < typename Key
         , typename Value
         , typename Compare
         , typename Container >
bool operator>=(
    const flat_map<Key, Value, Compare, Container>& l,
    const flat_map<Key, Value, Compare, Container>& r);
```

## Flat Multiset

> coming soon!

## Flat Multimap

> coming soon!

## [License (MIT)](./LICENSE.md)
