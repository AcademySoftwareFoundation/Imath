//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#include <limits>
#include "PyImathExport.h"
#include "PyImathStringTable.h"

namespace PyImath {

template<class T>
StringTableIndex
StringTableT<T>::lookup(const T &s) const
{
    typedef typename Table::template nth_index<1>::type StringSet;
    const StringSet &strings = _table.template get<1>();

    typename StringSet::const_iterator it = strings.find(s);
    if (it == strings.end()) {
      throw std::domain_error ("String table access out of bounds");
    }

    return it->i;
}

template<class T>
const T &
StringTableT<T>::lookup(StringTableIndex index) const
{
    typedef typename Table::template nth_index<0>::type IndexSet;
    const IndexSet &indices = _table.template get<0>();

    typename IndexSet::const_iterator it = indices.find(index);
    if (it == indices.end()) {
      throw std::domain_error ("String table access out of bounds");
    }

    return it->s;
}

template<class T>
StringTableIndex
StringTableT<T>::intern(const T &s)
{
    typedef typename Table::template nth_index<1>::type StringSet;
    const StringSet &strings = _table.template get<1>();

    typename StringSet::const_iterator it = strings.find(s);
    if (it == strings.end()) {
        size_t next_index = _table.size();
        if (next_index > std::numeric_limits<StringTableIndex::index_type>::max()) {
          throw std::domain_error ("Unable to intern string - string table would exceed maximum size");
        }
        StringTableIndex index = StringTableIndex(StringTableIndex::index_type(next_index));
        _table.insert(StringTableEntry<T>(index,s));
        return index;
    }

    return it->i;
}

template<class T>
size_t
StringTableT<T>::size() const
{
    return _table.size();
}

template<class T>
bool
StringTableT<T>::hasString(const T &s) const
{
    typedef typename Table::template nth_index<1>::type StringSet;
    const StringSet &strings = _table.template get<1>();
    return strings.find(s) != strings.end();
}

template<class T>
bool
StringTableT<T>::hasStringIndex(const StringTableIndex &s) const
{
    typedef typename Table::template nth_index<0>::type IndexSet;
    const IndexSet &indices = _table.template get<0>();
    return indices.find(s) != indices.end();
}

namespace {
template class PYIMATH_EXPORT StringTableDetailT<std::string>;
template class PYIMATH_EXPORT StringTableDetailT<std::wstring>;
}

template class PYIMATH_EXPORT StringTableT<std::string>;
template class PYIMATH_EXPORT StringTableT<std::wstring>;

} // namespace PyImath
