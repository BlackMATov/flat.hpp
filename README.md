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

[flat.hpp][flat] is a header only library. All you need to do is copy the header files (`flat_set.hpp` and `flat_map.hpp`) into your project and include them:

```cpp
#include "flat_set.hpp" // for flat_set<K>
#include "flat_map.hpp" // for flat_map<K,V>
```

## API

```cpp
template < typename Key
         , typename Compare = std::less<Key>
         , typename Allocator = std::allocator<Key>
         , typename Container = std::vector<Key, Allocator> >
class flat_set;
```

#### Member types

| Member type            | Definition                        |
|------------------------|-----------------------------------|
| key_type               | Key                               |
| value_type             | Key                               |
| size_type              | Container::size_type              |
| difference_type        | Container::difference_type        |
| key_compare            | Compare                           |
| value_compare          | Compare                           |
| allocator_type         | Allocator                         |
| container_type         | Container                         |
| reference              | Container::reference              |
| const_reference        | Container::const_reference        |
| pointer                | Container::pointer                |
| const_pointer          | Container::const_pointer          |
| iterator               | Container::iterator               |
| const_iterator         | Container::const_iterator         |
| reverse_iterator       | Container::reverse_iterator       |
| const_reverse_iterator | Container::const_reverse_iterator |

```cpp
template < typename Key
         , typename Value
         , typename Compare = std::less<Key>
         , typename Allocator = std::allocator<std::pair<Key, Value>>
         , typename Container = std::vector<std::pair<Key, Value>, Allocator> >
class flat_map;
```

| Member type            | Definition                        |
|------------------------|-----------------------------------|
| key_type               | Key                               |
| mapped_type            | Value                             |
| value_type             | Container::value_type             |
| size_type              | Container::size_type              |
| difference_type        | Container::difference_type        |
| key_compare            | Compare                           |
| allocator_type         | Allocator                         |
| container_type         | Container                         |
| reference              | Container::reference              |
| const_reference        | Container::const_reference        |
| pointer                | Container::pointer                |
| const_pointer          | Container::const_pointer          |
| iterator               | Container::iterator               |
| const_iterator         | Container::const_iterator         |
| reverse_iterator       | Container::reverse_iterator       |
| const_reverse_iterator | Container::const_reverse_iterator |

## [License (MIT)](./LICENSE.md)
