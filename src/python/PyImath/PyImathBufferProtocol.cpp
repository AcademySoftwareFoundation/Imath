//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#include <ImathVec.h>
#include "PyImathBufferProtocol.h"
#include "PyImathExport.h"
#include "PyImathFixedArray.h"
#include "PyImathFixedArrayTraits.h"

#include <type_traits>

namespace PyImath {

namespace {

//  A wrapper API for the buffer protocol functions to access and traverse
// the memory of a FixedArray.
//
template <class ArrayT>
class BufferAPI
{
  using T = typename ArrayT::BaseType;
  static_assert (std::is_same<ArrayT, FixedArray<T> >::value,
                 "BufferAPI is only valid for FixedArray classes");

  public:

    virtual ~BufferAPI()
      { delete[] shape; delete[] stride; }

    //  The size, in bytes, of the smallest individual element of a buffer
    // element.  For example, a V3fArray is a 2D array of 4-byte floats.
    Py_ssize_t atomicSize() const
      { return FixedArrayAtomicSize<T>::value; }

    //  NonCopyable
    BufferAPI (const BufferAPI &rhs)            = delete;
    BufferAPI &operator= (const BufferAPI &rhs) = delete;

    //  API
    virtual bool        sharedBuffer() const = 0;
    virtual Py_ssize_t  numBytes() const     = 0;
    virtual bool        readOnly() const     = 0;
    virtual void *      buffer()             = 0;

  protected:

    BufferAPI (const unsigned int length, const unsigned int interleave)
     : dimensions (FixedArrayDimension<T>::value),
            shape (new Py_ssize_t[dimensions]),
           stride (new Py_ssize_t[dimensions])
    {
        shape[0]  = Py_ssize_t (length);
        stride[0] = atomicSize() * FixedArrayWidth<T>::value * interleave;
        for (unsigned int d=1; d<dimensions; d++)
        {
            shape[d]  = FixedArrayWidth<T>::value * interleave;
            stride[d] = atomicSize();
        }
    }

  public:

    //  The number of dimensions in the data buffer (e.g. a one dimensional
    // FixedArray of V3fs would have a data buffer dimension of 2).
    int dimensions;

    Py_ssize_t *shape;
    Py_ssize_t *stride;
};


//  The SharedBuffer class is used for buffers that will be shared between
// two different Python objects, and so the FixedArray is stored internally
// as a reference.
//
template <class ArrayT>
class SharedBufferAPI : public BufferAPI<ArrayT>
{
  public:

    using BufferAPI<ArrayT>::atomicSize;

    explicit
    SharedBufferAPI (ArrayT &a)
     : BufferAPI<ArrayT> (a.len(), a.stride()),
              _orig (a)
    {}

    //  NonCopyable
    SharedBufferAPI (const SharedBufferAPI &rhs)            = delete;
    SharedBufferAPI &operator= (const SharedBufferAPI &rhs) = delete;

    virtual ~SharedBufferAPI() = default;

    bool sharedBuffer() const override
     { return true; }

    Py_ssize_t numBytes() const override
     { return _orig.len() * atomicSize() * _orig.stride(); }

    bool readOnly() const override
     { return !_orig.writable(); }

    void *buffer() override
     { return static_cast<void *> (&_orig.direct_index(0)); }

  private:

    ArrayT &_orig;
};


//  This class exists for the case in which the Python array view
// object is writable but the source FixedArray is not.
//
template <class ArrayT>
class CopyBufferAPI : public BufferAPI<ArrayT>
{
  public:

    using BufferAPI<ArrayT>::atomicSize;

    explicit
    CopyBufferAPI (ArrayT &a)
     : BufferAPI<ArrayT> (a.len(), a.stride()),
              _copy (a)
    {}

    virtual ~CopyBufferAPI() = default;

    //  NonCopyable
    CopyBufferAPI (const CopyBufferAPI &rhs)            = delete;
    CopyBufferAPI &operator= (const CopyBufferAPI &rhs) = delete;

    bool sharedBuffer() const override
     { return false; }

    Py_ssize_t numBytes() const override
     { return _copy.len() * atomicSize() * _copy.stride(); }

    bool readOnly() const override
     { return false; }

    void *buffer() override
     { return static_cast<void *> (&_copy.direct_index(0)); }

  private:

    ArrayT _copy;
};


template <class ArrayT>
Py_ssize_t
bufferInfo (ArrayT &array, void **buf)
{
    *buf = static_cast<void *> (&array.direct_index(0));
    return array.len() * sizeof(typename ArrayT::BaseType);
}


template <class ArrayT>
Py_ssize_t
getreadbuf (PyObject *obj, Py_ssize_t segment, void **buf)
{
    if (segment != 0)
    {
        PyErr_SetString (PyExc_ValueError, "FixedArrays are not segmented");
        return -1;
    }

    boost::python::extract<ArrayT> eObj (obj);
    if (!eObj.check())
    {
        PyErr_SetString (PyExc_ValueError, "Cannot extract FixedArray");
        return -1;
    }

    ArrayT array = eObj();
    return bufferInfo<ArrayT> (array, buf);
}


template <class ArrayT>
Py_ssize_t
getwritebuf (PyObject *obj, Py_ssize_t segment, void **buf)
{
    PyErr_SetString
        (PyExc_ValueError,
         "writable buffers supported only with new-style buffer protocol.");

    return -1;
}


template <class ArrayT>
Py_ssize_t
getsegcount (PyObject *obj, Py_ssize_t *lenp)
{
    //  FixedArrays are always in one, fixed-sized block
    return 1;
}


template <class ArrayT>
Py_ssize_t
getcharbuf (PyObject *obj, Py_ssize_t segment, const char **charBuf)
{
    return getreadbuf<ArrayT> (obj, segment, (void **)charBuf);
}


template <class ArrayT>
void
releasebuffer (PyObject *obj, Py_buffer *view)
{
    delete static_cast<BufferAPI<ArrayT> *>(view->internal);
}


template <class ArrayT>
int
getbuffer (PyObject *obj, Py_buffer *view, int flags)
{
    if (view == nullptr)
    {
        PyErr_SetString (PyExc_ValueError, "Buffer view is NULL");
        return -1;
    }

    if ((flags & PyBUF_F_CONTIGUOUS) == PyBUF_F_CONTIGUOUS)
    {
        PyErr_SetString (PyExc_ValueError, "FORTRAN order not supported");
        return -1;
    }

    boost::python::extract<ArrayT> eObj (obj);
    if (!eObj.check())
    {
        PyErr_SetString (PyExc_ValueError, "Cannot extract FixedArray");
        return -1;
    }
    ArrayT array = eObj();

    if (array.isMaskedReference())
    {
        PyErr_SetString (PyExc_ValueError, "Buffer protocol does not support masked references");
        return -1;
    }

    BufferAPI<ArrayT> *api   = nullptr;
    bool writableBuffer = ((flags & PyBUF_WRITABLE) == PyBUF_WRITABLE);
    if (writableBuffer && !array.writable())
        api = new CopyBufferAPI<ArrayT> (array);  
    else
        api = new SharedBufferAPI<ArrayT> (array);  

    view->internal   = api;
    view->buf        = api->buffer();
    view->len        = api->numBytes();
    view->readonly   = api->readOnly();
    view->itemsize   = api->atomicSize();
    view->suboffsets = nullptr;

    view->format = ((flags & PyBUF_FORMAT) == PyBUF_FORMAT) 
                 ? PyFormat<typename ArrayT::BaseType>()
                 : nullptr;

    view->strides = ((flags & PyBUF_STRIDES) == PyBUF_STRIDES)
                  ? api->stride
                  : nullptr;

    if ((flags & PyBUF_ND) == PyBUF_ND)
    {
        view->ndim  = api->dimensions;
        view->shape = api->shape;
    }
    else
    {
        view->ndim  = 0;
        view->shape = nullptr;
    }

    view->obj = obj;
    Py_INCREF (obj);

    return 0;
}


// This structure serves to instantiate a PyBufferProcs instance with pointers
// to the right buffer protocol functions.
template <class ArrayT>
struct FixedArrayBufferProcs
{
    static PyBufferProcs procs;
};


template <class ArrayT>
PyBufferProcs FixedArrayBufferProcs<ArrayT>::procs =
{
#if PY_MAJOR_VERSION == 2
    (readbufferproc)    getreadbuf<ArrayT>,
    (writebufferproc)   getwritebuf<ArrayT>,
    (segcountproc)      getsegcount<ArrayT>,
    (charbufferproc)    getcharbuf<ArrayT>,
    (getbufferproc)     getbuffer<ArrayT>,
    (releasebufferproc) releasebuffer<ArrayT>
#else  //  Note deprecation of support for the older style
    (getbufferproc)     getbuffer<ArrayT>,
    (releasebufferproc) releasebuffer<ArrayT>
#endif
};

} // anonymous


template <class ArrayT>
void
add_buffer_protocol (boost::python::class_<ArrayT> &classObj)
{
    auto *typeObj = reinterpret_cast<PyTypeObject *> (classObj.ptr());
    typeObj->tp_as_buffer = &FixedArrayBufferProcs<ArrayT>::procs;
#if PY_MAJOR_VERSION == 2
    typeObj->tp_flags |= (Py_TPFLAGS_HAVE_NEWBUFFER | Py_TPFLAGS_HAVE_GETCHARBUFFER);
#endif
}


template <class ArrayT>
ArrayT *
fixedArrayFromBuffer (PyObject *obj)
{
    if (!PyObject_CheckBuffer (obj))
        throw std::invalid_argument ("Python object does not support the buffer protocol");

    // Request a strided buffer with type & dimensions.
    Py_buffer view;
    memset(&view, 0, sizeof(view));
    if (PyObject_GetBuffer (obj, &view, PyBUF_FORMAT | PyBUF_STRIDES) != 0)
    {
        throw std::logic_error ("Failed to get dimensioned, typed buffer");
    }

    // We have a buffer.  Check that the type matches.
    if (!view.format ||
        view.format[0] == '>' ||  
        view.format[0] == '!' || 
        view.format[0] == '=' || 
        view.format[0] == '^')
    {
        PyBuffer_Release(&view);
        throw std::invalid_argument ("Unsupported buffer type");
    }

    ArrayT *array = new ArrayT (view.shape[0], PyImath::UNINITIALIZED);
    memcpy (&array->direct_index(0), view.buf, view.len);

    return array;
}

///////////////////////////////////////////////////////////////////////////////

template PYIMATH_EXPORT void add_buffer_protocol (boost::python::class_<FixedArray<short> >         &classObj);
template PYIMATH_EXPORT void add_buffer_protocol (boost::python::class_<FixedArray<int> >           &classObj);
template PYIMATH_EXPORT void add_buffer_protocol (boost::python::class_<FixedArray<int64_t> >       &classObj);
template PYIMATH_EXPORT void add_buffer_protocol (boost::python::class_<FixedArray<float> >         &classObj);
template PYIMATH_EXPORT void add_buffer_protocol (boost::python::class_<FixedArray<double> >        &classObj);
template PYIMATH_EXPORT void add_buffer_protocol (boost::python::class_<FixedArray<unsigned char> > &classObj);

template PYIMATH_EXPORT void add_buffer_protocol (boost::python::class_<FixedArray<Imath::Vec2<short> > >   &classObj);
template PYIMATH_EXPORT void add_buffer_protocol (boost::python::class_<FixedArray<Imath::Vec2<int> > >     &classObj);
template PYIMATH_EXPORT void add_buffer_protocol (boost::python::class_<FixedArray<Imath::Vec2<int64_t> > > &classObj);
template PYIMATH_EXPORT void add_buffer_protocol (boost::python::class_<FixedArray<Imath::Vec2<float> > >   &classObj);
template PYIMATH_EXPORT void add_buffer_protocol (boost::python::class_<FixedArray<Imath::Vec2<double> > >  &classObj);
template PYIMATH_EXPORT void add_buffer_protocol (boost::python::class_<FixedArray<Imath::Vec3<short> > >   &classObj);
template PYIMATH_EXPORT void add_buffer_protocol (boost::python::class_<FixedArray<Imath::Vec3<int> > >     &classObj);
template PYIMATH_EXPORT void add_buffer_protocol (boost::python::class_<FixedArray<Imath::Vec3<int64_t> > > &classObj);
template PYIMATH_EXPORT void add_buffer_protocol (boost::python::class_<FixedArray<Imath::Vec3<float> > >   &classObj);
template PYIMATH_EXPORT void add_buffer_protocol (boost::python::class_<FixedArray<Imath::Vec3<double> > >  &classObj);
template PYIMATH_EXPORT void add_buffer_protocol (boost::python::class_<FixedArray<Imath::Vec4<short> > >   &classObj);
template PYIMATH_EXPORT void add_buffer_protocol (boost::python::class_<FixedArray<Imath::Vec4<int> > >     &classObj);
template PYIMATH_EXPORT void add_buffer_protocol (boost::python::class_<FixedArray<Imath::Vec4<int64_t> > > &classObj);
template PYIMATH_EXPORT void add_buffer_protocol (boost::python::class_<FixedArray<Imath::Vec4<float> > >   &classObj);
template PYIMATH_EXPORT void add_buffer_protocol (boost::python::class_<FixedArray<Imath::Vec4<double> > >  &classObj);

template PYIMATH_EXPORT FixedArray<Imath::Vec2<short> >*   fixedArrayFromBuffer (PyObject *obj);
template PYIMATH_EXPORT FixedArray<Imath::Vec2<int> >*     fixedArrayFromBuffer (PyObject *obj);
template PYIMATH_EXPORT FixedArray<Imath::Vec2<int64_t> >* fixedArrayFromBuffer (PyObject *obj);
template PYIMATH_EXPORT FixedArray<Imath::Vec2<float> >*   fixedArrayFromBuffer (PyObject *obj);
template PYIMATH_EXPORT FixedArray<Imath::Vec2<double> >*  fixedArrayFromBuffer (PyObject *obj);
template PYIMATH_EXPORT FixedArray<Imath::Vec3<short> >*   fixedArrayFromBuffer (PyObject *obj);
template PYIMATH_EXPORT FixedArray<Imath::Vec3<int> >*     fixedArrayFromBuffer (PyObject *obj);
template PYIMATH_EXPORT FixedArray<Imath::Vec3<int64_t> >* fixedArrayFromBuffer (PyObject *obj);
template PYIMATH_EXPORT FixedArray<Imath::Vec3<float> >*   fixedArrayFromBuffer (PyObject *obj);
template PYIMATH_EXPORT FixedArray<Imath::Vec3<double> >*  fixedArrayFromBuffer (PyObject *obj);
template PYIMATH_EXPORT FixedArray<Imath::Vec4<short> >*   fixedArrayFromBuffer (PyObject *obj);
template PYIMATH_EXPORT FixedArray<Imath::Vec4<int> >*     fixedArrayFromBuffer (PyObject *obj);
template PYIMATH_EXPORT FixedArray<Imath::Vec4<int64_t> >* fixedArrayFromBuffer (PyObject *obj);
template PYIMATH_EXPORT FixedArray<Imath::Vec4<float> >*   fixedArrayFromBuffer (PyObject *obj);
template PYIMATH_EXPORT FixedArray<Imath::Vec4<double> >*  fixedArrayFromBuffer (PyObject *obj);
template PYIMATH_EXPORT FixedArray<short>*                 fixedArrayFromBuffer (PyObject *obj);
template PYIMATH_EXPORT FixedArray<int>*                   fixedArrayFromBuffer (PyObject *obj);
template PYIMATH_EXPORT FixedArray<int64_t>*               fixedArrayFromBuffer (PyObject *obj);
template PYIMATH_EXPORT FixedArray<float>*                 fixedArrayFromBuffer (PyObject *obj);
template PYIMATH_EXPORT FixedArray<double>*                fixedArrayFromBuffer (PyObject *obj);

}
