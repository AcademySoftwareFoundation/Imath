//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off


#include <boost/python.hpp>
#include <boost/shared_array.hpp>
#include <boost/any.hpp>
#include <ImathVec.h>
#include "PyImathExport.h"
#include "PyImathFixedVArray.h"

namespace PyImath {

template <class T>
FixedVArray<T>::FixedVArray (std::vector<T>* ptr, Py_ssize_t length,
                             Py_ssize_t stride, bool writable)
    : _ptr(ptr), _length(length), _stride(stride), _writable (writable),
      _handle(), _unmaskedLength(0)
{
    if (length < 0)
    {
        throw std::invalid_argument("Fixed array length must be non-negative");
    }
    if (stride <= 0)
    {
        throw std::invalid_argument("Fixed array stride must be positive");
    }

    // Nothing else to do (pointer given, so we have the data)
}

template <class T>
FixedVArray<T>::FixedVArray (std::vector<T>* ptr, Py_ssize_t length,
                             Py_ssize_t stride, boost::any handle, bool writable)
    : _ptr(ptr), _length(length), _stride(stride), _writable(writable),
      _handle(handle), _unmaskedLength(0)
{
    if (length < 0)
    {
        throw std::invalid_argument("Fixed array length must be non-negative");
    }
    if (stride <= 0)
    {
        throw std::invalid_argument("Fixed array stride must be positive");
    }

    // Nothing else to do (pointer given, so we have the data)
}

template <class T>
FixedVArray<T>::FixedVArray (const std::vector<T>* ptr, Py_ssize_t length,
                             Py_ssize_t stride)
    : _ptr(const_cast<std::vector<T> *>(ptr)), _length(length), _stride(stride),
      _writable(false), _handle(), _unmaskedLength(0)
{
    if (length < 0)
    {
        throw std::invalid_argument("Fixed array length must be non-negative");
    }
    if (stride <= 0)
    {
        throw std::invalid_argument("Fixed array stride must be positive");
    }

    // Nothing else to do (pointer given, so we have the data)
}

template <class T>
FixedVArray<T>::FixedVArray (const std::vector<T>* ptr, Py_ssize_t length,
                             Py_ssize_t stride, boost::any handle)
    : _ptr(const_cast<std::vector<T> *>(ptr)), _length(length), _stride(stride),
      _writable(false), _handle(handle), _unmaskedLength(0)
{
    if (length < 0)
    {
        throw std::invalid_argument("Fixed array length must be non-negative");
    }
    if (stride <= 0)
    {
        throw std::invalid_argument("Fixed array stride must be positive");
    }

    // Nothing else to do (pointer given, so we have the data)
}

template <class T>
FixedVArray<T>::FixedVArray(Py_ssize_t length)
    : _ptr(0), _length(length), _stride(1), _writable(true),
      _handle(), _unmaskedLength(0)
{
    if (length < 0)
    {
        throw std::invalid_argument("Fixed array length must be non-negative");
    }

    boost::shared_array<std::vector<T> > a(new std::vector<T>[length]);
 // Initial vectors in the array will be zero-length.
    _handle = a;
    _ptr = a.get();
}

template <class T>
FixedVArray<T>::FixedVArray(const T& initialValue, Py_ssize_t length)
    : _ptr(0), _length(length), _stride(1), _writable(true),
      _handle(), _unmaskedLength(0)
{
    if (length < 0)
    {
        throw std::invalid_argument("Fixed array length must be non-negative");
    }

    boost::shared_array<std::vector<T> > a(new std::vector<T>[length]);
    for (Py_ssize_t i = 0; i < length; ++i)
    {
        a[i].push_back (initialValue);
    }
    _handle = a;
    _ptr = a.get();
}

template <class T>
FixedVArray<T>::FixedVArray(FixedVArray<T>& other, const FixedArray<int>& mask)
    : _ptr(other._ptr), _stride(other._stride), _writable(other._writable),
      _handle(other._handle)
{
    if (other.isMaskedReference())
    {
      throw std::invalid_argument
            ("Masking an already-masked FixedVArray is not supported yet (SQ27000)");
    }

    size_t len = (size_t) other.match_dimension (mask);
    _unmaskedLength = len;

    size_t reduced_len = 0;
    for (size_t i = 0; i < len; ++i)
    {
        if (mask[i])
        {
            reduced_len++;
        }
    }

    _indices.reset (new size_t[reduced_len]);

    for (size_t i = 0, j = 0; i < len; ++i)
    {
        if (mask[i])
        {
            _indices[j] = i; // NOSONAR - suppress SonarCloud warning.
            j++;
        }
    }

    _length = reduced_len;
}

template <class T>
FixedVArray<T>::FixedVArray(const FixedArray<int>& size, const T& initialValue)
    : _ptr(nullptr), _length (size.len()), _stride(1),
      _writable(true), _handle(), _indices(), _unmaskedLength(0)
{
    boost::shared_array<std::vector<T> > a(new std::vector<T>[_length]);
    for (size_t i = 0; i < _length; ++i)
    {
        if (size[i] < 0)
            throw std::invalid_argument("Attempt to create negative FixedVArray element");

        std::vector<T> &v = a[i];

        v.resize (size[i]);
        std::fill (v.begin(), v.end(), initialValue);
    }
    _handle = a;
    _ptr = a.get();
}

template <class T>
FixedVArray<T>::FixedVArray(const FixedVArray<T>& other)
    : _ptr(other._ptr),
      _length(other._length),
      _stride(other._stride),
      _writable (other._writable),
      _handle(other._handle),
      _indices(other._indices),
      _unmaskedLength(other._unmaskedLength)
{
    // Nothing.
}

template <class T>
const FixedVArray<T> &
FixedVArray<T>::operator = (const FixedVArray<T>& other)
{
    if (&other == this)
        return *this;

    _ptr            = other._ptr;
    _length         = other._length;
    _stride         = other._stride;
    _writable       = other._writable;
    _handle         = other._handle;
    _unmaskedLength = other._unmaskedLength;
    _indices        = other._indices;

    return *this;
}

template <class T>
FixedVArray<T>::~FixedVArray()
{
    // Nothing.
}


template <class T>
std::vector<T>&
FixedVArray<T>::operator [] (size_t i)
{
    if (!_writable)
        throw std::invalid_argument("Fixed V-array is read-only.");

    return _ptr[(_indices ? raw_ptr_index(i) : i) * _stride];
}

template <class T>
const std::vector<T>&
FixedVArray<T>::operator [] (size_t i) const
{
    return _ptr[(_indices ? raw_ptr_index(i) : i) * _stride];
}


namespace {

//
// Make an index suitable for indexing into an array in c++
// from a python index, which can be negative for indexing 
// relative to the end of an array.
//
size_t
canonical_index (Py_ssize_t index, const size_t& totalLength)
{
    if (index < 0)
    {
        index += totalLength;
    }
    if ((size_t) index >= totalLength || index < 0)
    {
        PyErr_SetString (PyExc_IndexError, "Index out of range");
        boost::python::throw_error_already_set();
    }
    return index;  // still a 'virtual' index if this is a masked reference array
}

void
extract_slice_indices (PyObject* index, size_t& start, size_t& end,
                       Py_ssize_t& step, size_t& sliceLength,
                       const size_t& totalLength)
{
    if (PySlice_Check (index))
    {
#if PY_MAJOR_VERSION > 2
        PyObject* slice = index;
#else
        PySliceObject* slice = reinterpret_cast<PySliceObject *>(index);
#endif
        Py_ssize_t s, e, sl;
        if (PySlice_GetIndicesEx(slice, totalLength, &s, &e, &step, &sl) == -1)
        {
            boost::python::throw_error_already_set();
        }
        if (s < 0 || e < -1 || sl < 0)
        {
            throw std::domain_error
                  ("Slice extraction produced invalid start, end, or length indices");
        }

        start = s;
        end   = e;
        sliceLength = sl;
    }
    else if (PyInt_Check (index))
    {
        size_t i = canonical_index (PyInt_AsSsize_t(index), totalLength);
        start = i;
        end   = i + 1;
        step  = 1;
        sliceLength = 1;
    }
    else
    {
        PyErr_SetString (PyExc_TypeError, "Object is not a slice");
        boost::python::throw_error_already_set();
    }
}

} // namespace


// this must have a call policy of return_internal_reference
template <class T>
FixedArray<T>
FixedVArray<T>::getitem (Py_ssize_t index)
{
    const size_t i = canonical_index (index, _length);
    std::vector<T>& data = _ptr[(_indices ? raw_ptr_index(i) : i) * _stride];
    return FixedArray<T>(data.empty() ? nullptr : &data[0], data.size(), 1, _writable);
}

template <class T>
FixedVArray<T>
FixedVArray<T>::getslice (PyObject* index) const
{
    size_t start       = 0;
    size_t end         = 0;
    size_t sliceLength = 0;
    Py_ssize_t step;
    extract_slice_indices (index, start, end, step, sliceLength, _length);

    FixedVArray<T> f(sliceLength);

    if (_indices)
    {
        for (size_t i = 0; i < sliceLength; ++i)
        {
            f._ptr[i] = _ptr[raw_ptr_index(start + i*step)*_stride];
        }
    }
    else
    {
        for (size_t i = 0; i < sliceLength; ++i)
        {
            f._ptr[i] = _ptr[(start + i*step)*_stride];
        }
    }

    return f;
}

template <class T>
FixedVArray<T>
FixedVArray<T>::getslice_mask (const FixedArray<int>& mask)
{
    return FixedVArray<T> (*this, mask);
}

template <class T>
void
FixedVArray<T>::setitem_scalar (PyObject* index, const FixedArray<T>& data)
{
    if (!_writable)
        throw std::invalid_argument ("Fixed V-array is read-only.");

    size_t start       = 0;
    size_t end         = 0;
    size_t sliceLength = 0;
    Py_ssize_t step;
    extract_slice_indices (index, start, end, step, sliceLength, _length);

    if (_indices)
    {
        for (size_t i = 0; i < sliceLength; ++i)
        {
            std::vector<T> &d =_ptr[raw_ptr_index(start + i*step)*_stride];
            if (data.len() != d.size())
                throw std::invalid_argument("FixedVArray::setitem: length of data does not match lengh of array element");

            if (data.isMaskedReference())
            {
                for (size_t j = 0; j < data.len(); ++j)
                {
                    d[j] = data[j];
                }
            }
            else
            {
                for (size_t j = 0; j < data.len(); ++j)
                {
                    d[j] = data.direct_index(j);
                }
            }
        }
    }
    else
    {
        for (size_t i = 0; i < sliceLength; ++i)
        {
            std::vector<T> &d =_ptr[(start + i*step)*_stride];
            if (data.len() != d.size())
                throw std::invalid_argument("FixedVArray::setitem: length of data does not match lengh of array element");

            if (data.isMaskedReference())
            {
                for (size_t j = 0; j < data.len(); ++j)
                {
                    d[j] = data[j];
                }
            }
            else
            {
                for (size_t j = 0; j < data.len(); ++j)
                {
                    d[j] = data.direct_index(j);
                }
            }
        }
    } 
}

template <class T>
void
FixedVArray<T>::setitem_scalar_mask (const FixedArray<int>& mask, const FixedArray<T>& data)
{
    if (!_writable)
        throw std::invalid_argument ("Fixed V-array is read-only.");

    size_t len = match_dimension(mask, false);

    if (_indices)
    {
        for (size_t i = 0; i < len; ++i)
        {
            // We don't need to actually look at 'mask' because
            // match_dimensions has already forced some expected condition.
            std::vector<T> &d =_ptr[raw_ptr_index(i)*_stride];
            if (data.len() != d.size())
                throw std::invalid_argument("FixedVArray::setitem: length of data does not match lengh of array element");

            if (data.isMaskedReference())
            {
                for (size_t j = 0; j < data.len(); ++j)
                {
                    d[j] = data[j];
                }
            }
            else
            {
                for (size_t j = 0; j < data.len(); ++j)
                {
                    d[j] = data.direct_index(j);
                }
            }
        }
    }
    else
    {
        for (size_t i = 0; i < len; ++i)
        {
            if (mask[i])
            {
                std::vector<T> &d = _ptr[i*_stride];
                if (data.len() != d.size())
                    throw std::invalid_argument("FixedVArray::setitem: length of data does not match lengh of array element");

                if (data.isMaskedReference())
                {
                    for (size_t j = 0; j < data.len(); ++j)
                    {
                        d[j] = data[j];
                    }
                }
                else
                {
                    for (size_t j = 0; j < data.len(); ++j)
                    {
                        d[j] = data.direct_index(j);
                    }
                }
            }
        }
    }
}
template <class T>
void
FixedVArray<T>::setitem_vector (PyObject* index, const FixedVArray<T>& data)
{
    if (!_writable)
        throw std::invalid_argument ("Fixed V-array is read-only.");

    size_t start       = 0;
    size_t end         = 0;
    size_t sliceLength = 0;
    Py_ssize_t step;
    extract_slice_indices (index, start, end, step, sliceLength, _length);

    if ((size_t) data.len() != sliceLength)
    {
        PyErr_SetString (PyExc_IndexError,
                         "Dimensions of source do not match destination");
        boost::python::throw_error_already_set();
    }

    if (_indices)
    {
        for (size_t i = 0; i < sliceLength; ++i)
        {
            _ptr[raw_ptr_index(start + i*step)*_stride] = data[i];
        }
    }
    else
    {
        for (size_t i = 0; i < sliceLength; ++i)
        {
            _ptr[(start + i*step)*_stride] = data[i];
        }
    }
}

template <class T>
void
FixedVArray<T>::setitem_vector_mask (const FixedArray<int>& mask,
                                     const FixedVArray<T>&  data)
{
    if (!_writable)
        throw std::invalid_argument ("Fixed V-array is read-only.");

    // This restriction could be removed if there is a compelling use-case.
    if (_indices)
    {
        throw std::invalid_argument
            ("We don't support setting item masks for masked reference arrays");
    }

    size_t len = match_dimension(mask);

    if ((size_t) data.len() == len)
    {
        for (size_t i = 0; i < len; ++i)
        {
            if (mask[i])
            {
                _ptr[i*_stride] = data[i];
            }
        }
    }
    else
    {
        size_t count = 0;
        for (size_t i = 0; i < len; ++i)
        {
            if (mask[i])
            {
                count++;
            }
        }
        if ((size_t) data.len() != count)
        {
            throw std::invalid_argument
                ("Dimensions of source data do not match destination "
                 "either masked or unmasked");
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


template <class T>
int
FixedVArray<T>::SizeHelper::getitem (Py_ssize_t index) const
{
    size_t i = canonical_index(index, _a._length);

    if (_a._indices)
    {
        return _a._ptr[_a.raw_ptr_index(i)*_a._stride].size();
    }

    return _a._ptr[i*_a._stride].size();
}

template <class T>
FixedArray<int>
FixedVArray<T>::SizeHelper::getitem_slice (PyObject* index) const
{
    size_t start       = 0;
    size_t end         = 0;
    size_t sliceLength = 0;
    Py_ssize_t step;
    extract_slice_indices (index, start, end, step, sliceLength, _a._length);

    FixedArray<int> f(sliceLength);

    if (_a._indices)
    {
        for (size_t i = 0; i < sliceLength; ++i)
        {
            f.direct_index(i) = _a._ptr[_a.raw_ptr_index(start + i*step)*_a._stride].size();
        }
    }
    else
    {
        for (size_t i = 0; i < sliceLength; ++i)
        {
            f.direct_index(i) = _a._ptr[(start + i*step)*_a._stride].size();
        }
    }

    return f;
}

template <class T>
FixedArray<int>
FixedVArray<T>::SizeHelper::getitem_mask (const FixedArray<int>& mask) const
{
    int len = mask.len();
    if (len != _a.len())
    {
        throw std::invalid_argument("Dimensions of mask do not match array");
    }
    
    int count = 0;
    for (size_t i = 0; i < mask.len(); ++i)
    {
        if (mask[i]) count += 1;
    }

    FixedArray<int> f(count);
    
    if (_a._indices)
    {
        size_t index = 0;
        for (size_t i = 0; i < mask.len(); ++i)
        {
            if (mask[i])
            {
                f.direct_index(index) = _a._ptr[_a.raw_ptr_index(i)*_a._stride].size();
                index += 1;
            }
        }
    }
    else
    {
        size_t index = 0;
        for (size_t i = 0; i < mask.len(); ++i)
        {
            if (mask[i])
            {
                f.direct_index(index) = _a._ptr[i*_a._stride].size();
                index += 1;
            }
        }
    }

    return f;
}


template <class T>
void
FixedVArray<T>::SizeHelper::setitem_scalar (PyObject* index, size_t size)
{
    if (!_a.writable())
        throw std::invalid_argument ("Fixed V-array is read-only.");

    size_t start       = 0;
    size_t end         = 0;
    size_t sliceLength = 0;
    Py_ssize_t step;
    extract_slice_indices (index, start, end, step, sliceLength, _a._length);

    if (_a._indices)
    {
        for (size_t i = 0; i < sliceLength; ++i)
        {
            _a._ptr[_a.raw_ptr_index(start + i*step)*_a._stride].resize(size);
        }
    }
    else
    {
        for (size_t i = 0; i < sliceLength; ++i)
        {
            _a._ptr[(start + i*step)*_a._stride].resize(size);
        }
    }
}

template <class T>
void
FixedVArray<T>::SizeHelper::setitem_scalar_mask (const FixedArray<int>& mask, size_t size)
{
    if (!_a.writable())
        throw std::invalid_argument ("Fixed V-array is read-only.");

    size_t len = _a.match_dimension(mask, false);

    if (_a._indices)
    {
        for (size_t i = 0; i < len; ++i)
        {
            // We don't need to actually look at 'mask' because
            // match_dimensions has already forced some expected condition.
            _a._ptr[_a.raw_ptr_index(i)*_a._stride].resize(size);
        }
    }
    else
    {
        for (size_t i = 0; i < len; ++i)
        {
            if (mask[i])
            {
                _a._ptr[i*_a._stride].resize(size);
            }
        }
    }
}

template <class T>
void
FixedVArray<T>::SizeHelper::setitem_vector(PyObject *index, const FixedArray<int> &size)
{
    if (!_a.writable())
        throw std::invalid_argument ("Fixed V-array is read-only.");

    size_t start=0, end=0, slicelength=0;
    Py_ssize_t step;
    extract_slice_indices(index,start,end,step,slicelength,_a._length);
        
    // we have a valid range of indices
    if (size.len() != slicelength) {
        PyErr_SetString(PyExc_IndexError, "Dimensions of source do not match destination");
        boost::python::throw_error_already_set();
    }

    if (_a._indices)
    {
        for (size_t i=0; i<slicelength; ++i)
            _a._ptr[_a.raw_ptr_index(start+i*step)*_a._stride].resize(size[i]);
    }
    else
    {
        for (size_t i=0; i<slicelength; ++i)
            _a._ptr[(start+i*step)*_a._stride].resize(size[i]);
    }
}

template <class T>
void
FixedVArray<T>::SizeHelper::setitem_vector_mask(const FixedArray<int> &mask, const FixedArray<int> &size)
{
    if (!_a.writable())
        throw std::invalid_argument ("Fixed V-array is read-only.");

    // We could relax this but this restriction if there's a good
    // enough reason too.

    if (_a._indices)
    {
        throw std::invalid_argument("We don't support setting item masks for masked reference arrays.");
    }

    size_t len = _a.match_dimension(mask);
    if (size.len() == len)
    {
        for (size_t i = 0; i < len; ++i)
            if (mask[i]) _a._ptr[i*_a._stride].resize(size[i]);
    }
    else
    {
        size_t count = 0;
        for (size_t i = 0; i < len; ++i)
            if (mask[i]) count++;

        if (size.len() != count) {
            throw std::invalid_argument("Dimensions of source data do not match destination either masked or unmasked");
        }

        Py_ssize_t sizeIndex = 0;
        for (size_t i = 0; i < len; ++i)
        {
            if (mask[i])
            {
                _a._ptr[i*_a._stride].resize(size[sizeIndex]);
                sizeIndex++;
            }
        }
    }
}

template <class T>
size_t
FixedVArray<T>::raw_ptr_index (size_t i) const
{
    assert (isMaskedReference());
    assert (i < _length);
    assert (_indices[i] >= 0 && _indices[i] < _unmaskedLength);

    return _indices[i];
}


template <class T>
boost::shared_ptr<typename FixedVArray<T>::SizeHelper>
FixedVArray<T>::getSizeHelper()
{
    return boost::shared_ptr<SizeHelper>(new typename FixedVArray<T>::SizeHelper (*this));
}

// static
template <class T>
boost::python::class_<FixedVArray<T> >
FixedVArray<T>::register_(const char* doc)
{
    boost::python::class_<FixedVArray<T> > fixedVArray_class (name(), doc,
        boost::python::init<Py_ssize_t>("Construct a variable array of the "
        "specified length initialized to the default value for the given type"));

    fixedVArray_class
        .def(boost::python::init<const FixedVArray<T> &>("Construct a variable array with the same values as the given array"))
        .def(boost::python::init<const T &, Py_ssize_t>("Construct a variable array of the specified length initialized to the specified default value"))
        .def(boost::python::init<const FixedArray<int> &, const T &>("Construct a variable array with each array size equal to the specified elements and initialized to the specified default value"))
        .def("__getitem__", &FixedVArray<T>::getslice)
        .def("__getitem__", &FixedVArray<T>::getslice_mask)
        .def("__getitem__", &FixedVArray<T>::getitem, boost::python::with_custodian_and_ward_postcall<1,0>())

        .def("__setitem__", &FixedVArray<T>::setitem_scalar)
        .def("__setitem__", &FixedVArray<T>::setitem_scalar_mask)
        .def("__setitem__", &FixedVArray<T>::setitem_vector)
        .def("__setitem__", &FixedVArray<T>::setitem_vector_mask)

        .def("__len__",     &FixedVArray<T>::len)
        .def("writable",    &FixedVArray<T>::writable)
        .def("makeReadOnly",&FixedVArray<T>::makeReadOnly)
        
        .add_property("size", boost::python::make_function(&FixedVArray<T>::getSizeHelper, boost::python::with_custodian_and_ward_postcall<0,1>()))
        ;

    {
        boost::python::scope s(fixedVArray_class);

        boost::python::class_<typename FixedVArray<T>::SizeHelper,boost::noncopyable> sizeHelper_class ("SizeHelper", boost::python::no_init);
        sizeHelper_class
            .def("__getitem__", &FixedVArray<T>::SizeHelper::getitem)
            .def("__getitem__", &FixedVArray<T>::SizeHelper::getitem_mask)
            .def("__getitem__", &FixedVArray<T>::SizeHelper::getitem_slice)

            .def("__setitem__", &FixedVArray<T>::SizeHelper::setitem_scalar)
            .def("__setitem__", &FixedVArray<T>::SizeHelper::setitem_scalar_mask)
            .def("__setitem__", &FixedVArray<T>::SizeHelper::setitem_vector)
            .def("__setitem__", &FixedVArray<T>::SizeHelper::setitem_vector_mask)
            ;
    }

    boost::python::register_ptr_to_python<boost::shared_ptr<typename FixedVArray<T>::SizeHelper> >();

    return fixedVArray_class;
}


// ---- Explicit Class Instantiation ---------------------------------

template class PYIMATH_EXPORT FixedVArray<int>;
template class PYIMATH_EXPORT FixedVArray<float>;
template class PYIMATH_EXPORT FixedVArray<Imath::Vec2<int> >;
template class PYIMATH_EXPORT FixedVArray<Imath::Vec2<float> >;

} // namespace PyImath
