//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#ifndef _PyImathStringArray_h_
#define _PyImathStringArray_h_

#include "PyImathFixedArray.h"
#include "PyImathStringTable.h"

//
// A fixed lengy array class for string and wide string type in python.
// The implementation of StringArray is does not follow other FixeArray
// types. StringArray de-duplicate repeated strings using StringTable
// internally for compact memory usage.
//
namespace PyImath {

template <class T>
class StringArrayT : public FixedArray<StringTableIndex>
{
  public:
    typedef T   BaseType;
    typedef FixedArray<StringTableIndex> super;

    static StringArrayT<T>* createDefaultArray(size_t length);
    static StringArrayT<T>* createUniformArray(const T& initialValue, size_t length);
    static StringArrayT<T>* createFromRawArray(const T* rawArray, size_t length,
                                               bool writable = true);

    StringArrayT(StringTableT<T> &table, StringTableIndex *ptr, size_t length,
                 size_t stride = 1, boost::any tableHandle = boost::any(),
                 bool writable = true);

    StringArrayT(StringTableT<T> &table, StringTableIndex *ptr, size_t length,
                 size_t stride, boost::any handle, boost::any tableHandle = boost::any(),
                 bool writable = true);

    StringArrayT(const StringTableT<T> &table, const StringTableIndex *ptr, size_t length,
                 size_t stride = 1, boost::any tableHandle = boost::any());

    StringArrayT(const StringTableT<T> &table, const StringTableIndex *ptr, size_t length,
                 size_t stride, boost::any handle, boost::any tableHandle = boost::any());

    StringArrayT(StringArrayT& s, const FixedArray<int>& mask);

    const StringTableT<T> & stringTable() const { return _table; }

    T  getitem_string(Py_ssize_t index) const {return _table.lookup(getitem(index)); }
    StringArrayT* getslice_string(PyObject *index) const;
    StringArrayT* getslice_mask_string(const FixedArray<int>& mask);

    void setitem_string_scalar(PyObject *index, const T &data);

    void setitem_string_scalar_mask(const FixedArray<int> &mask, const T &data);
    void setitem_string_vector(PyObject *index, const StringArrayT<T> &data);
    void setitem_string_vector_mask(const FixedArray<int> &mask, const StringArrayT<T> &data);

  private:
    typedef StringArrayT<T>     this_type;

    StringTableT<T>             &_table;
    // StringArray can borrow a string table from somewhere else or maintain 
    // its own string table. This handle optionally stores a shared pointer to 
    // a allocated StringTable class instance
    boost::any                  _tableHandle;
};

template<class T>
FixedArray<int> operator == (const StringArrayT<T> &a0, const StringArrayT<T> &a1); 
template<class T>
FixedArray<int> operator == (const StringArrayT<T> &a0, const T &v1);
template<class T>
FixedArray<int> operator == (const T &v1,const StringArrayT<T> &a0);
template<class T>
FixedArray<int> operator != (const StringArrayT<T> &a0, const StringArrayT<T> &a1);
template<class T>
FixedArray<int> operator != (const StringArrayT<T> &a0, const T &v1);
template<class T>
FixedArray<int> operator != (const T &v1,const StringArrayT<T> &a0);

typedef StringArrayT<std::string> StringArray;
typedef StringArrayT<std::wstring> WstringArray;

} // namespace PyImath

#endif
