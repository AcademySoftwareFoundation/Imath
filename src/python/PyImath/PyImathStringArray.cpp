//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#include "PyImathStringArrayRegister.h"
#include "PyImathStringArray.h"
#include "PyImathExport.h"

namespace PyImath {

using namespace boost::python;

template<class T>
StringArrayT<T>* StringArrayT<T>::createDefaultArray(size_t length)
{
    return StringArrayT<T>::createUniformArray(T(), length);
}

template<class T>
StringArrayT<T>* StringArrayT<T>::createUniformArray(const T& initialValue, size_t length)
{
    typedef boost::shared_array<StringTableIndex> StringTableIndexArrayPtr;
    typedef boost::shared_ptr<StringTableT<T> > StringTablePtr;

    BOOST_STATIC_ASSERT(boost::is_pod<StringTableIndex>::value);

    StringTableIndexArrayPtr indexArray(reinterpret_cast<StringTableIndex*>(new char[sizeof(StringTableIndex)*length]));
    StringTablePtr table(new StringTableT<T>);

    const StringTableIndex index = table->intern(initialValue);

    for(size_t i=0; i<length; ++i)
        indexArray[i] = index;

    return new StringArrayT<T>(*table, indexArray.get(), length, 1, indexArray, boost::any(table));
}

template<class T>
StringArrayT<T>* StringArrayT<T>::createFromRawArray(const T* rawArray, size_t length, bool writable)
{
    typedef boost::shared_array<StringTableIndex> StringTableIndexArrayPtr;
    typedef boost::shared_ptr<StringTableT<T> > StringTablePtr;

    BOOST_STATIC_ASSERT(boost::is_pod<StringTableIndex>::value);

    StringTableIndexArrayPtr indexArray(reinterpret_cast<StringTableIndex*>(new char[sizeof(StringTableIndex)*length]));
    StringTablePtr table(new StringTableT<T>);

    for(size_t i=0; i<length; ++i)
        indexArray[i] = table->intern(rawArray[i]);

    return new StringArrayT<T>(*table, indexArray.get(), length, 1, indexArray, table, writable);
}

template<class T>
StringArrayT<T>::StringArrayT(StringTableT<T> &table, StringTableIndex *ptr, size_t length,
                              size_t stride, boost::any tableHandle, bool writable)
    : super(ptr,length,stride,writable), _table(table), _tableHandle(tableHandle)
{
    // nothing
}

template<class T>
StringArrayT<T>::StringArrayT(StringTableT<T> &table, StringTableIndex *ptr, size_t length,
                              size_t stride, boost::any handle, boost::any tableHandle, bool writable)
    : super(ptr,length,stride,handle,writable), _table(table), _tableHandle(tableHandle)
{
    // nothing
}

template<class T>
StringArrayT<T>::StringArrayT(const StringTableT<T> &table, const StringTableIndex *ptr,
                              size_t length, size_t stride, boost::any tableHandle)
    : super(ptr,length,stride), _table(const_cast<StringTableT<T> &>(table)),
                                _tableHandle(tableHandle)
{
    // nothing
}

template<class T>
StringArrayT<T>::StringArrayT(const StringTableT<T> &table, const StringTableIndex *ptr,
                              size_t length, size_t stride, boost::any handle, boost::any tableHandle)
    : super(ptr,length,stride,handle), _table(const_cast<StringTableT<T> &>(table)),
                                       _tableHandle(tableHandle)
{
    // nothing
}

template<class T>
StringArrayT<T>::StringArrayT(StringArrayT& s, const FixedArray<int>& mask)
    : super(s, mask),
      _table(s._table),
      _tableHandle(s._tableHandle)
{
}

template<class T>
StringArrayT<T>*
StringArrayT<T>::getslice_string(PyObject *index) const
{
    typedef boost::shared_array<StringTableIndex> StringTableIndexArrayPtr;
    typedef boost::shared_ptr<StringTableT<T> > StringTablePtr;

    BOOST_STATIC_ASSERT(boost::is_pod<StringTableIndex>::value);

    size_t start=0, end=0, slicelength=0;
    Py_ssize_t step;
    extract_slice_indices(index,start,end,step,slicelength);

    StringTableIndexArrayPtr indexArray(reinterpret_cast<StringTableIndex*>(new char[sizeof(StringTableIndex)*slicelength]));
    StringTablePtr table(new StringTableT<T>);

    for(size_t i=0; i<slicelength; ++i)
        indexArray[i] = table->intern(getitem_string(start+i*step));

    return new StringArrayT<T>(*table, indexArray.get(), slicelength, 1, indexArray, boost::any(table));
}

template<class T>
StringArrayT<T>*
StringArrayT<T>::getslice_mask_string(const FixedArray<int>& mask)
{
    return new StringArrayT(*this, mask);
}

template<class T>
void
StringArrayT<T>::setitem_string_scalar(PyObject *index, const T &data)
{
    if (!writable())
        throw std::invalid_argument("Fixed string-array is read-only.");

    size_t start=0, end=0, slicelength=0;
    Py_ssize_t step;
    extract_slice_indices(index,start,end,step,slicelength);
    StringTableIndex di = _table.intern(data);
    for (size_t i=0; i<slicelength; ++i) {
        (*this)[start+i*step] = di;
    }
}

template<class T>
void
StringArrayT<T>::setitem_string_scalar_mask(const FixedArray<int> &mask, const T &data)
{
    if (!writable())
        throw std::invalid_argument("Fixed string-array is read-only.");

    size_t len = match_dimension(mask);
    StringTableIndex di = _table.intern(data);
    for (size_t i=0; i<len; ++i) {
        if (mask[i]) (*this)[i] = di;
    }
}

template<class T>
void
StringArrayT<T>::setitem_string_vector(PyObject *index, const StringArrayT<T> &data)
{
    if (!writable())
        throw std::invalid_argument("Fixed string-array is read-only.");

    size_t start=0, end=0, slicelength=0;
    Py_ssize_t step;
    extract_slice_indices(index,start,end,step,slicelength);
        
    // we have a valid range of indices
    if ((size_t) data.len() != slicelength) {
        PyErr_SetString(PyExc_IndexError, "Dimensions of source do not match destination");
        throw_error_already_set();
    }
    for (size_t i=0; i<slicelength; ++i) {
        StringTableIndex di = _table.intern(data._table.lookup(data[i]));
        (*this)[start+i*step] = di;
    }
}

template<class T>
void
StringArrayT<T>::setitem_string_vector_mask(const FixedArray<int> &mask, const StringArrayT<T> &data)
{
    if (!writable())
        throw std::invalid_argument("Fixed string-array is read-only.");

    size_t len = match_dimension(mask);
    if ((size_t) data.len() == len) {
        for (size_t i=0; i<len; ++i) {
            if (mask[i]) {
                StringTableIndex di = _table.intern(data._table.lookup(data[i]));
                (*this)[i] = di;
            }
        }
    } else {
        size_t count = 0;
        for (size_t i=0; i<len; ++i) {
            if (mask[i]) count += 1;
        }

        if ((size_t) data.len() != count) {
            PyErr_SetString(PyExc_IndexError, "Dimensions of source data do not match destination either masked or unmasked");
            throw_error_already_set();
        }
            
        size_t dataIndex = 0;
        for (size_t i=0; i<len; ++i) {
            if (mask[i]) {
                StringTableIndex di = _table.intern(data._table.lookup(data[dataIndex]));
                (*this)[i] = di;
                dataIndex += 1;
            }
        }
    }
}

template<class T>
FixedArray<int> operator == (const StringArrayT<T> &a0, const StringArrayT<T> &a1) {
    size_t len = a0.match_dimension(a1);
    FixedArray<int> f(len);
    const StringTableT<T> &t0 = a0.stringTable();
    const StringTableT<T> &t1 = a1.stringTable();
    for (size_t i=0;i<len;++i) {
     f[i] = t0.lookup(a0[i])==t1.lookup(a1[i]); 
    }
    return f;
}

template<class T>
FixedArray<int> operator == (const StringArrayT<T> &a0, const T &v1) {
    size_t len = a0.len();
    FixedArray<int> f(len);
    const StringTableT<T> &t0 = a0.stringTable();
    if (t0.hasString(v1)) {
        StringTableIndex v1i = t0.lookup(v1);
        for (size_t i=0;i<len;++i) {
            f[i] = a0[i]==v1i;
        }
    } else {
        for (size_t i=0;i<len;++i) {
            f[i] = 0;
        }
    }
    return f;
}

template<class T>
FixedArray<int> operator == (const T &v1,const StringArrayT<T> &a0) {
    return a0 == v1;
}

template<class T>
FixedArray<int> operator != (const StringArrayT<T> &a0, const StringArrayT<T> &a1) {
    size_t len = a0.match_dimension(a1);
    FixedArray<int> f(len);
    const StringTableT<T> &t0 = a0.stringTable();
    const StringTableT<T> &t1 = a1.stringTable();
    for (size_t i=0;i<len;++i) {
        f[i] = t0.lookup(a0[i])!=t1.lookup(a1[i]); 
    }
    return f;
}

template<class T>
FixedArray<int> operator != (const StringArrayT<T> &a0, const T &v1) {
    size_t len = a0.len();
    FixedArray<int> f(len);
    const StringTableT<T> &t0 = a0.stringTable();
    if (t0.hasString(v1)) {
        StringTableIndex v1i = t0.lookup(v1);
        for (size_t i=0;i<len;++i) {
            f[i] = a0[i]!=v1i;
        }
    } else {
        for (size_t i=0;i<len;++i) {
            f[i] = 1;
        }
    }
    return f;
}

template<class T>
FixedArray<int> operator != (const T &v1,const StringArrayT<T> &a0) {
    return a0 != v1;
}

template<> PYIMATH_EXPORT StringTableIndex FixedArrayDefaultValue<StringTableIndex>::value() { return StringTableIndex(0); }
template<> PYIMATH_EXPORT const char*      FixedArray<StringTableIndex>::name() { return "StringTableArray"; }

template class PYIMATH_EXPORT StringArrayT<std::string>;
template class PYIMATH_EXPORT StringArrayT<std::wstring>;

template FixedArray<int> operator == (const StringArray& a0, const StringArray& a1);
template FixedArray<int> operator == (const StringArray& a0, const std::string& v1);
template FixedArray<int> operator == (const std::string& a0, const StringArray& v1);
template FixedArray<int> operator != (const StringArray& a0, const StringArray& a1);
template FixedArray<int> operator != (const StringArray& a0, const std::string& v1);
template FixedArray<int> operator != (const std::string& a0, const StringArray& v1);

template FixedArray<int> operator == (const WstringArray& a0, const WstringArray& a1);
template FixedArray<int> operator == (const WstringArray& a0, const std::wstring& v1);
template FixedArray<int> operator == (const std::wstring& a0, const WstringArray& v1);
template FixedArray<int> operator != (const WstringArray& a0, const WstringArray& a1);
template FixedArray<int> operator != (const WstringArray& a0, const std::wstring& v1);
template FixedArray<int> operator != (const std::wstring& a0, const WstringArray& v1);

void register_StringArrays()
{
    typedef StringArrayT<std::string> StringArray;
    typedef StringArrayT<std::wstring> WstringArray;
    
    class_<StringArray> string_array_class =
        class_<StringArray>("StringArray",no_init);
    string_array_class
        .def("__init__", make_constructor(StringArray::createDefaultArray))
        .def("__init__", make_constructor(StringArray::createUniformArray))
        .def("__getitem__", &StringArray::getslice_string, return_value_policy<manage_new_object>()) 
        .def("__getitem__", &StringArray::getitem_string)
        .def("__getitem__", &StringArray::getslice_mask_string, return_value_policy<manage_new_object>())
        .def("__setitem__", &StringArray::setitem_string_scalar)
        .def("__setitem__", &StringArray::setitem_string_scalar_mask)
        .def("__setitem__", &StringArray::setitem_string_vector)
        .def("__setitem__", &StringArray::setitem_string_vector_mask)
        .def("__len__",     &StringArray::len)
        .def("writable",    &StringArray::writable)
        .def("makeReadOnly",&StringArray::makeReadOnly)
        .def(self == self) // NOSONAR - suppress SonarCloud bug report.
        .def(self == other<std::string>())
        .def(other<std::string>() == self)
        .def(self != self) // NOSONAR - suppress SonarCloud bug report.
        .def(self != other<std::string>())
        .def(other<std::string>() != self)
        ;

    class_<WstringArray> wstring_array_class =
        class_<WstringArray>("WstringArray",no_init);
    wstring_array_class
        .def("__init__", make_constructor(WstringArray::createDefaultArray))
        .def("__init__", make_constructor(WstringArray::createUniformArray))
        .def("__getitem__", &WstringArray::getslice_string, return_value_policy<manage_new_object>()) 
        .def("__getitem__", &WstringArray::getitem_string)
        .def("__getitem__", &WstringArray::getslice_mask_string, return_value_policy<manage_new_object>())
        .def("__setitem__", &WstringArray::setitem_string_scalar)
        .def("__setitem__", &WstringArray::setitem_string_scalar_mask)
        .def("__setitem__", &WstringArray::setitem_string_vector)
        .def("__setitem__", &WstringArray::setitem_string_vector_mask)
        .def("__len__",&WstringArray::len)
        .def(self == self) // NOSONAR - suppress SonarCloud bug report.
        .def(self == other<std::wstring>())
        .def(other<std::wstring>() == self)
        .def(self != self) // NOSONAR - suppress SonarCloud bug report.
        .def(self != other<std::wstring>())
        .def(other<std::wstring>() != self)
        ;
}

} // namespace PyImath
