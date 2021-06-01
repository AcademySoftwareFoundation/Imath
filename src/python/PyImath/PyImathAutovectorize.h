//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off


#ifndef _PyImathAutovectorize_h_
#define _PyImathAutovectorize_h_

#include <Python.h>
#include <boost/python.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/push_front.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/remove_if.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/count.hpp>
#include <boost/mpl/or.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/function_traits.hpp>
#include <boost/static_assert.hpp>
#include <boost/python/args.hpp>
#include <iostream>
#include "PyImathFixedArray.h"
#include "PyImathTask.h"
#include "PyImathUtil.h"

namespace PyImath {

struct op_with_precomputation {};

namespace detail {

using boost::is_base_of;
using boost::is_same;
using boost::is_const;
using boost::remove_const;
using boost::remove_reference;
using boost::function_traits;

using boost::mpl::at;
using boost::mpl::at_c;
using boost::mpl::push_front;
using boost::mpl::vector;
using boost::mpl::push_back;
using boost::mpl::transform;
using boost::mpl::fold;
using boost::mpl::_;
using boost::mpl::_1;
using boost::mpl::_2;
using boost::mpl::long_;
using boost::mpl::false_;
using boost::mpl::true_;
using boost::mpl::not_;
using boost::mpl::or_;
using boost::mpl::and_;
using boost::mpl::size;
using boost::mpl::remove_if;
using boost::mpl::if_;
using boost::mpl::for_each;


struct null_precomputation {
    static void precompute(size_t len) { return; }
};

template <class T> struct op_precompute
{
    static void
    apply(size_t len)
    {
        if_<is_base_of<op_with_precomputation,T>,
            T,
            null_precomputation>::type::precompute(len);
    }
};

template <int N>
struct possible_vectorizations
{
    typedef typename fold<
        typename possible_vectorizations<N-1>::type,
        vector<>,
        push_back<push_back<_1,push_back<_2,false_> >,push_back<_2,true_> >
    >::type type;
};

template <>
struct possible_vectorizations<0>
{
    typedef vector<vector<> > type;    
};

template <class Vectorizable>
struct disallow_vectorization
{
    template <class Vectorize>
    struct apply
    {
        // Valid = !Vectorize || Vectorizable
        typedef typename transform<Vectorize,not_<_> >::type DontVectorize;
        typedef typename transform<DontVectorize,Vectorizable,or_<_,_> >::type Valid;
        typedef typename not_<fold<Valid,true_,and_<_,_> > >::type type;
    };
};

template <class Vectorizable>
struct allowable_vectorizations
{
    typedef typename possible_vectorizations<size<Vectorizable>::value>::type possible;
    typedef typename remove_if<possible,disallow_vectorization<Vectorizable> >::type type;
};

template <class T>
bool any_masked(const T &value)
{
    return false;
};

template <class T>
bool any_masked(const PyImath::FixedArray<T> &value)
{
    return value.isMaskedReference();
};

template <class T1, class T2>
bool any_masked(const T1 &a, const T2 &b)
{
    return any_masked(a) || any_masked(b);
}

template <class T1, class T2, class T3>
bool any_masked(const T1 &a, const T2 &b, const T3 &c)
{
    return any_masked(a,b) || any_masked(c);
}

template <class T1, class T2, class T3, class T4>
bool any_masked(const T1 &a, const T2 &b, const T3 &c, const T4 &d)
{
    return any_masked(a,b) || any_masked(c,d);
}

//-----------------------------------------------------------------------------------------

//
// measure_argument returns a pair indicating the integral length of the argument
// (scalar arguments have implicit length 1), and a bool indicating whether the argument
// is a vectorized argument.
//
template <class T>
struct measure_argument
{
    static inline std::pair<size_t,bool> apply(T arg) { return std::make_pair(1,false); }
};

template <class T>
struct measure_argument<PyImath::FixedArray<T> >
{
    static inline std::pair<size_t,bool> apply(const PyImath::FixedArray<T> &arg) { return std::make_pair(arg.len(),true); }
};

//
// match_lengths returns the compatible length given two argument lengths
//
static inline std::pair<size_t,bool>
match_lengths(const std::pair<size_t,bool> &len1, const std::pair<size_t,bool> &len2)
{
    // scalar arguemnts are always compatible with other arguments
    if (len1.second == false) return len2;
    if (len2.second == false) return len1;

    // now both arguments are vectorized, check for dimension match
    if (len1.first != len2.first)
      throw std::invalid_argument("Array dimensions passed into function do not match");

    return len1;
}


//
// measure_arguments finds the length that a return value from a given
// set of arguments should have, throwing an exception if the lengths
// are incompatible.  If all arguments are scalar, length 1 is returned.
//
template <class arg1_type>
size_t
measure_arguments(const arg1_type &arg1)
{
    std::pair<size_t,bool> len = measure_argument<arg1_type>::apply(arg1);
    return len.first;
}

template <class arg1_type, class arg2_type>
size_t
measure_arguments(const arg1_type &arg1, const arg2_type &arg2)
{
    std::pair<size_t,bool> len = measure_argument<arg1_type>::apply(arg1);
    len = match_lengths(len,measure_argument<arg2_type>::apply(arg2));
    return len.first;
}

template <class arg1_type, class arg2_type, class arg3_type>
size_t
measure_arguments(const arg1_type &arg1, const arg2_type &arg2, const arg3_type &arg3)
{
    std::pair<size_t,bool> len = measure_argument<arg1_type>::apply(arg1);
    len = match_lengths(len,measure_argument<arg2_type>::apply(arg2));
    len = match_lengths(len,measure_argument<arg3_type>::apply(arg3));
    return len.first;
}

template <class arg1_type, class arg2_type, class arg3_type, class arg4_type>
size_t
measure_arguments(const arg1_type &arg1, const arg2_type &arg2, const arg3_type &arg3, const arg4_type &arg4)
{
    std::pair<size_t,bool> len = measure_argument<arg1_type>::apply(arg1);
    len = match_lengths(len,measure_argument<arg2_type>::apply(arg2));
    len = match_lengths(len,measure_argument<arg3_type>::apply(arg3));
    len = match_lengths(len,measure_argument<arg4_type>::apply(arg4));
    return len.first;
}

template <class arg1_type, class arg2_type, class arg3_type, class arg4_type, class arg5_type>
size_t
measure_arguments(const arg1_type &arg1, const arg2_type &arg2, const arg3_type &arg3, const arg4_type &arg4, const arg5_type &arg5)
{
    std::pair<size_t,bool> len = measure_argument<arg1_type>::apply(arg1);
    len = match_lengths(len,measure_argument<arg2_type>::apply(arg2));
    len = match_lengths(len,measure_argument<arg3_type>::apply(arg3));
    len = match_lengths(len,measure_argument<arg4_type>::apply(arg4));
    len = match_lengths(len,measure_argument<arg5_type>::apply(arg5));
    return len.first;
}

//-----------------------------------------------------------------------------------------

template <class T>
struct create_uninitalized_return_value
{
    static T apply(size_t length)
    {
        return T();
    }
};

template <class T>
struct create_uninitalized_return_value<PyImath::FixedArray<T> >
{
    static PyImath::FixedArray<T> apply(size_t length)
    {
        return PyImath::FixedArray<T>(Py_ssize_t(length),PyImath::UNINITIALIZED);
    }
};

template <class T, class VectorizeArg>
struct vectorized_result_type
{
    typedef typename if_<VectorizeArg,PyImath::FixedArray<T>,T>::type type;
};

template <typename T>
struct SimpleNonArrayWrapper
{
    struct ReadOnlyDirectAccess
    {
        ReadOnlyDirectAccess (const T& arg)
            : _arg (arg) {}
        ReadOnlyDirectAccess (const ReadOnlyDirectAccess& other)
            : _arg (other._arg) {}

        const T&  operator[] (size_t) const { return _arg; }

      private:
        const T&  _arg;
    };

    struct WritableDirectAccess : public ReadOnlyDirectAccess
    {
        WritableDirectAccess (T& arg)
            : ReadOnlyDirectAccess (arg), _arg (arg) {}
        WritableDirectAccess (const WritableDirectAccess& other)
            : ReadOnlyDirectAccess (other), _arg (other._arg) {}

        T&  operator[] (size_t) { return _arg; }

      private:
        T&  _arg;
    };

    typedef ReadOnlyDirectAccess ReadOnlyMaskedAccess;
    typedef WritableDirectAccess WritableMaskedAccess;
};


template <class T>
struct access_type
{
    typedef typename remove_reference<T>::type     prim_type;
    typedef typename remove_const<prim_type>::type base_type;
    typedef typename if_<is_const<prim_type>,
                         const PyImath::FixedArray<base_type> &,
                               PyImath::FixedArray<base_type> &>::type reference_type;
    typedef typename remove_reference<reference_type>::type class_type;

    typedef typename if_<is_const<prim_type>,
                         typename class_type::ReadOnlyMaskedAccess,
                         typename class_type::WritableMaskedAccess>::type masked;
    typedef typename if_<is_const<prim_type>,
                         typename class_type::ReadOnlyDirectAccess,
                         typename class_type::WritableDirectAccess>::type direct;
};

template <class T, class VectorizeArg>
struct argument_access_type
{
    typedef typename remove_const<typename remove_reference<T>::type>::type base_type;
    typedef typename if_<VectorizeArg,const PyImath::FixedArray<base_type> &,T>::type type;

    typedef typename if_<VectorizeArg,
                         typename remove_reference<type>::type,
                              SimpleNonArrayWrapper<base_type> >::type _class_type;

    typedef typename _class_type::ReadOnlyMaskedAccess masked;
    typedef typename _class_type::ReadOnlyDirectAccess direct;
};

template <class T, class VectorizeArg>
struct result_access_type
{
    typedef typename remove_const<typename remove_reference<T>::type>::type base_type;
    typedef typename if_<VectorizeArg,PyImath::FixedArray<base_type>,T>::type type;

    typedef typename if_<VectorizeArg, type,
                         SimpleNonArrayWrapper<base_type> >::type _class_type;

    typedef typename _class_type::WritableMaskedAccess masked;
    typedef typename _class_type::WritableDirectAccess direct;
};

template <class AccessType, class T>
AccessType getArrayAccess (T&  value)
           { return AccessType (value); }

template <class AccessType, class T>
AccessType getArrayAccess (const PyImath::FixedArray<T>& array)
           { return AccessType (array); }

template <class AccessType, class T>
AccessType getArrayAccess (PyImath::FixedArray<T>& array)
           { return AccessType (array); }

//

template <class Op, class result_access_type, class access_type>
struct VectorizedOperation1 : public Task
{
    result_access_type retAccess;
    access_type        access;

    VectorizedOperation1 (result_access_type r, access_type a1)
        : retAccess (r), access (a1) {}

    void execute(size_t start, size_t end)
    {
        for (size_t i = start; i < end; ++i)
        {
            retAccess[i] = Op::apply (access[i]);
        }
    }
};

template <class Op, class result_access_type, class access_type, class arg1_access_type>
struct VectorizedOperation2 : public Task
{
    result_access_type retAccess;
    access_type        access;
    arg1_access_type   argAccess;

    VectorizedOperation2(result_access_type r, access_type a1, arg1_access_type a2)
        : retAccess (r), access (a1), argAccess (a2) {}

    void execute(size_t start, size_t end)
    {
        for (size_t i = start; i < end; ++i)
        {
            retAccess[i] = Op::apply (access[i], argAccess[i]);
        }
    }
};

template <class Op, class result_access_type, class access_type,
                    class arg1_access_type, class arg2_access_type>
struct VectorizedOperation3 : public Task
{
    result_access_type retAccess;
    access_type        access;
    arg1_access_type   arg1Access;
    arg2_access_type   arg2Access;

    VectorizedOperation3(result_access_type r, access_type a,
                         arg1_access_type a1, arg2_access_type a2)
        : retAccess(r), access(a), arg1Access(a1), arg2Access(a2) {}

    void execute(size_t start, size_t end)
    {
        for (size_t i = start; i < end; ++i)
        {
            retAccess[i] = Op::apply(access[i], arg1Access[i], arg2Access[i]);
        }
    }
};

template <class Op, class result_access_type, class access_type,
                    class arg1_access_type, class arg2_access_type, class arg3_access_type>
struct VectorizedOperation4 : public Task
{
    result_access_type retAccess;
    access_type        access;
    arg1_access_type   arg1Access;
    arg2_access_type   arg2Access;
    arg3_access_type   arg3Access;

    VectorizedOperation4(result_access_type r, access_type a,
                         arg1_access_type a1, arg2_access_type a2, arg3_access_type a3)
        : retAccess(r), access(a), arg1Access(a1), arg2Access(a2), arg3Access(a3) {}

    void execute(size_t start, size_t end)
    {
        for (size_t i = start; i < end; ++i)
        {
            retAccess[i] = Op::apply(access[i], arg1Access[i], arg2Access[i], arg3Access[i]);
        }
    }
};

template <class Op, class result_access_type, class access_type,
                    class arg1_access_type, class arg2_access_type, class arg3_access_type, class arg4_access_type>
struct VectorizedOperation5 : public Task
{
    result_access_type retAccess;
    access_type        access;
    arg1_access_type   arg1Access;
    arg2_access_type   arg2Access;
    arg3_access_type   arg3Access;
    arg4_access_type   arg4Access;

    VectorizedOperation5(result_access_type r, access_type a,
                         arg1_access_type a1, arg2_access_type a2, arg3_access_type a3, arg4_access_type a4)
        : retAccess(r), access(a), arg1Access(a1), arg2Access(a2), arg3Access(a3), arg4Access(a4) {}

    void execute(size_t start, size_t end)
    {
        for (size_t i = start; i < end; ++i)
        {
            retAccess[i] = Op::apply(access[i], arg1Access[i], arg2Access[i], arg3Access[i], arg4Access[i]);
        }
    }
};

template <class Op, class Vectorize, class Func>
struct VectorizedFunction1 {
    BOOST_STATIC_ASSERT((size<Vectorize>::value == function_traits<Func>::arity));

    typedef function_traits<Func> traits;
    typedef typename fold<Vectorize,false_,or_<_,_> >::type any_vectorized;

    typedef typename result_access_type<typename traits::result_type,
                                        any_vectorized>::type result_type;
    typedef typename result_access_type<typename traits::result_type,
                                        any_vectorized>::direct result_access_type;
    // Result array is created here 'from scratch', so is always 'direct' access.

    typedef typename argument_access_type<typename traits::arg1_type,
                              typename at<Vectorize,long_<0> >::type>::type arg1_type;
    typedef typename argument_access_type<typename traits::arg1_type,
                              typename at<Vectorize,long_<0> >::type>::direct arg1_direct_access_type;
    typedef typename argument_access_type<typename traits::arg1_type,
                              typename at<Vectorize,long_<0> >::type>::masked arg1_masked_access_type;

    static result_type
    apply(arg1_type arg1)
    {
        PY_IMATH_LEAVE_PYTHON;
        size_t len = measure_arguments(arg1);
        op_precompute<Op>::apply(len);
        result_type retval = create_uninitalized_return_value<result_type>::apply(len);

        result_access_type resultAccess = getArrayAccess<result_access_type> (retval);

        if (any_masked(arg1))
        {
            arg1_masked_access_type argAccess =
                 getArrayAccess<arg1_masked_access_type> (arg1);

            VectorizedOperation1<Op,result_access_type,arg1_masked_access_type>
                vop (resultAccess, argAccess);
            dispatchTask(vop,len);
        }
        else
        {
            arg1_direct_access_type argAccess =
                 getArrayAccess<arg1_direct_access_type> (arg1);

            VectorizedOperation1<Op,result_access_type,arg1_direct_access_type>
                vop (resultAccess, argAccess);
            dispatchTask(vop,len);
        }

        PY_IMATH_RETURN_PYTHON;        
        return retval;
    }

    static std::string
    format_arguments(const boost::python::detail::keywords<1> &args)
    {
        // TODO: add types here
        return std::string("(")+args.elements[0].name+") - ";
    }
};

template <class Op, class Vectorize, class Func>
struct VectorizedFunction2 {
    BOOST_STATIC_ASSERT((size<Vectorize>::value == function_traits<Func>::arity));

    typedef function_traits<Func> traits;
    typedef typename fold<Vectorize,false_,or_<_,_> >::type any_vectorized;

    typedef typename result_access_type<typename traits::result_type,
                                        any_vectorized>::type result_type;
    typedef typename result_access_type<typename traits::result_type,
                                        any_vectorized>::direct result_access_type;
    // Result array is created here 'from scratch', so is always 'direct' access.

    typedef typename argument_access_type<typename traits::arg1_type,
                              typename at<Vectorize,long_<0> >::type>::type arg1_type;
    typedef typename argument_access_type<typename traits::arg1_type,
                              typename at<Vectorize,long_<0> >::type>::direct arg1_direct_access_type;
    typedef typename argument_access_type<typename traits::arg1_type,
                              typename at<Vectorize,long_<0> >::type>::masked arg1_masked_access_type;

    typedef typename argument_access_type<typename traits::arg2_type,
                              typename at<Vectorize,long_<1> >::type>::type arg2_type;
    typedef typename argument_access_type<typename traits::arg2_type,
                              typename at<Vectorize,long_<1> >::type>::direct arg2_direct_access_type;
    typedef typename argument_access_type<typename traits::arg2_type,
                              typename at<Vectorize,long_<1> >::type>::masked arg2_masked_access_type;

    static result_type
    apply(arg1_type arg1, arg2_type arg2)
    {
        PY_IMATH_LEAVE_PYTHON;
        size_t len = measure_arguments(arg1,arg2);
        op_precompute<Op>::apply(len);
        result_type retval = create_uninitalized_return_value<result_type>::apply(len);

        result_access_type resultAccess = getArrayAccess<result_access_type> (retval);

        if (any_masked(arg1))
        {
            arg1_masked_access_type arg1Access =
                 getArrayAccess<arg1_masked_access_type> (arg1);

            if (any_masked(arg2))
            {
                arg2_masked_access_type arg2Access =
                     getArrayAccess<arg2_masked_access_type> (arg2);

                VectorizedOperation2<Op,result_access_type,
                                   arg1_masked_access_type,
                                   arg2_masked_access_type>
                    vop (resultAccess, arg1Access, arg2Access);
                dispatchTask(vop,len);
            }
            else
            {
                arg2_direct_access_type arg2Access =
                     getArrayAccess<arg2_direct_access_type> (arg2);

                VectorizedOperation2<Op,result_access_type,
                                   arg1_masked_access_type,
                                   arg2_direct_access_type>
                    vop (resultAccess, arg1Access, arg2Access);
                dispatchTask(vop,len);
            }
        }
        else
        {
            arg1_direct_access_type arg1Access =
                 getArrayAccess<arg1_direct_access_type> (arg1);

            if (any_masked(arg2))
            {
                arg2_masked_access_type arg2Access =
                     getArrayAccess<arg2_masked_access_type> (arg2);

                VectorizedOperation2<Op,result_access_type,
                                   arg1_direct_access_type,
                                   arg2_masked_access_type>
                    vop (resultAccess, arg1Access, arg2Access);
                dispatchTask(vop,len);
            }
            else
            {
                arg2_direct_access_type arg2Access =
                     getArrayAccess<arg2_direct_access_type> (arg2);

                VectorizedOperation2<Op,result_access_type,
                                   arg1_direct_access_type,
                                   arg2_direct_access_type>
                    vop (resultAccess, arg1Access, arg2Access);
                dispatchTask(vop,len);
            }
        }

        PY_IMATH_RETURN_PYTHON;        
        return retval;
    }

    static std::string
    format_arguments(const boost::python::detail::keywords<2> &args)
    {
        // TODO: add types here
        return std::string("(")+args.elements[0].name+","+args.elements[1].name+") - ";
    }
};

template <class Op, class Vectorize, class Func>
struct VectorizedFunction3 {
    BOOST_STATIC_ASSERT((size<Vectorize>::value == function_traits<Func>::arity));

    typedef function_traits<Func> traits;
    typedef typename fold<Vectorize,false_,or_<_,_> >::type any_vectorized;

    typedef typename result_access_type<typename traits::result_type,
                                        any_vectorized>::type result_type;
    typedef typename result_access_type<typename traits::result_type,
                                        any_vectorized>::direct result_access_type;
    // Result array is created here 'from scratch', so is always 'direct' access.

    typedef typename argument_access_type<typename traits::arg1_type,
                              typename at<Vectorize,long_<0> >::type>::type arg1_type;
    typedef typename argument_access_type<typename traits::arg1_type,
                              typename at<Vectorize,long_<0> >::type>::direct arg1_direct_access_type;
    typedef typename argument_access_type<typename traits::arg1_type,
                              typename at<Vectorize,long_<0> >::type>::masked arg1_masked_access_type;

    typedef typename argument_access_type<typename traits::arg2_type,
                              typename at<Vectorize,long_<1> >::type>::type arg2_type;
    typedef typename argument_access_type<typename traits::arg2_type,
                              typename at<Vectorize,long_<1> >::type>::direct arg2_direct_access_type;
    typedef typename argument_access_type<typename traits::arg2_type,
                              typename at<Vectorize,long_<1> >::type>::masked arg2_masked_access_type;

    typedef typename argument_access_type<typename traits::arg3_type,
                              typename at<Vectorize,long_<2> >::type>::type arg3_type;
    typedef typename argument_access_type<typename traits::arg3_type,
                              typename at<Vectorize,long_<2> >::type>::direct arg3_direct_access_type;
    typedef typename argument_access_type<typename traits::arg3_type,
                              typename at<Vectorize,long_<2> >::type>::masked arg3_masked_access_type;

    static result_type
    apply(arg1_type arg1, arg2_type arg2, arg3_type arg3)
    {
        PY_IMATH_LEAVE_PYTHON;
        size_t len = measure_arguments(arg1,arg2,arg3);
        op_precompute<Op>::apply(len);
        result_type retval = create_uninitalized_return_value<result_type>::apply(len);

        result_access_type resultAccess = getArrayAccess<result_access_type> (retval);

        if (any_masked(arg1))
        {
            arg1_masked_access_type arg1Access =
                 getArrayAccess<arg1_masked_access_type> (arg1);

            if (any_masked(arg2))
            {
                arg2_masked_access_type arg2Access =
                     getArrayAccess<arg2_masked_access_type> (arg2);

                if (any_masked(arg3))
                {
                    arg3_masked_access_type arg3Access =
                         getArrayAccess<arg3_masked_access_type> (arg3);

                    VectorizedOperation3<Op,result_access_type,
                                       arg1_masked_access_type,
                                       arg2_masked_access_type,
                                       arg3_masked_access_type>
                        vop (resultAccess, arg1Access, arg2Access, arg3Access);
                    dispatchTask(vop,len);
                }
                else
                {
                    arg3_direct_access_type arg3Access =
                         getArrayAccess<arg3_direct_access_type> (arg3);

                    VectorizedOperation3<Op,result_access_type,
                                       arg1_masked_access_type,
                                       arg2_masked_access_type,
                                       arg3_direct_access_type>
                        vop (resultAccess, arg1Access, arg2Access, arg3Access);
                    dispatchTask(vop,len);
                }
            }
            else
            {
                arg2_direct_access_type arg2Access =
                     getArrayAccess<arg2_direct_access_type> (arg2);

                if (any_masked(arg3))
                {
                    arg3_masked_access_type arg3Access =
                         getArrayAccess<arg3_masked_access_type> (arg3);

                    VectorizedOperation3<Op,result_access_type,
                                       arg1_masked_access_type,
                                       arg2_direct_access_type,
                                       arg3_masked_access_type>
                        vop (resultAccess, arg1Access, arg2Access, arg3Access);
                    dispatchTask(vop,len);
                }
                else
                {
                    arg3_direct_access_type arg3Access =
                         getArrayAccess<arg3_direct_access_type> (arg3);

                    VectorizedOperation3<Op,result_access_type,
                                       arg1_masked_access_type,
                                       arg2_direct_access_type,
                                       arg3_direct_access_type>
                        vop (resultAccess, arg1Access, arg2Access, arg3Access);
                    dispatchTask(vop,len);
                }
            }
        }
        else
        {
            arg1_direct_access_type arg1Access =
                 getArrayAccess<arg1_direct_access_type> (arg1);

            if (any_masked(arg2))
            {
                arg2_masked_access_type arg2Access =
                     getArrayAccess<arg2_masked_access_type> (arg2);

                if (any_masked(arg3))
                {
                    arg3_masked_access_type arg3Access =
                         getArrayAccess<arg3_masked_access_type> (arg3);

                    VectorizedOperation3<Op,result_access_type,
                                       arg1_direct_access_type,
                                       arg2_masked_access_type,
                                       arg3_masked_access_type>
                        vop (resultAccess, arg1Access, arg2Access, arg3Access);
                    dispatchTask(vop,len);
                }
                else
                {
                    arg3_direct_access_type arg3Access =
                         getArrayAccess<arg3_direct_access_type> (arg3);

                    VectorizedOperation3<Op,result_access_type,
                                       arg1_direct_access_type,
                                       arg2_masked_access_type,
                                       arg3_direct_access_type>
                        vop (resultAccess, arg1Access, arg2Access, arg3Access);
                    dispatchTask(vop,len);
                }
            }
            else
            {
                arg2_direct_access_type arg2Access =
                     getArrayAccess<arg2_direct_access_type> (arg2);

                if (any_masked(arg3))
                {
                    arg3_masked_access_type arg3Access =
                         getArrayAccess<arg3_masked_access_type> (arg3);

                    VectorizedOperation3<Op,result_access_type,
                                       arg1_direct_access_type,
                                       arg2_direct_access_type,
                                       arg3_masked_access_type>
                        vop (resultAccess, arg1Access, arg2Access, arg3Access);
                    dispatchTask(vop,len);
                }
                else
                {
                    arg3_direct_access_type arg3Access =
                         getArrayAccess<arg3_direct_access_type> (arg3);

                    VectorizedOperation3<Op,result_access_type,
                                       arg1_direct_access_type,
                                       arg2_direct_access_type,
                                       arg3_direct_access_type>
                        vop (resultAccess, arg1Access, arg2Access, arg3Access);
                    dispatchTask(vop,len);
                }
            }
        }

        PY_IMATH_RETURN_PYTHON;
        return retval;
    }

    static std::string
    format_arguments(const boost::python::detail::keywords<3> &args)
    {
        // TODO: add types here
        return std::string("(")+args.elements[0].name+","+args.elements[1].name+","+args.elements[2].name+") - ";
    }
};

template <class Op, class Vectorize, class Func>
struct VectorizedFunction4 {
    BOOST_STATIC_ASSERT((size<Vectorize>::value == function_traits<Func>::arity));

    typedef function_traits<Func> traits;
    typedef typename fold<Vectorize,false_,or_<_,_> >::type any_vectorized;

    typedef typename result_access_type<typename traits::result_type,
                                        any_vectorized>::type result_type;
    typedef typename result_access_type<typename traits::result_type,
                                        any_vectorized>::direct result_access_type;
    // Result array is created here 'from scratch', so is always 'direct' access.

    typedef typename argument_access_type<typename traits::arg1_type,
                              typename at<Vectorize,long_<0> >::type>::type arg1_type;
    typedef typename argument_access_type<typename traits::arg1_type,
                              typename at<Vectorize,long_<0> >::type>::direct arg1_direct_access_type;
    typedef typename argument_access_type<typename traits::arg1_type,
                              typename at<Vectorize,long_<0> >::type>::masked arg1_masked_access_type;

    typedef typename argument_access_type<typename traits::arg2_type,
                              typename at<Vectorize,long_<1> >::type>::type arg2_type;
    typedef typename argument_access_type<typename traits::arg2_type,
                              typename at<Vectorize,long_<1> >::type>::direct arg2_direct_access_type;
    typedef typename argument_access_type<typename traits::arg2_type,
                              typename at<Vectorize,long_<1> >::type>::masked arg2_masked_access_type;

    typedef typename argument_access_type<typename traits::arg3_type,
                              typename at<Vectorize,long_<2> >::type>::type arg3_type;
    typedef typename argument_access_type<typename traits::arg3_type,
                              typename at<Vectorize,long_<2> >::type>::direct arg3_direct_access_type;
    typedef typename argument_access_type<typename traits::arg3_type,
                              typename at<Vectorize,long_<2> >::type>::masked arg3_masked_access_type;

    typedef typename argument_access_type<typename traits::arg4_type,
                              typename at<Vectorize,long_<3> >::type>::type arg4_type;
    typedef typename argument_access_type<typename traits::arg4_type,
                              typename at<Vectorize,long_<3> >::type>::direct arg4_direct_access_type;
    typedef typename argument_access_type<typename traits::arg4_type,
                              typename at<Vectorize,long_<3> >::type>::masked arg4_masked_access_type;

    static result_type
    apply(arg1_type arg1, arg2_type arg2, arg3_type arg3, arg4_type arg4)
    {
        PY_IMATH_LEAVE_PYTHON;
        size_t len = measure_arguments(arg1,arg2,arg3,arg4);
        op_precompute<Op>::apply(len);
        result_type retval = create_uninitalized_return_value<result_type>::apply(len);

        result_access_type resultAccess = getArrayAccess<result_access_type> (retval);

        if (any_masked(arg1))
        {
            arg1_masked_access_type arg1Access =
                 getArrayAccess<arg1_masked_access_type> (arg1);

            if (any_masked(arg2))
            {
                arg2_masked_access_type arg2Access =
                     getArrayAccess<arg2_masked_access_type> (arg2);

                if (any_masked(arg3))
                {
                    arg3_masked_access_type arg3Access =
                         getArrayAccess<arg3_masked_access_type> (arg3);

                    if (any_masked(arg4))
                    {
                        arg4_masked_access_type arg4Access =
                            getArrayAccess<arg4_masked_access_type> (arg4);

                        VectorizedOperation4<Op,result_access_type,
                                           arg1_masked_access_type,
                                           arg2_masked_access_type,
                                           arg3_masked_access_type,
                                           arg4_masked_access_type>
                            vop (resultAccess, arg1Access, arg2Access, arg3Access, arg4Access);
                        dispatchTask(vop,len);
                    }
                    else
                    {
                        arg4_direct_access_type arg4Access =
                            getArrayAccess<arg4_direct_access_type> (arg4);

                        VectorizedOperation4<Op,result_access_type,
                                           arg1_masked_access_type,
                                           arg2_masked_access_type,
                                           arg3_masked_access_type,
                                           arg4_direct_access_type>
                            vop (resultAccess, arg1Access, arg2Access, arg3Access, arg4Access);
                        dispatchTask(vop,len);
                    }
                }
                else
                {
                    arg3_direct_access_type arg3Access =
                         getArrayAccess<arg3_direct_access_type> (arg3);

                    if (any_masked(arg4))
                    {
                        arg4_masked_access_type arg4Access =
                            getArrayAccess<arg4_masked_access_type> (arg4);

                        VectorizedOperation4<Op,result_access_type,
                                           arg1_masked_access_type,
                                           arg2_masked_access_type,
                                           arg3_direct_access_type,
                                           arg4_masked_access_type>
                            vop (resultAccess, arg1Access, arg2Access, arg3Access, arg4Access);
                    }
                    else
                    {
                        arg4_direct_access_type arg4Access =
                            getArrayAccess<arg4_direct_access_type> (arg4);

                        VectorizedOperation4<Op,result_access_type,
                                           arg1_masked_access_type,
                                           arg2_masked_access_type,
                                           arg3_direct_access_type,
                                           arg4_direct_access_type>
                            vop (resultAccess, arg1Access, arg2Access, arg3Access, arg4Access);
                        dispatchTask(vop,len);
                    }
                }
            }
            else
            {
                arg2_direct_access_type arg2Access =
                     getArrayAccess<arg2_direct_access_type> (arg2);

                if (any_masked(arg3))
                {
                    arg3_masked_access_type arg3Access =
                         getArrayAccess<arg3_masked_access_type> (arg3);

                    if (any_masked(arg4))
                    {
                        arg4_masked_access_type arg4Access =
                            getArrayAccess<arg4_masked_access_type> (arg4);

                        VectorizedOperation4<Op,result_access_type,
                                           arg1_masked_access_type,
                                           arg2_direct_access_type,
                                           arg3_masked_access_type,
                                           arg4_masked_access_type>
                            vop (resultAccess, arg1Access, arg2Access, arg3Access, arg4Access);
                        dispatchTask(vop,len);
                    }
                    else
                    {
                        arg4_direct_access_type arg4Access =
                            getArrayAccess<arg4_direct_access_type> (arg4);

                        VectorizedOperation4<Op,result_access_type,
                                           arg1_masked_access_type,
                                           arg2_direct_access_type,
                                           arg3_masked_access_type,
                                           arg4_direct_access_type>
                            vop (resultAccess, arg1Access, arg2Access, arg3Access, arg4Access);
                        dispatchTask(vop,len);
                    }
                }
                else
                {
                    arg3_direct_access_type arg3Access =
                         getArrayAccess<arg3_direct_access_type> (arg3);

                    if (any_masked(arg4))
                    {
                        arg4_masked_access_type arg4Access =
                            getArrayAccess<arg4_masked_access_type> (arg4);

                        VectorizedOperation4<Op,result_access_type,
                                           arg1_masked_access_type,
                                           arg2_direct_access_type,
                                           arg3_direct_access_type,
                                           arg4_masked_access_type>
                            vop (resultAccess, arg1Access, arg2Access, arg3Access, arg4Access);
                        dispatchTask(vop,len);
                    }
                    else
                    {
                        arg4_direct_access_type arg4Access =
                            getArrayAccess<arg4_direct_access_type> (arg4);

                        VectorizedOperation4<Op,result_access_type,
                                           arg1_masked_access_type,
                                           arg2_direct_access_type,
                                           arg3_direct_access_type,
                                           arg4_direct_access_type>
                            vop (resultAccess, arg1Access, arg2Access, arg3Access, arg4Access);
                        dispatchTask(vop,len);
                    }
                }
            }
        }
        else
        {
            arg1_direct_access_type arg1Access =
                 getArrayAccess<arg1_direct_access_type> (arg1);

            if (any_masked(arg2))
            {
                arg2_masked_access_type arg2Access =
                     getArrayAccess<arg2_masked_access_type> (arg2);

                if (any_masked(arg3))
                {
                    arg3_masked_access_type arg3Access =
                         getArrayAccess<arg3_masked_access_type> (arg3);

                    if (any_masked(arg4))
                    {
                        arg4_masked_access_type arg4Access =
                            getArrayAccess<arg4_masked_access_type> (arg4);

                        VectorizedOperation4<Op,result_access_type,
                                           arg1_direct_access_type,
                                           arg2_masked_access_type,
                                           arg3_masked_access_type,
                                           arg4_masked_access_type>
                            vop (resultAccess, arg1Access, arg2Access, arg3Access, arg4Access);
                        dispatchTask(vop,len);
                    }
                    else
                    {
                        arg4_direct_access_type arg4Access =
                            getArrayAccess<arg4_direct_access_type> (arg4);

                        VectorizedOperation4<Op,result_access_type,
                                           arg1_direct_access_type,
                                           arg2_masked_access_type,
                                           arg3_masked_access_type,
                                           arg4_direct_access_type>
                            vop (resultAccess, arg1Access, arg2Access, arg3Access, arg4Access);
                        dispatchTask(vop,len);
                    }
                }
                else
                {
                    arg3_direct_access_type arg3Access =
                         getArrayAccess<arg3_direct_access_type> (arg3);

                    if (any_masked(arg4))
                    {
                        arg4_masked_access_type arg4Access =
                            getArrayAccess<arg4_masked_access_type> (arg4);

                        VectorizedOperation4<Op,result_access_type,
                                           arg1_direct_access_type,
                                           arg2_masked_access_type,
                                           arg3_direct_access_type,
                                           arg4_masked_access_type>
                            vop (resultAccess, arg1Access, arg2Access, arg3Access, arg4Access);
                        dispatchTask(vop,len);
                    }
                    else
                    {
                        arg4_direct_access_type arg4Access =
                            getArrayAccess<arg4_direct_access_type> (arg4);

                        VectorizedOperation4<Op,result_access_type,
                                           arg1_direct_access_type,
                                           arg2_masked_access_type,
                                           arg3_direct_access_type,
                                           arg4_direct_access_type>
                            vop (resultAccess, arg1Access, arg2Access, arg3Access, arg4Access);
                        dispatchTask(vop,len);
                    }
                }
            }
            else
            {
                arg2_direct_access_type arg2Access =
                     getArrayAccess<arg2_direct_access_type> (arg2);

                if (any_masked(arg3))
                {
                    arg3_masked_access_type arg3Access =
                         getArrayAccess<arg3_masked_access_type> (arg3);

                    if (any_masked(arg4))
                    {
                        arg4_masked_access_type arg4Access =
                            getArrayAccess<arg4_masked_access_type> (arg4);

                        VectorizedOperation4<Op,result_access_type,
                                           arg1_direct_access_type,
                                           arg2_direct_access_type,
                                           arg3_masked_access_type,
                                           arg4_masked_access_type>
                            vop (resultAccess, arg1Access, arg2Access, arg3Access, arg4Access);
                        dispatchTask(vop,len);
                    }
                    else
                    {
                        arg4_direct_access_type arg4Access =
                            getArrayAccess<arg4_direct_access_type> (arg4);

                        VectorizedOperation4<Op,result_access_type,
                                           arg1_direct_access_type,
                                           arg2_direct_access_type,
                                           arg3_masked_access_type,
                                           arg4_direct_access_type>
                            vop (resultAccess, arg1Access, arg2Access, arg3Access, arg4Access);
                        dispatchTask(vop,len);
                    }
                }
                else
                {
                    arg3_direct_access_type arg3Access =
                         getArrayAccess<arg3_direct_access_type> (arg3);

                    if (any_masked(arg4))
                    {
                        arg4_masked_access_type arg4Access =
                            getArrayAccess<arg4_masked_access_type> (arg4);

                        VectorizedOperation4<Op,result_access_type,
                                           arg1_direct_access_type,
                                           arg2_direct_access_type,
                                           arg3_direct_access_type,
                                           arg4_masked_access_type>
                            vop (resultAccess, arg1Access, arg2Access, arg3Access, arg4Access);
                        dispatchTask(vop,len);
                    }
                    else
                    {
                        arg4_direct_access_type arg4Access =
                            getArrayAccess<arg4_direct_access_type> (arg4);

                        VectorizedOperation4<Op,result_access_type,
                                           arg1_direct_access_type,
                                           arg2_direct_access_type,
                                           arg3_direct_access_type,
                                           arg4_direct_access_type>
                            vop (resultAccess, arg1Access, arg2Access, arg3Access, arg4Access);
                        dispatchTask(vop,len);
                    }
                }
            }
        }

        PY_IMATH_RETURN_PYTHON;
        return retval;
    }

    static std::string
    format_arguments(const boost::python::detail::keywords<4> &args)
    {
        // TODO: add types here
        return std::string("(")+args.elements[0].name+","+args.elements[1].name+","+args.elements[2].name+","+args.elements[3].name+") - ";
    }
};

template <class Op, class Vectorize, class Func>
struct VectorizedFunction5 {
    BOOST_STATIC_ASSERT((size<Vectorize>::value == function_traits<Func>::arity));

    typedef function_traits<Func> traits;
    typedef typename fold<Vectorize,false_,or_<_,_> >::type any_vectorized;

    typedef typename result_access_type<typename traits::result_type,
                                        any_vectorized>::type result_type;
    typedef typename result_access_type<typename traits::result_type,
                                        any_vectorized>::direct result_access_type;
    // Result array is created here 'from scratch', so is always 'direct' access.

    typedef typename argument_access_type<typename traits::arg1_type,
                              typename at<Vectorize,long_<0> >::type>::type arg1_type;
    typedef typename argument_access_type<typename traits::arg1_type,
                              typename at<Vectorize,long_<0> >::type>::direct arg1_direct_access_type;
    typedef typename argument_access_type<typename traits::arg1_type,
                              typename at<Vectorize,long_<0> >::type>::masked arg1_masked_access_type;

    typedef typename argument_access_type<typename traits::arg2_type,
                              typename at<Vectorize,long_<1> >::type>::type arg2_type;
    typedef typename argument_access_type<typename traits::arg2_type,
                              typename at<Vectorize,long_<1> >::type>::direct arg2_direct_access_type;
    typedef typename argument_access_type<typename traits::arg2_type,
                              typename at<Vectorize,long_<1> >::type>::masked arg2_masked_access_type;

    typedef typename argument_access_type<typename traits::arg3_type,
                              typename at<Vectorize,long_<2> >::type>::type arg3_type;
    typedef typename argument_access_type<typename traits::arg3_type,
                              typename at<Vectorize,long_<2> >::type>::direct arg3_direct_access_type;
    typedef typename argument_access_type<typename traits::arg3_type,
                              typename at<Vectorize,long_<2> >::type>::masked arg3_masked_access_type;

    typedef typename argument_access_type<typename traits::arg4_type,
                              typename at<Vectorize,long_<3> >::type>::type arg4_type;
    typedef typename argument_access_type<typename traits::arg4_type,
                              typename at<Vectorize,long_<3> >::type>::direct arg4_direct_access_type;
    typedef typename argument_access_type<typename traits::arg4_type,
                              typename at<Vectorize,long_<3> >::type>::masked arg4_masked_access_type;

    typedef typename argument_access_type<typename traits::arg5_type,
                              typename at<Vectorize,long_<4> >::type>::type arg5_type;
    typedef typename argument_access_type<typename traits::arg5_type,
                              typename at<Vectorize,long_<4> >::type>::direct arg5_direct_access_type;
    typedef typename argument_access_type<typename traits::arg5_type,
                              typename at<Vectorize,long_<4> >::type>::masked arg5_masked_access_type;

    static result_type
    apply(arg1_type arg1, arg2_type arg2, arg3_type arg3, arg4_type arg4, arg5_type arg5)
    {
        PY_IMATH_LEAVE_PYTHON;
        size_t len = measure_arguments(arg1,arg2,arg3,arg4,arg5);
        op_precompute<Op>::apply(len);
        result_type retval = create_uninitalized_return_value<result_type>::apply(len);

        result_access_type resultAccess = getArrayAccess<result_access_type> (retval);

        if (any_masked(arg1))
        {
            arg1_masked_access_type arg1Access =
                 getArrayAccess<arg1_masked_access_type> (arg1);

            if (any_masked(arg2))
            {
                arg2_masked_access_type arg2Access =
                     getArrayAccess<arg2_masked_access_type> (arg2);

                if (any_masked(arg3))
                {
                    arg3_masked_access_type arg3Access =
                         getArrayAccess<arg3_masked_access_type> (arg3);

                    if (any_masked(arg4))
                    {
                        arg4_masked_access_type arg4Access =
                            getArrayAccess<arg4_masked_access_type> (arg4);

                        if (any_masked(arg5))
                        {
                            arg5_masked_access_type arg5Access =
                                getArrayAccess<arg5_masked_access_type> (arg5);

                            VectorizedOperation5<Op,result_access_type,
                                               arg1_masked_access_type,
                                               arg2_masked_access_type,
                                               arg3_masked_access_type,
                                               arg4_masked_access_type,
                                               arg5_masked_access_type>
                                vop (resultAccess, arg1Access, arg2Access, arg3Access, arg4Access, arg5Access);
                            dispatchTask(vop,len);
                        }
                        else
                        {
                            arg5_direct_access_type arg5Access =
                                getArrayAccess<arg5_direct_access_type> (arg5);

                            VectorizedOperation5<Op,result_access_type,
                                               arg1_masked_access_type,
                                               arg2_masked_access_type,
                                               arg3_masked_access_type,
                                               arg4_masked_access_type,
                                               arg5_direct_access_type>
                                vop (resultAccess, arg1Access, arg2Access, arg3Access, arg4Access, arg5Access);
                            dispatchTask(vop,len);
                        }
                    }
                    else
                    {
                        arg4_direct_access_type arg4Access =
                            getArrayAccess<arg4_direct_access_type> (arg4);

                        if (any_masked(arg5))
                        {
                            arg5_masked_access_type arg5Access =
                                getArrayAccess<arg5_masked_access_type> (arg5);

                            VectorizedOperation5<Op,result_access_type,
                                               arg1_masked_access_type,
                                               arg2_masked_access_type,
                                               arg3_masked_access_type,
                                               arg4_direct_access_type,
                                               arg5_masked_access_type>
                                vop (resultAccess, arg1Access, arg2Access, arg3Access, arg4Access, arg5Access);
                            dispatchTask(vop,len);
                        }
                        else
                        {
                            arg5_direct_access_type arg5Access =
                                getArrayAccess<arg5_direct_access_type> (arg5);

                            VectorizedOperation5<Op,result_access_type,
                                               arg1_masked_access_type,
                                               arg2_masked_access_type,
                                               arg3_masked_access_type,
                                               arg4_direct_access_type,
                                               arg5_direct_access_type>
                                vop (resultAccess, arg1Access, arg2Access, arg3Access, arg4Access, arg5Access);
                            dispatchTask(vop,len);
                        }
                    }
                }
                else
                {
                    arg3_direct_access_type arg3Access =
                         getArrayAccess<arg3_direct_access_type> (arg3);

                    if (any_masked(arg4))
                    {
                        arg4_masked_access_type arg4Access =
                            getArrayAccess<arg4_masked_access_type> (arg4);

                        if (any_masked(arg5))
                        {
                            arg5_masked_access_type arg5Access =
                                getArrayAccess<arg5_masked_access_type> (arg5);

                            VectorizedOperation5<Op,result_access_type,
                                               arg1_masked_access_type,
                                               arg2_masked_access_type,
                                               arg3_direct_access_type,
                                               arg4_masked_access_type,
                                               arg5_masked_access_type>
                                vop (resultAccess, arg1Access, arg2Access, arg3Access, arg4Access, arg5Access);
                            dispatchTask(vop,len);
                        }
                        else
                        {
                            arg5_direct_access_type arg5Access =
                                getArrayAccess<arg5_direct_access_type> (arg5);

                            VectorizedOperation5<Op,result_access_type,
                                               arg1_masked_access_type,
                                               arg2_masked_access_type,
                                               arg3_direct_access_type,
                                               arg4_masked_access_type,
                                               arg5_direct_access_type>
                                vop (resultAccess, arg1Access, arg2Access, arg3Access, arg4Access, arg5Access);
                            dispatchTask(vop,len);
                        }
                    }
                    else
                    {
                        arg4_direct_access_type arg4Access =
                            getArrayAccess<arg4_direct_access_type> (arg4);

                        if (any_masked(arg5))
                        {
                            arg5_masked_access_type arg5Access =
                                getArrayAccess<arg5_masked_access_type> (arg5);

                            VectorizedOperation5<Op,result_access_type,
                                               arg1_masked_access_type,
                                               arg2_masked_access_type,
                                               arg3_direct_access_type,
                                               arg4_direct_access_type,
                                               arg5_masked_access_type>
                                vop (resultAccess, arg1Access, arg2Access, arg3Access, arg4Access, arg5Access);
                            dispatchTask(vop,len);
                        }
                        else
                        {
                            arg5_direct_access_type arg5Access =
                                getArrayAccess<arg5_direct_access_type> (arg5);

                            VectorizedOperation5<Op,result_access_type,
                                               arg1_masked_access_type,
                                               arg2_masked_access_type,
                                               arg3_direct_access_type,
                                               arg4_direct_access_type,
                                               arg5_direct_access_type>
                                vop (resultAccess, arg1Access, arg2Access, arg3Access, arg4Access, arg5Access);
                            dispatchTask(vop,len);
                        }
                    }
                }
            }
            else
            {
                arg2_direct_access_type arg2Access =
                     getArrayAccess<arg2_direct_access_type> (arg2);

                if (any_masked(arg3))
                {
                    arg3_masked_access_type arg3Access =
                         getArrayAccess<arg3_masked_access_type> (arg3);

                    if (any_masked(arg4))
                    {
                        arg4_masked_access_type arg4Access =
                            getArrayAccess<arg4_masked_access_type> (arg4);

                        if (any_masked(arg5))
                        {
                            arg5_masked_access_type arg5Access =
                                getArrayAccess<arg5_masked_access_type> (arg5);

                            VectorizedOperation5<Op,result_access_type,
                                               arg1_masked_access_type,
                                               arg2_direct_access_type,
                                               arg3_masked_access_type,
                                               arg4_masked_access_type,
                                               arg5_masked_access_type>
                                vop (resultAccess, arg1Access, arg2Access, arg3Access, arg4Access, arg5Access);
                            dispatchTask(vop,len);
                        }
                        else
                        {
                            arg5_direct_access_type arg5Access =
                                getArrayAccess<arg5_direct_access_type> (arg5);

                            VectorizedOperation5<Op,result_access_type,
                                               arg1_masked_access_type,
                                               arg2_direct_access_type,
                                               arg3_masked_access_type,
                                               arg4_masked_access_type,
                                               arg5_direct_access_type>
                                vop (resultAccess, arg1Access, arg2Access, arg3Access, arg4Access, arg5Access);
                            dispatchTask(vop,len);
                        }
                    }
                    else
                    {
                        arg4_direct_access_type arg4Access =
                            getArrayAccess<arg4_direct_access_type> (arg4);

                        if (any_masked(arg5))
                        {
                            arg5_masked_access_type arg5Access =
                                getArrayAccess<arg5_masked_access_type> (arg5);

                            VectorizedOperation5<Op,result_access_type,
                                               arg1_masked_access_type,
                                               arg2_direct_access_type,
                                               arg3_masked_access_type,
                                               arg4_direct_access_type,
                                               arg5_masked_access_type>
                                vop (resultAccess, arg1Access, arg2Access, arg3Access, arg4Access, arg5Access);
                            dispatchTask(vop,len);
                        }
                        else
                        {
                            arg5_direct_access_type arg5Access =
                                getArrayAccess<arg5_direct_access_type> (arg5);

                            VectorizedOperation5<Op,result_access_type,
                                               arg1_masked_access_type,
                                               arg2_direct_access_type,
                                               arg3_masked_access_type,
                                               arg4_direct_access_type,
                                               arg5_direct_access_type>
                                vop (resultAccess, arg1Access, arg2Access, arg3Access, arg4Access, arg5Access);
                            dispatchTask(vop,len);
                        }
                    }
                }
                else
                {
                    arg3_direct_access_type arg3Access =
                         getArrayAccess<arg3_direct_access_type> (arg3);

                    if (any_masked(arg4))
                    {
                        arg4_masked_access_type arg4Access =
                            getArrayAccess<arg4_masked_access_type> (arg4);

                        if (any_masked(arg5))
                        {
                            arg5_masked_access_type arg5Access =
                                getArrayAccess<arg5_masked_access_type> (arg5);

                            VectorizedOperation5<Op,result_access_type,
                                               arg1_masked_access_type,
                                               arg2_direct_access_type,
                                               arg3_direct_access_type,
                                               arg4_masked_access_type,
                                               arg5_masked_access_type>
                                vop (resultAccess, arg1Access, arg2Access, arg3Access, arg4Access, arg5Access);
                            dispatchTask(vop,len);
                        }
                        else
                        {
                            arg5_direct_access_type arg5Access =
                                getArrayAccess<arg5_direct_access_type> (arg5);

                            VectorizedOperation5<Op,result_access_type,
                                               arg1_masked_access_type,
                                               arg2_direct_access_type,
                                               arg3_direct_access_type,
                                               arg4_masked_access_type,
                                               arg5_direct_access_type>
                                vop (resultAccess, arg1Access, arg2Access, arg3Access, arg4Access, arg5Access);
                            dispatchTask(vop,len);
                        }
                    }
                    else
                    {
                        arg4_direct_access_type arg4Access =
                            getArrayAccess<arg4_direct_access_type> (arg4);

                        if (any_masked(arg5))
                        {
                            arg5_masked_access_type arg5Access =
                                getArrayAccess<arg5_masked_access_type> (arg5);

                            VectorizedOperation5<Op,result_access_type,
                                               arg1_masked_access_type,
                                               arg2_direct_access_type,
                                               arg3_direct_access_type,
                                               arg4_direct_access_type,
                                               arg5_masked_access_type>
                                vop (resultAccess, arg1Access, arg2Access, arg3Access, arg4Access, arg5Access);
                            dispatchTask(vop,len);
                        }
                        else
                        {
                            arg5_direct_access_type arg5Access =
                                getArrayAccess<arg5_direct_access_type> (arg5);

                            VectorizedOperation5<Op,result_access_type,
                                               arg1_masked_access_type,
                                               arg2_direct_access_type,
                                               arg3_direct_access_type,
                                               arg4_direct_access_type,
                                               arg5_direct_access_type>
                                vop (resultAccess, arg1Access, arg2Access, arg3Access, arg4Access, arg5Access);
                            dispatchTask(vop,len);
                        }
                    }
                }
            }
        }
        else
        {
            arg1_direct_access_type arg1Access =
                 getArrayAccess<arg1_direct_access_type> (arg1);

            if (any_masked(arg2))
            {
                arg2_masked_access_type arg2Access =
                     getArrayAccess<arg2_masked_access_type> (arg2);

                if (any_masked(arg3))
                {
                    arg3_masked_access_type arg3Access =
                         getArrayAccess<arg3_masked_access_type> (arg3);

                    if (any_masked(arg4))
                    {
                        arg4_masked_access_type arg4Access =
                            getArrayAccess<arg4_masked_access_type> (arg4);

                        if (any_masked(arg5))
                        {
                            arg5_masked_access_type arg5Access =
                                getArrayAccess<arg5_masked_access_type> (arg5);

                            VectorizedOperation5<Op,result_access_type,
                                               arg1_direct_access_type,
                                               arg2_masked_access_type,
                                               arg3_masked_access_type,
                                               arg4_masked_access_type,
                                               arg5_masked_access_type>
                                vop (resultAccess, arg1Access, arg2Access, arg3Access, arg4Access, arg5Access);
                            dispatchTask(vop,len);
                        }
                        else
                        {
                            arg5_direct_access_type arg5Access =
                                getArrayAccess<arg5_direct_access_type> (arg5);

                            VectorizedOperation5<Op,result_access_type,
                                               arg1_direct_access_type,
                                               arg2_masked_access_type,
                                               arg3_masked_access_type,
                                               arg4_masked_access_type,
                                               arg5_direct_access_type>
                                vop (resultAccess, arg1Access, arg2Access, arg3Access, arg4Access, arg5Access);
                            dispatchTask(vop,len);
                        }
                    }
                    else
                    {
                        arg4_direct_access_type arg4Access =
                            getArrayAccess<arg4_direct_access_type> (arg4);

                        if (any_masked(arg5))
                        {
                            arg5_masked_access_type arg5Access =
                                getArrayAccess<arg5_masked_access_type> (arg5);

                            VectorizedOperation5<Op,result_access_type,
                                               arg1_direct_access_type,
                                               arg2_masked_access_type,
                                               arg3_masked_access_type,
                                               arg4_direct_access_type,
                                               arg5_masked_access_type>
                                vop (resultAccess, arg1Access, arg2Access, arg3Access, arg4Access, arg5Access);
                            dispatchTask(vop,len);
                        }
                        else
                        {
                            arg5_direct_access_type arg5Access =
                                getArrayAccess<arg5_direct_access_type> (arg5);

                            VectorizedOperation5<Op,result_access_type,
                                               arg1_direct_access_type,
                                               arg2_masked_access_type,
                                               arg3_masked_access_type,
                                               arg4_direct_access_type,
                                               arg5_direct_access_type>
                                vop (resultAccess, arg1Access, arg2Access, arg3Access, arg4Access, arg5Access);
                            dispatchTask(vop,len);
                        }
                    }
                }
                else
                {
                    arg3_direct_access_type arg3Access =
                         getArrayAccess<arg3_direct_access_type> (arg3);

                    if (any_masked(arg4))
                    {
                        arg4_masked_access_type arg4Access =
                            getArrayAccess<arg4_masked_access_type> (arg4);

                        if (any_masked(arg5))
                        {
                            arg5_masked_access_type arg5Access =
                                getArrayAccess<arg5_masked_access_type> (arg5);

                            VectorizedOperation5<Op,result_access_type,
                                               arg1_direct_access_type,
                                               arg2_masked_access_type,
                                               arg3_direct_access_type,
                                               arg4_masked_access_type,
                                               arg5_masked_access_type>
                                vop (resultAccess, arg1Access, arg2Access, arg3Access, arg4Access, arg5Access);
                            dispatchTask(vop,len);
                        }
                        else
                        {
                            arg5_direct_access_type arg5Access =
                                getArrayAccess<arg5_direct_access_type> (arg5);

                            VectorizedOperation5<Op,result_access_type,
                                               arg1_direct_access_type,
                                               arg2_masked_access_type,
                                               arg3_direct_access_type,
                                               arg4_masked_access_type,
                                               arg5_direct_access_type>
                                vop (resultAccess, arg1Access, arg2Access, arg3Access, arg4Access, arg5Access);
                            dispatchTask(vop,len);
                        }
                    }
                    else
                    {
                        arg4_direct_access_type arg4Access =
                            getArrayAccess<arg4_direct_access_type> (arg4);

                        if (any_masked(arg5))
                        {
                            arg5_masked_access_type arg5Access =
                                getArrayAccess<arg5_masked_access_type> (arg5);

                            VectorizedOperation5<Op,result_access_type,
                                               arg1_direct_access_type,
                                               arg2_masked_access_type,
                                               arg3_direct_access_type,
                                               arg4_direct_access_type,
                                               arg5_masked_access_type>
                                vop (resultAccess, arg1Access, arg2Access, arg3Access, arg4Access, arg5Access);
                            dispatchTask(vop,len);
                        }
                        else
                        {
                            arg5_direct_access_type arg5Access =
                                getArrayAccess<arg5_direct_access_type> (arg5);

                            VectorizedOperation5<Op,result_access_type,
                                               arg1_direct_access_type,
                                               arg2_masked_access_type,
                                               arg3_direct_access_type,
                                               arg4_direct_access_type,
                                               arg5_direct_access_type>
                                vop (resultAccess, arg1Access, arg2Access, arg3Access, arg4Access, arg5Access);
                            dispatchTask(vop,len);
                        }
                    }
                }
            }
            else
            {
                arg2_direct_access_type arg2Access =
                     getArrayAccess<arg2_direct_access_type> (arg2);

                if (any_masked(arg3))
                {
                    arg3_masked_access_type arg3Access =
                         getArrayAccess<arg3_masked_access_type> (arg3);

                    if (any_masked(arg4))
                    {
                        arg4_masked_access_type arg4Access =
                            getArrayAccess<arg4_masked_access_type> (arg4);

                        if (any_masked(arg5))
                        {
                            arg5_masked_access_type arg5Access =
                                getArrayAccess<arg5_masked_access_type> (arg5);

                            VectorizedOperation5<Op,result_access_type,
                                               arg1_direct_access_type,
                                               arg2_direct_access_type,
                                               arg3_masked_access_type,
                                               arg4_masked_access_type,
                                               arg5_masked_access_type>
                                vop (resultAccess, arg1Access, arg2Access, arg3Access, arg4Access, arg5Access);
                            dispatchTask(vop,len);
                        }
                        else
                        {
                            arg5_direct_access_type arg5Access =
                                getArrayAccess<arg5_direct_access_type> (arg5);

                            VectorizedOperation5<Op,result_access_type,
                                               arg1_direct_access_type,
                                               arg2_direct_access_type,
                                               arg3_masked_access_type,
                                               arg4_masked_access_type,
                                               arg5_direct_access_type>
                                vop (resultAccess, arg1Access, arg2Access, arg3Access, arg4Access, arg5Access);
                            dispatchTask(vop,len);
                        }
                    }
                    else
                    {
                        arg4_direct_access_type arg4Access =
                            getArrayAccess<arg4_direct_access_type> (arg4);

                        if (any_masked(arg5))
                        {
                            arg5_masked_access_type arg5Access =
                                getArrayAccess<arg5_masked_access_type> (arg5);

                            VectorizedOperation5<Op,result_access_type,
                                               arg1_direct_access_type,
                                               arg2_direct_access_type,
                                               arg3_masked_access_type,
                                               arg4_direct_access_type,
                                               arg5_masked_access_type>
                                vop (resultAccess, arg1Access, arg2Access, arg3Access, arg4Access, arg5Access);
                            dispatchTask(vop,len);
                        }
                        else
                        {
                            arg5_direct_access_type arg5Access =
                                getArrayAccess<arg5_direct_access_type> (arg5);

                            VectorizedOperation5<Op,result_access_type,
                                               arg1_direct_access_type,
                                               arg2_direct_access_type,
                                               arg3_masked_access_type,
                                               arg4_direct_access_type,
                                               arg5_direct_access_type>
                                vop (resultAccess, arg1Access, arg2Access, arg3Access, arg4Access, arg5Access);
                            dispatchTask(vop,len);
                        }
                    }
                }
                else
                {
                    arg3_direct_access_type arg3Access =
                         getArrayAccess<arg3_direct_access_type> (arg3);

                    if (any_masked(arg4))
                    {
                        arg4_masked_access_type arg4Access =
                            getArrayAccess<arg4_masked_access_type> (arg4);

                        if (any_masked(arg5))
                        {
                            arg5_masked_access_type arg5Access =
                                getArrayAccess<arg5_masked_access_type> (arg5);

                            VectorizedOperation5<Op,result_access_type,
                                               arg1_direct_access_type,
                                               arg2_direct_access_type,
                                               arg3_direct_access_type,
                                               arg4_masked_access_type,
                                               arg5_masked_access_type>
                                vop (resultAccess, arg1Access, arg2Access, arg3Access, arg4Access, arg5Access);
                            dispatchTask(vop,len);
                        }
                        else
                        {
                            arg5_direct_access_type arg5Access =
                                getArrayAccess<arg5_direct_access_type> (arg5);

                            VectorizedOperation5<Op,result_access_type,
                                               arg1_direct_access_type,
                                               arg2_direct_access_type,
                                               arg3_direct_access_type,
                                               arg4_masked_access_type,
                                               arg5_direct_access_type>
                                vop (resultAccess, arg1Access, arg2Access, arg3Access, arg4Access, arg5Access);
                            dispatchTask(vop,len);
                        }
                    }
                    else
                    {
                        arg4_direct_access_type arg4Access =
                            getArrayAccess<arg4_direct_access_type> (arg4);

                        if (any_masked(arg5))
                        {
                            arg5_masked_access_type arg5Access =
                                getArrayAccess<arg5_masked_access_type> (arg5);

                            VectorizedOperation5<Op,result_access_type,
                                               arg1_direct_access_type,
                                               arg2_direct_access_type,
                                               arg3_direct_access_type,
                                               arg4_direct_access_type,
                                               arg5_masked_access_type>
                                vop (resultAccess, arg1Access, arg2Access, arg3Access, arg4Access, arg5Access);
                            dispatchTask(vop,len);
                        }
                        else
                        {
                            arg5_direct_access_type arg5Access =
                                getArrayAccess<arg5_direct_access_type> (arg5);

                            VectorizedOperation5<Op,result_access_type,
                                               arg1_direct_access_type,
                                               arg2_direct_access_type,
                                               arg3_direct_access_type,
                                               arg4_direct_access_type,
                                               arg5_direct_access_type>
                                vop (resultAccess, arg1Access, arg2Access, arg3Access, arg4Access, arg5Access);
                            dispatchTask(vop,len);
                        }
                    }
                }
            }
        }

        PY_IMATH_RETURN_PYTHON;
        return retval;
    }

    static std::string
    format_arguments(const boost::python::detail::keywords<5> &args)
    {
        // TODO: add types here
        return std::string("(")+args.elements[0].name+","+args.elements[1].name+","+args.elements[2].name+","+args.elements[3].name+","+args.elements[4].name+") - ";
    }
};

template <class Op, class Func, class Keywords>
struct function_binding
{
    std::string _name, _doc;
    const Keywords &_args;


    function_binding(const std::string &name, const std::string &doc,const Keywords &args)
        : _name(name), _doc(doc), _args(args)
    {}

    template <class Vectorize>
    void operator()(Vectorize) const
    {
        typedef typename at<vector<
             int,  // unused, arity 0
             VectorizedFunction1<Op,Vectorize,Func>,
             VectorizedFunction2<Op,Vectorize,Func>,
             VectorizedFunction3<Op,Vectorize,Func>,
             VectorizedFunction4<Op,Vectorize,Func>,
             VectorizedFunction5<Op,Vectorize,Func>
            >,
            long_<function_traits<Func>::arity> >::type vectorized_function_type;
        std::string doc = _name + vectorized_function_type::format_arguments(_args) + _doc;
        boost::python::def(_name.c_str(),&vectorized_function_type::apply,doc.c_str(),_args);
    }
};

template <class Op,class Func,class Keywords>
function_binding<Op,Func,Keywords>
build_function_binding(Func *func,const std::string &name,const std::string &doc,const Keywords &args)
{
    return function_binding<Op,Func,Keywords>(name,doc,args);
}

template <class Op,class Vectorizable,class Keywords>
struct generate_bindings_struct
{
    //BOOST_STATIC_ASSERT(size<Vectorizable>::value == function_traits<Op::apply>::arity);
    static void apply(const std::string &name,const std::string &doc,const Keywords &args) {
        for_each<typename allowable_vectorizations<Vectorizable>::type>(build_function_binding<Op>(Op::apply,name,doc,args));
    }
};


template <class Op, class access_type>
struct VectorizedVoidOperation0 : public Task
{
    access_type access;

    VectorizedVoidOperation0 (access_type a) : access(a) {}

    void execute (size_t start, size_t end)
    {
        for (size_t i = start; i < end; ++i)
        {
            Op::apply (access[i]);
        }
    }
};

template <class Op, class access_type, class arg1_access_type>
struct VectorizedVoidOperation1 : public Task
{
    access_type      access;
    arg1_access_type arg1;

    VectorizedVoidOperation1(access_type a, arg1_access_type a1) : access(a), arg1(a1) {}

    void execute(size_t start, size_t end)
    {
        for (size_t i = start; i < end; ++i)
        {
            Op::apply (access[i], arg1[i]);
        }
    }
};

template <class Op, class access_type, class arg1_access_type, class array_type>
struct VectorizedMaskedVoidOperation1 : public Task
{
    access_type      access;
    arg1_access_type arg1;
    array_type       array;

    VectorizedMaskedVoidOperation1(access_type a, arg1_access_type a1, array_type arr)
        : access(a), arg1(a1), array(arr) {}

    void execute(size_t start, size_t end)
    {
        for (size_t i = start; i < end; ++i)
        {
            const size_t ri = array.raw_ptr_index(i);
            Op::apply (access[i], arg1[ri]);
        }
    }
};

template <class Op, class access_type, class arg1_access_type, class arg2_access_type>
struct VectorizedVoidOperation2 : public Task
{
    access_type      access;
    arg1_access_type arg1;
    arg2_access_type arg2;

    VectorizedVoidOperation2(access_type a, arg1_access_type a1, arg2_access_type a2)
        : access(a), arg1(a1), arg2(a2) {}

    void execute(size_t start, size_t end)
    {
        for (size_t i = start; i < end; ++i)
        {
            Op::apply (access[i], arg1[i], arg2[i]);
        }
    }
};


template <class Op, class Vectorize, class Func>
struct VectorizedVoidMemberFunction0 {
    BOOST_STATIC_ASSERT((size<Vectorize>::value+1 == function_traits<Func>::arity));

    typedef function_traits<Func> traits;

    typedef typename access_type<typename traits::arg1_type>::reference_type reference_type;
    typedef typename access_type<typename traits::arg1_type>::direct direct_access_type;
    typedef typename access_type<typename traits::arg1_type>::masked masked_access_type;

    static reference_type
    apply(reference_type array)
    {
        PY_IMATH_LEAVE_PYTHON;
        size_t len = measure_arguments(array);
        op_precompute<Op>::apply(len);

        if (any_masked(array))
        {
            masked_access_type access (array);
            VectorizedVoidOperation0<Op,masked_access_type> vop (access);
            dispatchTask(vop,len);
        }
        else
        {
            direct_access_type access (array);
            VectorizedVoidOperation0<Op,direct_access_type> vop (access);
            dispatchTask(vop,len);
        }

        PY_IMATH_RETURN_PYTHON;
        return array;
    }
};

template <class Op, class Vectorize, class Func>
struct VectorizedVoidMemberFunction1 {
    BOOST_STATIC_ASSERT((size<Vectorize>::value+1 == function_traits<Func>::arity));

    typedef function_traits<Func> traits;

    typedef typename access_type<typename traits::arg1_type>::reference_type reference_type;
    typedef typename access_type<typename traits::arg1_type>::direct direct_access_type;
    typedef typename access_type<typename traits::arg1_type>::masked masked_access_type;

    typedef typename argument_access_type<typename traits::arg2_type,
                              typename at<Vectorize,long_<0> >::type>::type arg1_type;
    typedef typename argument_access_type<typename traits::arg2_type,
                              typename at<Vectorize,long_<0> >::type>::direct arg1_direct_access_type;
    typedef typename argument_access_type<typename traits::arg2_type,
                              typename at<Vectorize,long_<0> >::type>::masked arg1_masked_access_type;

    static reference_type
    apply(reference_type array, arg1_type arg1)
    {
        PY_IMATH_LEAVE_PYTHON;
        size_t len = measure_arguments(array,arg1);
        op_precompute<Op>::apply(len);

        if (any_masked(array))
        {
            masked_access_type arrayAccess (array);

            if (any_masked(arg1))
            {
                arg1_masked_access_type argAccess =
                     getArrayAccess<arg1_masked_access_type> (arg1);

                VectorizedVoidOperation1<Op,masked_access_type,arg1_masked_access_type>
                    vop (arrayAccess, argAccess);
                dispatchTask(vop,len);
            }
            else
            {
                arg1_direct_access_type argAccess =
                     getArrayAccess<arg1_direct_access_type> (arg1);

                VectorizedVoidOperation1<Op,masked_access_type,arg1_direct_access_type>
                    vop (arrayAccess, argAccess);
                dispatchTask(vop,len);
            }
        }
        else
        {
            direct_access_type arrayAccess (array);

            if (any_masked(arg1))
            {
                arg1_masked_access_type argAccess =
                     getArrayAccess<arg1_masked_access_type> (arg1);

                VectorizedVoidOperation1<Op,direct_access_type,arg1_masked_access_type>
                    vop (arrayAccess, argAccess);
                dispatchTask(vop,len);
            }
            else
            {
                arg1_direct_access_type argAccess =
                     getArrayAccess<arg1_direct_access_type> (arg1);

                VectorizedVoidOperation1<Op,direct_access_type,arg1_direct_access_type>
                    vop (arrayAccess, argAccess);
                dispatchTask(vop,len);
            }
        }

        PY_IMATH_RETURN_PYTHON;
        return array;
    }

    static std::string
    format_arguments(const boost::python::detail::keywords<1> &args)
    {
        // TODO: add types here
        return std::string("(")+args.elements[0].name+") - ";
    }
};

//
// special class to handle single argument void memberfunctions, such as those
// used for the inplace operators like +=, -=, etc.  In this case we allow additional
// compatibilty between a masked class and an unmasked right hand side, using the
// mask to select results.
//
template <class Op, class Func>
struct VectorizedVoidMaskableMemberFunction1 {
    BOOST_STATIC_ASSERT((2 == function_traits<Func>::arity));

    typedef function_traits<Func> traits;

    typedef typename access_type<typename traits::arg1_type>::reference_type reference_type;
    typedef typename access_type<typename traits::arg1_type>::direct direct_access_type;
    typedef typename access_type<typename traits::arg1_type>::masked masked_access_type;

    typedef typename argument_access_type<typename traits::arg2_type,
                                                   boost::mpl::true_>::type arg1_type;
    typedef typename argument_access_type<typename traits::arg2_type,
                                                   boost::mpl::true_>::direct arg1_direct_access_type;
    typedef typename argument_access_type<typename traits::arg2_type,
                                                   boost::mpl::true_>::masked arg1_masked_access_type;

    static reference_type
    apply(reference_type array, arg1_type arg1)
    {
        PY_IMATH_LEAVE_PYTHON;
        size_t len = array.match_dimension(arg1, false);
        op_precompute<Op>::apply(len);

        if (array.isMaskedReference() && (size_t) arg1.len() == array.unmaskedLength())
        {
            // class is masked, and the unmasked length matches the right hand side

            masked_access_type arrayAccess (array);

            if (any_masked(arg1))
            {
                arg1_masked_access_type argAccess =
                     getArrayAccess<arg1_masked_access_type> (arg1);

                VectorizedMaskedVoidOperation1<Op,masked_access_type,
                                             arg1_masked_access_type,
                                                      reference_type>
                    vop (arrayAccess, argAccess, array);
                dispatchTask(vop,len);
            }
            else
            {
                arg1_direct_access_type argAccess =
                     getArrayAccess<arg1_direct_access_type> (arg1);

                VectorizedMaskedVoidOperation1<Op,masked_access_type,
                                             arg1_direct_access_type,
                                                      reference_type>
                    vop (arrayAccess, argAccess, array);
                dispatchTask(vop,len);
            }
        }
        else
        {
            // the two arrays match length (masked or otherwise), use the standard path.

            if (any_masked(array))
            {
                masked_access_type arrayAccess (array);

                if (any_masked(arg1))
                {
                    arg1_masked_access_type argAccess =
                         getArrayAccess<arg1_masked_access_type> (arg1);

                    VectorizedVoidOperation1<Op,masked_access_type,arg1_masked_access_type>
                        vop (arrayAccess, argAccess);
                    dispatchTask(vop,len);
                }
                else
                {
                    arg1_direct_access_type argAccess =
                         getArrayAccess<arg1_direct_access_type> (arg1);

                    VectorizedVoidOperation1<Op,masked_access_type,arg1_direct_access_type>
                        vop (arrayAccess, argAccess);
                    dispatchTask(vop,len);
                }
            }
            else
            {
                direct_access_type arrayAccess (array);

                if (any_masked(arg1))
                {
                    arg1_masked_access_type argAccess =
                         getArrayAccess<arg1_masked_access_type> (arg1);

                    VectorizedVoidOperation1<Op,direct_access_type,arg1_masked_access_type>
                        vop (arrayAccess, argAccess);
                    dispatchTask(vop,len);
                }
                else
                {
                    arg1_direct_access_type argAccess =
                         getArrayAccess<arg1_direct_access_type> (arg1);

                    VectorizedVoidOperation1<Op,direct_access_type,arg1_direct_access_type>
                        vop (arrayAccess, argAccess);
                    dispatchTask(vop,len);
                }
            }
        }
           
        PY_IMATH_RETURN_PYTHON;
        return array;
    }

    static std::string
    format_arguments(const boost::python::detail::keywords<1> &args)
    {
        // TODO: add types here
        return std::string("(")+args.elements[0].name+") - ";
    }
};

template <class Op, class Vectorize, class Func>
struct VectorizedVoidMemberFunction2 {
    BOOST_STATIC_ASSERT((size<Vectorize>::value+1 == function_traits<Func>::arity));

    typedef function_traits<Func> traits;

    typedef typename access_type<typename traits::arg1_type>::reference_type reference_type;
    typedef typename access_type<typename traits::arg1_type>::direct direct_access_type;
    typedef typename access_type<typename traits::arg1_type>::masked masked_access_type;

    typedef typename argument_access_type<typename traits::arg2_type,
                              typename at<Vectorize,long_<0> >::type>::type arg1_type;
    typedef typename argument_access_type<typename traits::arg2_type,
                              typename at<Vectorize,long_<0> >::type>::direct arg1_direct_access_type;
    typedef typename argument_access_type<typename traits::arg2_type,
                              typename at<Vectorize,long_<0> >::type>::masked arg1_masked_access_type;

    typedef typename argument_access_type<typename traits::arg3_type,
                              typename at<Vectorize,long_<1> >::type>::type arg2_type;
    typedef typename argument_access_type<typename traits::arg3_type,
                              typename at<Vectorize,long_<1> >::type>::direct arg2_direct_access_type;
    typedef typename argument_access_type<typename traits::arg3_type,
                              typename at<Vectorize,long_<1> >::type>::masked arg2_masked_access_type;

    static reference_type
    apply(reference_type array, arg1_type arg1, arg2_type arg2)
    {
        PY_IMATH_LEAVE_PYTHON;
        size_t len = measure_arguments(array,arg1,arg2);
        op_precompute<Op>::apply(len);

        if (any_masked(array))
        {
            masked_access_type arrayAccess (array);

            if (any_masked(arg1))
            {
                arg1_masked_access_type arg1Access =
                     getArrayAccess<arg1_masked_access_type> (arg1);

                if (any_masked(arg2))
                {
                    arg2_masked_access_type arg2Access =
                         getArrayAccess<arg2_masked_access_type> (arg2);

                    VectorizedVoidOperation2<Op,masked_access_type,
                                           arg1_masked_access_type,
                                           arg2_masked_access_type>
                          vop (arrayAccess, arg1Access, arg2Access);
                    dispatchTask(vop,len);
                }
                else
                {
                    arg2_direct_access_type arg2Access =
                         getArrayAccess<arg2_direct_access_type> (arg2);

                    VectorizedVoidOperation2<Op,masked_access_type,
                                           arg1_masked_access_type,
                                           arg2_direct_access_type>
                          vop (arrayAccess, arg1Access, arg2Access);
                    dispatchTask(vop,len);
                }
            }
            else
            {
                arg1_direct_access_type arg1Access =
                     getArrayAccess<arg1_direct_access_type> (arg1);

                if (any_masked(arg2))
                {
                    arg2_masked_access_type arg2Access =
                         getArrayAccess<arg2_masked_access_type> (arg2);

                    VectorizedVoidOperation2<Op,masked_access_type,
                                           arg1_direct_access_type,
                                           arg2_masked_access_type>
                          vop (arrayAccess, arg1Access, arg2Access);
                    dispatchTask(vop,len);
                }
                else
                {
                    arg2_direct_access_type arg2Access =
                         getArrayAccess<arg2_direct_access_type> (arg2);

                    VectorizedVoidOperation2<Op,masked_access_type,
                                           arg1_direct_access_type,
                                           arg2_direct_access_type>
                          vop (arrayAccess, arg1Access, arg2Access);
                    dispatchTask(vop,len);
                }
            }
        }
        else
        {
            direct_access_type arrayAccess (array);

            if (any_masked(arg1))
            {
                arg1_masked_access_type arg1Access =
                     getArrayAccess<arg1_masked_access_type> (arg1);

                if (any_masked(arg2))
                {
                    arg2_masked_access_type arg2Access =
                         getArrayAccess<arg2_masked_access_type> (arg2);

                    VectorizedVoidOperation2<Op,direct_access_type,
                                           arg1_masked_access_type,
                                           arg2_masked_access_type>
                          vop (arrayAccess, arg1Access, arg2Access);
                    dispatchTask(vop,len);
                }
                else
                {
                    arg2_direct_access_type arg2Access =
                         getArrayAccess<arg2_direct_access_type> (arg2);

                    VectorizedVoidOperation2<Op,direct_access_type,
                                           arg1_masked_access_type,
                                           arg2_direct_access_type>
                          vop (arrayAccess, arg1Access, arg2Access);
                    dispatchTask(vop,len);
                }
            }
            else
            {
                arg1_direct_access_type arg1Access =
                     getArrayAccess<arg1_direct_access_type> (arg1);

                if (any_masked(arg2))
                {
                    arg2_masked_access_type arg2Access =
                         getArrayAccess<arg2_masked_access_type> (arg2);

                    VectorizedVoidOperation2<Op,direct_access_type,
                                           arg1_direct_access_type,
                                           arg2_masked_access_type>
                          vop (arrayAccess, arg1Access, arg2Access);
                    dispatchTask(vop,len);
                }
                else
                {
                    arg2_direct_access_type arg2Access =
                         getArrayAccess<arg2_direct_access_type> (arg2);

                    VectorizedVoidOperation2<Op,direct_access_type,
                                           arg1_direct_access_type,
                                           arg2_direct_access_type>
                          vop (arrayAccess, arg1Access, arg2Access);
                    dispatchTask(vop,len);
                }
            }
        }

        PY_IMATH_RETURN_PYTHON;
        return array;
    }

    static std::string
    format_arguments(const boost::python::detail::keywords<2> &args)
    {
        // TODO: add types here
        return std::string("(")+args.elements[0].name+","+args.elements[1].name+") - ";
    }
};


template <class Op, class Vectorize, class Func>
struct VectorizedMemberFunction0 {
    BOOST_STATIC_ASSERT((size<Vectorize>::value+1 == function_traits<Func>::arity));

    typedef function_traits<Func> traits;

    typedef typename vectorized_result_type<typename traits::result_type,true_>::type result_type;

    typedef typename access_type<typename traits::arg1_type>::reference_type reference_type;
    typedef typename access_type<typename traits::arg1_type>::direct direct_access_type;
    typedef typename access_type<typename traits::arg1_type>::masked masked_access_type;

    // The return value can't be const or masked.  Verify that condition.
    BOOST_STATIC_ASSERT( !is_const<result_type>::value );
    typedef typename result_type::WritableDirectAccess result_access_type;

    static result_type
    apply(reference_type array)
    {
        PY_IMATH_LEAVE_PYTHON;
        size_t len = measure_arguments(array);
        op_precompute<Op>::apply(len);
        result_type retval = create_uninitalized_return_value<result_type>::apply(len);

        result_access_type returnAccess (retval);

        if (any_masked(array))
        {
            masked_access_type access (array);
            VectorizedOperation1<Op,result_access_type,masked_access_type> vop(returnAccess,access);
            dispatchTask(vop,len);
        }
        else
        {
            direct_access_type access (array);
            VectorizedOperation1<Op,result_access_type,direct_access_type> vop(returnAccess,access);
            dispatchTask(vop,len);
        }

        PY_IMATH_RETURN_PYTHON;
        return retval;
    }
};

template <class Op, class Vectorize, class Func>
struct VectorizedMemberFunction1 {
    BOOST_STATIC_ASSERT((size<Vectorize>::value+1 == function_traits<Func>::arity));

    typedef function_traits<Func> traits;

    typedef typename vectorized_result_type<typename traits::result_type,true_>::type result_type;

    typedef typename access_type<typename traits::arg1_type>::reference_type reference_type;
    typedef typename access_type<typename traits::arg1_type>::direct direct_access_type;
    typedef typename access_type<typename traits::arg1_type>::masked masked_access_type;

    typedef typename argument_access_type<typename traits::arg2_type,
                              typename at<Vectorize,long_<0> >::type>::type arg1_type;
    typedef typename argument_access_type<typename traits::arg2_type,
                              typename at<Vectorize,long_<0> >::type>::direct arg1_direct_access_type;
    typedef typename argument_access_type<typename traits::arg2_type,
                              typename at<Vectorize,long_<0> >::type>::masked arg1_masked_access_type;

    // The return value can't be const or masked.  Verify that condition.
    BOOST_STATIC_ASSERT( !is_const<result_type>::value );
    typedef typename result_type::WritableDirectAccess result_access_type;

    static result_type
    apply(reference_type array, arg1_type arg1)
    {
        PY_IMATH_LEAVE_PYTHON;
        size_t len = measure_arguments(array,arg1);
        op_precompute<Op>::apply(len);
        result_type retval = create_uninitalized_return_value<result_type>::apply(len);

        result_access_type returnAccess (retval);

        if (any_masked(array))
        {
            masked_access_type access (array);

            if (any_masked(arg1))
            {
                arg1_masked_access_type argAccess =
                     getArrayAccess<arg1_masked_access_type> (arg1);

                VectorizedOperation2<Op,result_access_type,
                                        masked_access_type,
                                   arg1_masked_access_type> vop (returnAccess, access, argAccess);
                dispatchTask(vop,len);
            }
            else
            {
                arg1_direct_access_type argAccess =
                     getArrayAccess<arg1_direct_access_type> (arg1);

                VectorizedOperation2<Op,result_access_type,
                                        masked_access_type,
                                   arg1_direct_access_type> vop (returnAccess, access, argAccess);
                dispatchTask(vop,len);
            }
        }
        else
        {
            direct_access_type access (array);

            if (any_masked(arg1))
            {
                arg1_masked_access_type argAccess =
                     getArrayAccess<arg1_masked_access_type> (arg1);

                VectorizedOperation2<Op,result_access_type,
                                        direct_access_type,
                                   arg1_masked_access_type> vop (returnAccess, access, argAccess);
                dispatchTask(vop,len);
            }
            else
            {
                arg1_direct_access_type argAccess =
                     getArrayAccess<arg1_direct_access_type> (arg1);

                VectorizedOperation2<Op,result_access_type,
                                        direct_access_type,
                                   arg1_direct_access_type> vop (returnAccess, access, argAccess);
                dispatchTask(vop,len);
            }
        }

        PY_IMATH_RETURN_PYTHON;
        return retval;
    }

    static std::string
    format_arguments(const boost::python::detail::keywords<1> &args)
    {
        // TODO: add types here
        return std::string("(")+args.elements[0].name+") - ";
    }
};

template <class Op, class Vectorize, class Func>
struct VectorizedMemberFunction2 {
    BOOST_STATIC_ASSERT((size<Vectorize>::value+1 == function_traits<Func>::arity));

    typedef function_traits<Func> traits;

    typedef typename vectorized_result_type<typename traits::result_type,true_>::type result_type;

    typedef typename access_type<typename traits::arg1_type>::reference_type reference_type;
    typedef typename access_type<typename traits::arg1_type>::direct direct_access_type;
    typedef typename access_type<typename traits::arg1_type>::masked masked_access_type;

    typedef typename argument_access_type<typename traits::arg2_type,
                              typename at<Vectorize,long_<0> >::type>::type arg1_type;
    typedef typename argument_access_type<typename traits::arg2_type,
                              typename at<Vectorize,long_<0> >::type>::direct arg1_direct_access_type;
    typedef typename argument_access_type<typename traits::arg2_type,
                              typename at<Vectorize,long_<0> >::type>::masked arg1_masked_access_type;

    typedef typename argument_access_type<typename traits::arg3_type,
                              typename at<Vectorize,long_<1> >::type>::type arg2_type;
    typedef typename argument_access_type<typename traits::arg3_type,
                              typename at<Vectorize,long_<1> >::type>::direct arg2_direct_access_type;
    typedef typename argument_access_type<typename traits::arg3_type,
                              typename at<Vectorize,long_<1> >::type>::masked arg2_masked_access_type;

    // The return value can't be const or masked.  Verify that condition.
    BOOST_STATIC_ASSERT( !is_const<result_type>::value );
    typedef typename result_type::WritableDirectAccess result_access_type;

    static result_type
    apply(reference_type array, arg1_type arg1, arg2_type arg2)
    {
        PY_IMATH_LEAVE_PYTHON;
        size_t len = measure_arguments(array,arg1,arg2);
        op_precompute<Op>::apply(len);
        result_type retval = create_uninitalized_return_value<result_type>::apply(len);

        result_access_type returnAccess (retval);

        if (any_masked(array))
        {
            masked_access_type access (array);

            if (any_masked(arg1))
            {
                arg1_masked_access_type arg1Access =
                     getArrayAccess<arg1_masked_access_type> (arg1);

                if (any_masked(arg2))
                {
                    arg2_masked_access_type arg2Access =
                         getArrayAccess<arg2_masked_access_type> (arg2);

                    VectorizedOperation3<Op,result_access_type,
                                            masked_access_type,
                                       arg1_masked_access_type,
                                       arg2_masked_access_type>
                        vop (returnAccess, access, arg1Access, arg2Access);
                    dispatchTask(vop,len);
                }
                else
                {
                    arg2_direct_access_type arg2Access =
                         getArrayAccess<arg2_direct_access_type> (arg2);

                    VectorizedOperation3<Op,result_access_type,
                                            masked_access_type,
                                       arg1_masked_access_type,
                                       arg2_direct_access_type>
                        vop (returnAccess, access, arg1Access, arg2Access);
                    dispatchTask(vop,len);
                }
            }
            else
            {
                arg1_direct_access_type arg1Access =
                     getArrayAccess<arg1_direct_access_type> (arg1);

                if (any_masked(arg2))
                {
                    arg2_masked_access_type arg2Access =
                         getArrayAccess<arg2_masked_access_type> (arg2);

                    VectorizedOperation3<Op,result_access_type,
                                            masked_access_type,
                                       arg1_direct_access_type,
                                       arg2_masked_access_type>
                        vop (returnAccess, access, arg1Access, arg2Access);
                    dispatchTask(vop,len);
                }
                else
                {
                    arg2_direct_access_type arg2Access =
                         getArrayAccess<arg2_direct_access_type> (arg2);

                    VectorizedOperation3<Op,result_access_type,
                                            masked_access_type,
                                       arg1_direct_access_type,
                                       arg2_direct_access_type>
                        vop (returnAccess, access, arg1Access, arg2Access);
                    dispatchTask(vop,len);
                }
            }
        }
        else
        {
            direct_access_type access (array);

            if (any_masked(arg1))
            {
                arg1_masked_access_type arg1Access =
                     getArrayAccess<arg1_masked_access_type> (arg1);

                if (any_masked(arg2))
                {
                    arg2_masked_access_type arg2Access =
                         getArrayAccess<arg2_masked_access_type> (arg2);

                    VectorizedOperation3<Op,result_access_type,
                                            direct_access_type,
                                       arg1_masked_access_type,
                                       arg2_masked_access_type>
                        vop (returnAccess, access, arg1Access, arg2Access);
                    dispatchTask(vop,len);
                }
                else
                {
                    arg2_direct_access_type arg2Access =
                         getArrayAccess<arg2_direct_access_type> (arg2);

                    VectorizedOperation3<Op,result_access_type,
                                            direct_access_type,
                                       arg1_masked_access_type,
                                       arg2_direct_access_type>
                        vop (returnAccess, access, arg1Access, arg2Access);
                    dispatchTask(vop,len);
                }
            }
            else
            {
                arg1_direct_access_type arg1Access =
                     getArrayAccess<arg1_direct_access_type> (arg1);

                if (any_masked(arg2))
                {
                    arg2_masked_access_type arg2Access =
                         getArrayAccess<arg2_masked_access_type> (arg2);

                    VectorizedOperation3<Op,result_access_type,
                                            direct_access_type,
                                       arg1_direct_access_type,
                                       arg2_masked_access_type>
                        vop (returnAccess, access, arg1Access, arg2Access);
                    dispatchTask(vop,len);
                }
                else
                {
                    arg2_direct_access_type arg2Access =
                         getArrayAccess<arg2_direct_access_type> (arg2);

                    VectorizedOperation3<Op,result_access_type,
                                            direct_access_type,
                                       arg1_direct_access_type,
                                       arg2_direct_access_type>
                        vop (returnAccess, access, arg1Access, arg2Access);
                    dispatchTask(vop,len);
                }
            }
        }

        PY_IMATH_RETURN_PYTHON;
        return retval;
    }

    static std::string
    format_arguments(const boost::python::detail::keywords<2> &args)
    {
        // TODO: add types here
        return std::string("(")+args.elements[0].name+","+args.elements[1].name+") - ";
    }
};

template <class Op, class Cls, class Func, class Keywords>
struct member_function_binding
{
    Cls &_cls;
    std::string _name, _doc;
    const Keywords &_args;

    member_function_binding(Cls &cls,const std::string &name, const std::string &doc,const Keywords &args)
        : _cls(cls), _name(name), _doc(doc), _args(args)
    {}

    template <class Vectorize>
    void operator()(Vectorize) const
    {
        typedef typename if_<is_same<void,typename function_traits<Func>::result_type>,
                             typename if_<boost::mpl::equal<Vectorize,boost::mpl::vector<boost::mpl::true_> >,
                                 VectorizedVoidMaskableMemberFunction1<Op,Func>,
                                 VectorizedVoidMemberFunction1<Op,Vectorize,Func> >::type,
                             VectorizedMemberFunction1<Op,Vectorize,Func>
                         >::type member_func1_type;

        typedef typename if_<is_same<void,typename function_traits<Func>::result_type>,
                         VectorizedVoidMemberFunction2<Op,Vectorize,Func>,
                         VectorizedMemberFunction2<Op,Vectorize,Func> >::type member_func2_type;

        typedef typename if_<is_same<void,typename function_traits<Func>::result_type>,
                         boost::python::return_internal_reference<>,  // the void vectorizations return a reference to self
                         boost::python::default_call_policies>::type call_policies;

        typedef typename at<vector<
            int,  // unused, arity 0
            int,  // unused, arity 1 - first argument corresponds to the class type
            member_func1_type,
            member_func2_type
            >,
            long_<function_traits<Func>::arity> >::type vectorized_function_type;
        std::string doc = _name + vectorized_function_type::format_arguments(_args) + _doc;
        _cls.def(_name.c_str(),&vectorized_function_type::apply,doc.c_str(),_args,call_policies());
    }
};

template <class Op,class Cls,class Func,class Keywords>
member_function_binding<Op,Cls,Func,Keywords>
build_member_function_binding(Cls &cls,Func *func,const std::string &name,const std::string &doc,const Keywords &args)
{
    return member_function_binding<Op,Cls,Func,Keywords>(cls,name,doc,args);
}

template <class Op,class Cls,class Vectorizable,class Keywords>
struct generate_member_bindings_struct
{
    //BOOST_STATIC_ASSERT(size<Vectorizable>::value+1 == function_traits<Op::apply>::arity);
    static void apply(Cls &cls,const std::string &name,const std::string &doc,const Keywords &args) {
        for_each<typename allowable_vectorizations<Vectorizable>::type>(build_member_function_binding<Op>(cls,Op::apply,name,doc,args));
    }
};

template <class Op,class Cls,class Func>
void
generate_single_member_binding(Cls &cls,Func *func,const std::string &name,const std::string &doc)
{
    typedef typename if_<is_same<void,typename function_traits<Func>::result_type>,
                         VectorizedVoidMemberFunction0<Op,boost::mpl::vector<>,Func>,
                         VectorizedMemberFunction0<Op,boost::mpl::vector<>,Func> >::type vectorized_function_type;

    typedef typename if_<is_same<void,typename function_traits<Func>::result_type>,
                         boost::python::return_internal_reference<>,  // the void vectorizations return a reference to self
                         boost::python::default_call_policies>::type call_policies;

    cls.def(name.c_str(),&vectorized_function_type::apply,doc.c_str(),call_policies());
}

} // namespace detail

// TODO: update for arg("name")=default_value syntax
template <class Op,class Vectorizable0>
void generate_bindings(const std::string &name,const std::string &doc,const boost::python::detail::keywords<1> &args) {
    using namespace detail;
    generate_bindings_struct<Op,vector<Vectorizable0>,boost::python::detail::keywords<1> >::apply(name,doc,args);
}

template <class Op,class Vectorizable0, class Vectorizable1>
void generate_bindings(const std::string &name,const std::string &doc,const boost::python::detail::keywords<2> &args) {
    using namespace detail;
    generate_bindings_struct<Op,vector<Vectorizable0,Vectorizable1>,boost::python::detail::keywords<2> >::apply(name,doc,args);
}

template <class Op,class Vectorizable0, class Vectorizable1, class Vectorizable2>
void generate_bindings(const std::string &name,const std::string &doc,const boost::python::detail::keywords<3> &args) {
    using namespace detail;
    generate_bindings_struct<Op,vector<Vectorizable0,Vectorizable1,Vectorizable2>,boost::python::detail::keywords<3> >::apply(name,doc,args);
}

template <class Op,class Vectorizable0, class Vectorizable1, class Vectorizable2, class Vectorizable3>
void generate_bindings(const std::string &name,const std::string &doc,const boost::python::detail::keywords<4> &args) {
    using namespace detail;
    generate_bindings_struct<Op,vector<Vectorizable0,Vectorizable1,Vectorizable2,Vectorizable3>,boost::python::detail::keywords<4> >::apply(name,doc,args);
}

template <class Op,class Vectorizable0, class Vectorizable1, class Vectorizable2, class Vectorizable3, class Vectorizable4>
void generate_bindings(const std::string &name,const std::string &doc,const boost::python::detail::keywords<5> &args) {
    using namespace detail;
    generate_bindings_struct<Op,vector<Vectorizable0,Vectorizable1,Vectorizable2,Vectorizable3,Vectorizable4>,boost::python::detail::keywords<5> >::apply(name,doc,args);
}

template <class Op,class Cls>
void
generate_member_bindings(Cls &cls,const std::string &name,const std::string &doc)
{
    using namespace detail;
    generate_single_member_binding<Op>(cls,&Op::apply,name,doc);
}

template <class Op,class Vectorizable0,class Cls>
void
generate_member_bindings(Cls &cls,const std::string &name,const std::string &doc,
                         const boost::python::detail::keywords<1> &args)
{
    using boost::mpl::vector;
    detail::generate_member_bindings_struct<Op,Cls,vector<Vectorizable0>,
                                            boost::python::detail::keywords<1> >::apply(cls,name,doc,args);
}

template <class Op,class Vectorizable0,class Vectorizable1,class Cls>
void
generate_member_bindings(Cls &cls,const std::string &name,const std::string &doc,
                         const boost::python::detail::keywords<2> &args)
{
    using boost::mpl::vector;
    detail::generate_member_bindings_struct<Op,Cls,vector<Vectorizable0,Vectorizable1>,
                                            boost::python::detail::keywords<2> >::apply(cls,name,doc,args);
}

} // namespace PyImath

#endif // _PyImathAutovectorize_h_
