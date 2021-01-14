//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#ifndef _PyImathFixedVArray_h_
#define _PyImathFixedVArray_h_

#include <boost/python.hpp>
#include <boost/any.hpp>
#include <vector>
#include "PyImathFixedArray.h"

namespace PyImath {

template <class T>
class FixedVArray
{
    // This class (at least for now) holds a std::vector of 'T' types.
    // This will give us the 'variable' part of the array.  Generally,
    // we will initially support only a very small subset of accessor
    // methods before the semantics are fully defined.  Currently, the
    // VArray semantics are defined in the 'varraySemantics.txt' file.

    std::vector<T> *  _ptr;
    size_t            _length;
    size_t            _stride;

    // This handle optionally stores a shared_array to allocated array data
    // so that everything is freed properly on exit.
    boost::any        _handle;

    boost::shared_array<size_t>  _indices;  // non-NULL if we're a masked reference
    size_t                       _unmaskedLength;

  public:
    typedef T  BaseType;

    FixedVArray (std::vector<T>* ptr, Py_ssize_t length,
                 Py_ssize_t stride = 1);

    FixedVArray (std::vector<T>* ptr, Py_ssize_t length,
                 Py_ssize_t stride, boost::any handle);

    explicit FixedVArray (Py_ssize_t length);

 // Not needed.  vector-lengths are zero (uninitialized) by default.
 // FixedVArray (Py_ssize_t length, Uninitialized);

    FixedVArray (const T& initialValue, Py_ssize_t length);

    FixedVArray (FixedVArray<T>& f, const FixedArray<int>& mask);

 // template <class S>
 // explicit FixedVArray (const FixedVArray<S> &other);

    FixedVArray (const FixedVArray<T>& other);

    const FixedVArray&  operator = (const FixedVArray<T>& other);

   ~FixedVArray();

    // ----------------

    const boost::any&  handle() { return _handle; }

    Py_ssize_t  len()    const { return _length; }
    size_t      stride() const { return _stride; }

    bool        isMaskedReference() const { return _indices.get() != 0; }
    size_t      unmaskedLength()    const { return _unmaskedLength; }

    std::vector<T>&        operator [] (size_t i);
    const std::vector<T>&  operator [] (size_t i) const;

    // ----------------

 // typedef typename boost::mpl::if_<boost::is_class<T>,T&,T>::type get_type;
 // get_type        getitem (Py_ssize_t index);
 // 
 // typedef typename boost::mpl::if_<boost::is_class<T>,const T&,T>::type get_type_const;
 // get_type_const  getitem (Py_ssize_t index) const;

    // ----------------

    FixedVArray<T>  getslice (PyObject* index) const;  // AAJ: Question about 'FixedArray' version
    FixedVArray<T>  getslice_mask (const FixedArray<int>& mask);

 // void            setitem_scalar (PyObject* index, const T& data);
 // void            setitem_scalar_mask (const FixedArray<int>& mask, const T& data);
    void            setitem_vector (PyObject* index, const FixedVArray<T>& data);
    void            setitem_vector_mask (const FixedArray<int>& mask, const FixedVArray<T>& data);

 // FixedVArray<T>  ifelse_scalar(const FixedArray<int>& choice, const T& other);
    FixedVArray<T>  ifelse_vector(const FixedArray<int>& choice, const FixedVArray<T>& other);

    // ----------------

    static boost::python::class_<FixedVArray<T> > register_(const char* doc);

    // Instantiations of fixed variable arrays must implement this static member.
    static const char* name();

    template <class T2>
    size_t  match_dimension (const FixedArray<T2>& mask,
                             bool strictComparison = true) const
    {
        if (len() == mask.len())
        {
            return len();
        }

        bool throwExc = false;
        if (strictComparison)
        {
            throwExc = true;
        }
        else if (_indices)
        {
            if (_unmaskedLength != (size_t) mask.len())
            {
                throwExc = true;
            }
        }
        else
        {
            throwExc = true;
        }

        if (throwExc)
        {
          throw std::invalid_argument("Dimensions of source do not match destination");
        }

        return len();
    }

    size_t  match_dimension (const FixedVArray<T>& other,
                             bool strictComparison = true) const
    {
        if (len() == other.len())
        {
            return len();
        }

        bool throwExc = false;
        if (strictComparison)
        {
            throwExc = true;
        }
        else if (_indices)
        {
            if (_unmaskedLength != (size_t) other.len())
            {
                throwExc = true;
            }
        }
        else
        {
            throwExc = true;
        }

        if (throwExc)
        {
          throw std::invalid_argument("Dimensions of source do not match destination");
        }

        return len();
    }

  protected:
    size_t  raw_ptr_index (size_t i) const;

};

} // namespace PyImath

#endif // _PyImathFixedVArray_h_
