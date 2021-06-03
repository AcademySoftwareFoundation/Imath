//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#ifndef _PyImathFixedArray_h_
#define _PyImathFixedArray_h_

#include <boost/python.hpp>
#include <boost/operators.hpp>
#include <boost/shared_array.hpp>
#include <boost/any.hpp>
#include <iostream>
#include "PyImathUtil.h"

//
// Note: when PyImath from the v2 release of OpenEXR depended on Iex,
// the PY_IMATH_LEAVE/RETURN_PYTHON macros bracketed calls that
// enabled/disabled float-point exceptions via via the MathExcOn
// class. This was a compile-time option based on the setting of
// PYIMATH_ENABLE_EXCEPTIONS. This behavior is now deprecated, hence
// the empty macros.
//

#define PY_IMATH_LEAVE_PYTHON PyImath::PyReleaseLock pyunlock;
#define PY_IMATH_RETURN_PYTHON

namespace PyImath {

namespace {

//
// Utility classes used for converting array members to boost python objects.
//

template <class T>
struct ReturnReference
{
    static boost::python::object  applyReadOnly (const T& val)
    {
        typename boost::python::copy_const_reference::apply<const T&>::type converter;
        return boost::python::object(boost::python::handle<>(converter(val)));
    }

    static boost::python::object  applyWritable (T& val)
    {
        typename boost::python::reference_existing_object::apply<T&>::type converter;
        return boost::python::object(boost::python::handle<>(converter(val)));
    }

    static bool  isReferenceWrap () { return true; }
};

template <class T>
struct ReturnByValue
{
    static boost::python::object  applyReadOnly (const T& val)
    {
        typename boost::python::return_by_value::apply<T>::type converter;
        return boost::python::object(boost::python::handle<>(converter(val)));
    }

    static boost::python::object  applyWritable (T& val)
    {
        return applyReadOnly (val);
    }

    static bool  isReferenceWrap () { return false; }
};

} // namespace

//
// Utility class for a runtime-specified fixed length array type in python
//
template <class T>
struct FixedArrayDefaultValue
{
    static T value();
};

enum Uninitialized {UNINITIALIZED};

template <class T>
class FixedArray
{
    T *     _ptr;
    size_t  _length;
    size_t  _stride;
    bool    _writable;

    // this handle optionally stores a shared_array to allocated array data
    // so that everything is freed properly on exit.
    boost::any _handle;

    boost::shared_array<size_t> _indices; // non-NULL iff I'm a masked reference
    size_t                      _unmaskedLength;


  public:
    typedef T   BaseType;

    FixedArray(T *ptr, Py_ssize_t length, Py_ssize_t stride = 1, bool writable = true)
        : _ptr(ptr), _length(length), _stride(stride), _writable(writable),
          _handle(), _unmaskedLength(0)
    {
        if (length < 0)
        {
          throw std::domain_error  ("Fixed array length must be non-negative");
        }
        if (stride <= 0)
        {
          throw std::domain_error  ("Fixed array stride must be positive");
        }
        // nothing
    }

    FixedArray(T *ptr, Py_ssize_t length, Py_ssize_t stride,
               boost::any handle, bool writable = true)
        : _ptr(ptr), _length(length), _stride(stride), _writable(writable),
          _handle(handle), _unmaskedLength(0)
    {
        if (_length < 0)
        {
            throw std::domain_error("Fixed array length must be non-negative");
        }
        if (stride <= 0)
        {
            throw std::domain_error("Fixed array stride must be positive");
        }
        // nothing
    }

    FixedArray(const T *ptr, Py_ssize_t length, Py_ssize_t stride = 1)
        : _ptr(const_cast<T *>(ptr)), _length(length), _stride(stride),
          _writable(false), _handle(), _unmaskedLength(0)
    {
        if (length < 0)
        {
            throw std::logic_error("Fixed array length must be non-negative");
        }
        if (stride <= 0)
        {
            throw std::logic_error("Fixed array stride must be positive");
        }
        // nothing
    }

    FixedArray(const T *ptr, Py_ssize_t length, Py_ssize_t stride, boost::any handle)
        : _ptr(const_cast<T *>(ptr)), _length(length), _stride(stride), _writable(false),
          _handle(handle), _unmaskedLength(0)
    {
        if (_length < 0)
        {
            throw std::logic_error("Fixed array length must be non-negative");
        }
        if (stride <= 0)
        {
            throw std::logic_error("Fixed array stride must be positive");
        }
        // nothing
    }

    explicit FixedArray(Py_ssize_t length)
        : _ptr(0), _length(length), _stride(1), _writable(true),
          _handle(), _unmaskedLength(0)
    {
        if (_length < 0) {
            throw std::domain_error("Fixed array length must be non-negative");
        }
        boost::shared_array<T> a(new T[length]);
        T tmp = FixedArrayDefaultValue<T>::value();
        for (Py_ssize_t i=0; i<length; ++i) a[i] = tmp;
        _handle = a;
        _ptr = a.get();
    }

    FixedArray(Py_ssize_t length,Uninitialized)
        : _ptr(0), _length(length), _stride(1), _writable(true),
          _handle(), _unmaskedLength(0)
    {
        if (_length < 0) {
            throw std::domain_error("Fixed array length must be non-negative");
        }
        boost::shared_array<T> a(new T[length]);
        _handle = a;
        _ptr = a.get();
    }

    FixedArray(const T &initialValue, Py_ssize_t length)
        : _ptr(0), _length(length), _stride(1), _writable(true),
          _handle(), _unmaskedLength(0)
    {
        if (_length < 0) {
            throw std::domain_error("Fixed array length must be non-negative");
        }
        boost::shared_array<T> a(new T[length]);
        for (Py_ssize_t i=0; i<length; ++i) a[i] = initialValue;
        _handle = a;
        _ptr = a.get();
    }

    template <typename MaskArrayType>
    FixedArray(FixedArray& f, const MaskArrayType& mask) 
        : _ptr(f._ptr), _stride(f._stride), _writable(f._writable), _handle(f._handle), _unmaskedLength(0)
    {
        if (f.isMaskedReference())
        {
            throw std::invalid_argument("Masking an already-masked FixedArray not supported yet (SQ27000)");
        }

        size_t len = f.match_dimension(mask);
        _unmaskedLength = len;

        size_t reduced_len = 0;
        for (size_t i = 0; i < len; ++i)
            if (mask[i])
                reduced_len++;

        _indices.reset(new size_t[reduced_len]);

        for (size_t i = 0, j = 0; i < len; ++i)
        {
            if (mask[i])
            {
                _indices[j] = i;
                j++;
            }
        }

        _length = reduced_len;
    }

    template <typename MaskArrayType>
    FixedArray(const FixedArray& f, const MaskArrayType& mask) 
        : _ptr(f._ptr), _stride(f._stride), _writable(false), _handle(f._handle), _unmaskedLength(0)
    {
        if (f.isMaskedReference())
        {
            throw std::invalid_argument("Masking an already-masked FixedArray not supported yet (SQ27000)");
        }

        size_t len = f.match_dimension(mask);
        _unmaskedLength = len;

        size_t reduced_len = 0;
        for (size_t i = 0; i < len; ++i)
            if (mask[i])
                reduced_len++;

        _indices.reset(new size_t[reduced_len]);

        for (size_t i = 0, j = 0; i < len; ++i)
        {
            if (mask[i])
            {
                _indices[j] = i;
                j++;
            }
        }

        _length = reduced_len;
    }

    template <class S>
    explicit FixedArray(const FixedArray<S> &other)
        : _ptr(0), _length(other.len()), _stride(1), _writable(true),
          _handle(), _unmaskedLength(other.unmaskedLength())
    {
        boost::shared_array<T> a(new T[_length]);
        for (size_t i=0; i<_length; ++i) a[i] = T(other[i]);
        _handle = a;
        _ptr = a.get();

        if (_unmaskedLength)
        {
            _indices.reset(new size_t[_length]);

            for (size_t i = 0; i < _length; ++i)
                _indices[i] = other.raw_ptr_index(i);
        }
    }

    FixedArray(const FixedArray &other)
        : _ptr(other._ptr), _length(other._length), _stride(other._stride),
          _writable(other._writable),
          _handle(other._handle),
          _indices(other._indices),
          _unmaskedLength(other._unmaskedLength)
    {
    }
        
    const FixedArray &
    operator = (const FixedArray &other)
    {
        if (&other == this) return *this;

        _ptr = other._ptr;
        _length = other._length;
        _stride = other._stride;
        _writable = other._writable;
        _handle = other._handle;
        _unmaskedLength = other._unmaskedLength;
        _indices = other._indices;

        return *this;
    }

    ~FixedArray()
    {
        // nothing
    }

    explicit operator bool() const {return _ptr != nullptr;}

    const boost::any & handle() { return _handle; }

    //
    // Make an index suitable for indexing into an array in c++ from
    // a python index, which can be negative for indexing relative to
    // the end of an array
    //
    size_t canonical_index(Py_ssize_t index) const
    {
        if (index < 0) index += len();
        if (index >= len() || index < 0) {
            PyErr_SetString(PyExc_IndexError, "Index out of range");
            boost::python::throw_error_already_set();
        }
        return index; // still a virtual index if this is a masked reference array
    }

    void extract_slice_indices(PyObject *index, size_t &start, size_t &end, Py_ssize_t &step, size_t &slicelength) const
    {
        if (PySlice_Check(index)) {
#if PY_MAJOR_VERSION > 2
            PyObject *slice = index;
#else
            PySliceObject *slice = reinterpret_cast<PySliceObject *>(index);
#endif
            Py_ssize_t s,e,sl;
            if (PySlice_GetIndicesEx(slice,_length,&s,&e,&step,&sl) == -1) {
                boost::python::throw_error_already_set();
            }
            // e can be -1 if the iteration is backwards with a negative slice operator [::-n] (n > 0).
            if (s < 0 || e < -1 || sl < 0) {
                throw std::domain_error("Slice extraction produced invalid start, end, or length indices");
            }
            start = s;
            end = e;
            slicelength = sl;
        } else if (PyInt_Check(index)) {
            size_t i = canonical_index(PyInt_AsSsize_t(index));
            start = i; end = i+1; step = 1; slicelength = 1;
        } else {
            PyErr_SetString(PyExc_TypeError, "Object is not a slice");
	    boost::python::throw_error_already_set();
        }
    }

    // Although this method isn't used directly by this class,
    // there are some sub-classes that are using it.
    typedef typename boost::mpl::if_<boost::is_class<T>,      T&,T>::type get_type;
    get_type       getitem(Py_ssize_t index)       { return (*this)[canonical_index(index)]; }
    typedef typename boost::mpl::if_<boost::is_class<T>,const T&,T>::type get_type_const;
    get_type_const getitem(Py_ssize_t index) const { return (*this)[canonical_index(index)]; }

    // We return an internal reference for class-types and a copy of the data
    // for non-class types.  Returning an internal refeference doesn't seem
    // to work with non-class types.

    boost::python::object  getobjectTuple (Py_ssize_t index)
    {
        typedef typename boost::mpl::if_<boost::is_class<T>,
                                         ReturnReference<T>,
                                         ReturnByValue<T> >::type convertType;

        boost::python::object retval;
        int referenceMode = 0;

        const size_t i = canonical_index(index);
        T& val = _ptr[(isMaskedReference() ? raw_ptr_index(i) : i) * _stride];

        if (_writable)
        {
            retval = convertType::applyWritable (val);

            if (convertType::isReferenceWrap())
                referenceMode = 0;  // Managed reference.
            else
                referenceMode = 2;  // Default policy (return-by-value)
        }
        else
        {
            retval = convertType::applyReadOnly (val);

            if (convertType::isReferenceWrap())
                referenceMode = 1;  // Copy const reference
            else
                referenceMode = 2;  // Default policy (return-by-value)
        }

        return boost::python::make_tuple (referenceMode, retval);
    }

    boost::python::object  getobjectTuple (Py_ssize_t index) const
    {
        typedef typename boost::mpl::if_<boost::is_class<T>,
                                         ReturnReference<T>,
                                         ReturnByValue<T> >::type convertType;

        boost::python::object retval;
        int referenceMode = 1;

        const size_t i = canonical_index(index);
        const T& val = _ptr[(isMaskedReference() ? raw_ptr_index(i) : i) * _stride];

        retval = convertType::applyReadOnly (val);

        if (convertType::isReferenceWrap())
            referenceMode = 1;  // Copy const reference
        else
            referenceMode = 2;  // Default policy (return-by-value)

        return boost::python::make_tuple (referenceMode, retval);
    }

    FixedArray getslice(::PyObject *index) const
    {
        size_t start=0, end=0, slicelength=0;
        Py_ssize_t step;
        extract_slice_indices(index,start,end,step,slicelength);
        FixedArray f(slicelength);

        if (isMaskedReference())
        {
            for (size_t i=0; i<slicelength; ++i)
                f._ptr[i] = _ptr[raw_ptr_index(start+i*step)*_stride];
        }
        else
        {
            for (size_t i=0; i<slicelength; ++i)
                f._ptr[i] = _ptr[(start+i*step)*_stride];
        }
        return f;
    }

    template <typename MaskArrayType>
    FixedArray getslice_mask(const MaskArrayType& mask)
    {
        // 'writable' state is preserved in the returned fixed-array.
        FixedArray f(*this, mask);
        return f;
    }

    void
    setitem_scalar(PyObject *index, const T &data)
    {
        if (!_writable)
            throw std::invalid_argument("Fixed array is read-only.");

        size_t start=0, end=0, slicelength=0;
        Py_ssize_t step;
        extract_slice_indices(index,start,end,step,slicelength);

        if (isMaskedReference())
        {
            for (size_t i=0; i<slicelength; ++i)
                _ptr[raw_ptr_index(start+i*step)*_stride] = data;
        }
        else
        {
            for (size_t i=0; i<slicelength; ++i)
                _ptr[(start+i*step)*_stride] = data;
        }
    }

    template <typename MaskArrayType>
    void
    setitem_scalar_mask(const MaskArrayType &mask, const T &data)
    {
        if (!_writable)
            throw std::invalid_argument("Fixed array is read-only.");

        size_t len = match_dimension(mask, false);

        if (isMaskedReference())
        {
            for (size_t i = 0; i < len; ++i)
                _ptr[raw_ptr_index(i)*_stride] = data;
        }
        else
        {
            for (size_t i=0; i<len; ++i)
                if (mask[i]) _ptr[i*_stride] = data;
        }
    }

    template <typename ArrayType>
    void
    setitem_vector(::PyObject *index, const ArrayType &data)
    {
        if (!_writable)
            throw std::invalid_argument("Fixed array is read-only.");

        size_t start=0, end=0, slicelength=0;
        Py_ssize_t step;
        extract_slice_indices(index,start,end,step,slicelength);
        
        // we have a valid range of indices
        if ((size_t)data.len() != slicelength) {
            PyErr_SetString(PyExc_IndexError, "Dimensions of source do not match destination");
            boost::python::throw_error_already_set();
        }

        if (isMaskedReference())
        {
            for (size_t i=0; i<slicelength; ++i)
                _ptr[raw_ptr_index(start+i*step)*_stride] = data[i];
        }
        else
        {
            for (size_t i=0; i<slicelength; ++i)
                _ptr[(start+i*step)*_stride] = data[i];
        }
    }

    template <typename MaskArrayType, typename ArrayType>
    void
    setitem_vector_mask(const MaskArrayType &mask, const ArrayType &data)
    {
        if (!_writable)
            throw std::invalid_argument("Fixed array is read-only.");

        // We could relax this but this restriction if there's a good
        // enough reason too.

        if (isMaskedReference())
        {
            throw std::invalid_argument("We don't support setting item masks for masked reference arrays.");
        }

        size_t len = match_dimension(mask);
        if ((size_t)data.len() == len)
        {
            for (size_t i = 0; i < len; ++i)
                if (mask[i]) _ptr[i*_stride] = data[i];
        }
        else
        {
            size_t count = 0;
            for (size_t i = 0; i < len; ++i)
                if (mask[i]) count++;

            if (data.len() != count) {
                throw std::invalid_argument("Dimensions of source data do not match destination either masked or unmasked");
            }

            Py_ssize_t dataIndex = 0;
            for (size_t i = 0; i < len; ++i)
            {
                if (mask[i])
                {
                    _ptr[i*_stride] = data[dataIndex];
                    dataIndex++;
                }
            }
        }
    }

    // exposed as Py_ssize_t for compatilbity with standard python sequences
    Py_ssize_t len()      const { return _length;   }
    size_t     stride()   const { return _stride;   }
    bool       writable() const { return _writable; }

    // This method is mainly here for use in confidence tests, but there may
    // be other use-cases where a writable array needs to be made read-only.
    // Note that we do not provide a 'makeWritable' method here, because that
    // type of operation shouldn't be allowed.
    void       makeReadOnly() { _writable = false; }

    // no bounds checking on i!
    T& operator [] (size_t i)
    {
        if (!_writable)
            throw std::invalid_argument("Fixed array is read-only.");

        return _ptr[(isMaskedReference() ? raw_ptr_index(i) : i) * _stride];
    }

    // no bounds checking on i!
    const T& operator [] (size_t i) const
    {
        return _ptr[(isMaskedReference() ? raw_ptr_index(i) : i) * _stride];
    }

    // no mask conversion or bounds checking on i!
    T& direct_index(size_t i)
    {
        if (!_writable)
            throw std::invalid_argument("Fixed array is read-only.");

        return _ptr[i*_stride];
    }

    // no mask conversion or bounds checking on i!
    const T& direct_index (size_t i) const
    {
        return _ptr[i*_stride];
    }

    // In some cases, an access to the raw data without the 'writable' check 
    // is needed.  Generally in specialized python-wrapping helpers.
    T& unchecked_index (size_t i)
    {
        return _ptr[(isMaskedReference() ? raw_ptr_index(i) : i) * _stride];
    }

    T& unchecked_direct_index (size_t i)
    {
        return _ptr[i*_stride];
    }

    bool isMaskedReference() const {return _indices.get() != 0;}
    size_t unmaskedLength() const {return _unmaskedLength;}

    // Conversion of indices to raw pointer indices.
    // This should only be called when this is a masked reference.
    // No safety checks done for performance.
    size_t raw_ptr_index(size_t i) const
    {
        assert(isMaskedReference());
        assert(i < _length);
        assert(_indices[i] >= 0 && _indices[i] < _unmaskedLength);
        return _indices[i];
    }

    static boost::python::class_<FixedArray<T> > register_(const char *doc)
    {
        // Depending on the data-type (class or fundamental) and the writable
        // state of the array, different forms are returned by the '__getitem__' 
        // method.  If writable and a class, an internal reference to the data
        // is returned so that its value can be changed.  If not-writable or a
        // fundemental data type (float, int, etc.), then a 'copy' of the data
        // is returned.

        typename boost::python::object (FixedArray<T>::*nonconst_getobject)(Py_ssize_t) =
                                                 &FixedArray<T>::getobjectTuple;
        typename boost::python::object (FixedArray<T>::   *const_getobject)(Py_ssize_t) const =
                                                 &FixedArray<T>::getobjectTuple;

        boost::python::class_<FixedArray<T> > c(name(),doc, boost::python::init<size_t>("construct an array of the specified length initialized to the default value for the type"));
        c
            .def(boost::python::init<const FixedArray<T> &>("construct an array with the same values as the given array"))
            .def(boost::python::init<const T &,size_t>("construct an array of the specified length initialized to the specified default value"))
            .def("__getitem__", &FixedArray<T>::getslice)
            .def("__getitem__", &FixedArray<T>::getslice_mask<FixedArray<int> > )
            .def("__getitem__",    const_getobject,
                 selectable_postcall_policy_from_tuple<
                     boost::python::with_custodian_and_ward_postcall<0,1>,
                     boost::python::return_value_policy<boost::python::copy_const_reference>,
                     boost::python::default_call_policies>())
            .def("__getitem__", nonconst_getobject,
                 selectable_postcall_policy_from_tuple<
                     boost::python::with_custodian_and_ward_postcall<0,1>,
                     boost::python::return_value_policy<boost::python::copy_const_reference>,
                     boost::python::default_call_policies>())
            .def("__setitem__", &FixedArray<T>::setitem_scalar)
            .def("__setitem__", &FixedArray<T>::setitem_scalar_mask<FixedArray<int> >)
            .def("__setitem__", &FixedArray<T>::setitem_vector<FixedArray<T> >)
            .def("__setitem__", &FixedArray<T>::setitem_vector_mask<FixedArray<int>, FixedArray<T> >)
            .def("__len__",&FixedArray<T>::len)
            .def("writable",&FixedArray<T>::writable)
            .def("makeReadOnly", &FixedArray<T>::makeReadOnly)
            .def("ifelse",&FixedArray<T>::ifelse_scalar)
            .def("ifelse",&FixedArray<T>::ifelse_vector)
            ;
        return c;
    }

    template <typename ArrayType>
    size_t match_dimension(const ArrayType &a1, bool strictComparison = true) const
    {
        if (len() == a1.len())
            return len();

        bool throwExc = false;
        if (strictComparison)
            throwExc = true;
        else if (isMaskedReference())
        {
            if (_unmaskedLength != a1.len())
                throwExc = true;
        }
        else
            throwExc = true;

        if (throwExc)
        {
          throw std::invalid_argument("Dimensions of source do not match destination");
        }

        return len();
    }

    FixedArray<T> ifelse_vector(const FixedArray<int> &choice, const FixedArray<T> &other) {
        size_t len = match_dimension(choice);
        match_dimension(other);
        FixedArray<T> tmp(len); // should use default construction but V3f doens't initialize
        for (size_t i=0; i < len; ++i) tmp[i] = choice[i] ? (*this)[i] : other[i];
        return tmp;
    }

    FixedArray<T> ifelse_scalar(const FixedArray<int> &choice, const T &other) {
        size_t len = match_dimension(choice);
        FixedArray<T> tmp(len); // should use default construction but V3f doens't initialize
        for (size_t i=0; i < len; ++i) tmp[i] = choice[i] ? (*this)[i] : other;
        return tmp;
    }

    // Instantiations of fixed ararys must implement this static member
    static const char *name();

    // Various 'Accessor' classes used in performance-critical areas while also
    // managing the writable/read-only state efficiently.

    class ReadOnlyDirectAccess
    {
      public:
        ReadOnlyDirectAccess (const FixedArray<T>& array)
            : _ptr (array._ptr), _stride (array._stride)
        {
            if (array.isMaskedReference())
                throw std::invalid_argument ("Fixed array is masked. ReadOnlyDirectAccess not granted.");
        }

        ReadOnlyDirectAccess (const ReadOnlyDirectAccess& other)
            : _ptr (other._ptr), _stride (other._stride) {}

        const T&  operator[] (size_t i) const { return _ptr[i*_stride]; }

      private:
        const T*  _ptr;

      protected:
        const size_t  _stride;
    };

    class WritableDirectAccess : public ReadOnlyDirectAccess
    {
      public:
        WritableDirectAccess (FixedArray<T>& array)
            : ReadOnlyDirectAccess (array), _ptr (array._ptr)
        {
            if (!array.writable())
                throw std::invalid_argument ("Fixed array is read-only.  WritableDirectAccess not granted.");
        }

        WritableDirectAccess (const WritableDirectAccess& other)
            : ReadOnlyDirectAccess (other), _ptr (other._ptr) {}

        T&  operator[] (size_t i) { return _ptr[i*_stride]; }

      private:
        T*  _ptr;

        using ReadOnlyDirectAccess::_stride;
    };

    //

    class ReadOnlyMaskedAccess
    {
      public:
        ReadOnlyMaskedAccess (const FixedArray<T>& array)
            : _ptr (array._ptr), _stride (array._stride),
              _indices (array._indices)
        {
            if (!array.isMaskedReference())
                throw std::invalid_argument ("Fixed array is not masked. ReadOnlyMaskedAccess not granted.");
        }

        ReadOnlyMaskedAccess (const ReadOnlyMaskedAccess& other)
            : _ptr (other._ptr), _stride (other._stride),
              _indices (other._indices) {}

        // No index-range check here.
        const T&  operator[] (size_t i) const { return _ptr[_indices[i]*_stride]; }

      private:
        const T*  _ptr;

      protected:
        const size_t                 _stride;
        boost::shared_array<size_t>  _indices;
    };

    class WritableMaskedAccess : public ReadOnlyMaskedAccess
    {
      public:
        WritableMaskedAccess (FixedArray<T>& array)
            : ReadOnlyMaskedAccess (array), _ptr (array._ptr)
        {
            if (!array.writable())
                std::invalid_argument ("Fixed array is read-only. WritableMaskedAccess not granted.");
        }

        WritableMaskedAccess (const WritableMaskedAccess& other)
            : ReadOnlyMaskedAccess (other), _ptr (other._ptr) {}

        // No index-range check here.
        T&  operator[] (size_t i) { return _ptr[_indices[i]*_stride]; }

      private:
        T*  _ptr;

        using ReadOnlyMaskedAccess::_stride;
        using ReadOnlyMaskedAccess::_indices;
    };

};

//
// Helper struct for arary indexing  with a known compile time length
//
template <class Container, class Data>
struct IndexAccessDefault {
    typedef Data & result_type;
    static Data & apply(Container &c, size_t i) { return c[i]; }
};

template <class Container, class Data, int Length, class IndexAccess = IndexAccessDefault<Container,Data> >
struct StaticFixedArray
{
    static Py_ssize_t len(const Container &) { return Length; }
    static typename   IndexAccess::result_type getitem(Container &c, Py_ssize_t index) { return IndexAccess::apply(c,canonical_index(index)); }
    static void       setitem(Container &c, Py_ssize_t index, const Data &data) { IndexAccess::apply(c,canonical_index(index)) = data; }
    static size_t     canonical_index(Py_ssize_t index)
    {
        if (index < 0) index += Length;
        if (index < 0 || index >= Length) {
            PyErr_SetString(PyExc_IndexError, "Index out of range");
            boost::python::throw_error_already_set();
        }
        return index;
    }
};

}

#endif // _PyImathFixedArray_h_
