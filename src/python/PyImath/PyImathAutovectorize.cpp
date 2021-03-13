//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#include "PyImathAutovectorize.h"


namespace PyImath {

namespace detail {
//
// cheek possible vectorizations to ensure correctness
//
// single argument should be ((false),(true))
//
BOOST_STATIC_ASSERT(( size<possible_vectorizations<1>::type>::type::value == 2 ));
BOOST_STATIC_ASSERT(( size<at_c<possible_vectorizations<1>::type,0>::type>::type::value == 1 ));
BOOST_STATIC_ASSERT(( at_c<at_c<possible_vectorizations<1>::type,0>::type,0>::type::value == false ));
BOOST_STATIC_ASSERT(( size<at_c<possible_vectorizations<1>::type,1>::type>::type::value == 1 ));
BOOST_STATIC_ASSERT(( at_c<at_c<possible_vectorizations<1>::type,1>::type,0>::type::value == true ));

//
// two argument should be ((false,false),(false,true),(true,false),(true,true))
//
BOOST_STATIC_ASSERT(( size<possible_vectorizations<2>::type>::type::value == 4 ));
BOOST_STATIC_ASSERT(( size<at_c<possible_vectorizations<2>::type,0>::type>::type::value == 2 ));
BOOST_STATIC_ASSERT(( at_c<at_c<possible_vectorizations<2>::type,0>::type,0>::type::value == false ));
BOOST_STATIC_ASSERT(( at_c<at_c<possible_vectorizations<2>::type,0>::type,1>::type::value == false ));
BOOST_STATIC_ASSERT(( size<at_c<possible_vectorizations<2>::type,1>::type>::type::value == 2 ));
BOOST_STATIC_ASSERT(( at_c<at_c<possible_vectorizations<2>::type,1>::type,0>::type::value == false ));
BOOST_STATIC_ASSERT(( at_c<at_c<possible_vectorizations<2>::type,1>::type,1>::type::value == true ));
BOOST_STATIC_ASSERT(( size<at_c<possible_vectorizations<2>::type,2>::type>::type::value == 2 ));
BOOST_STATIC_ASSERT(( at_c<at_c<possible_vectorizations<2>::type,2>::type,0>::type::value == true ));
BOOST_STATIC_ASSERT(( at_c<at_c<possible_vectorizations<2>::type,2>::type,1>::type::value == false ));
BOOST_STATIC_ASSERT(( size<at_c<possible_vectorizations<2>::type,3>::type>::type::value == 2 ));
BOOST_STATIC_ASSERT(( at_c<at_c<possible_vectorizations<2>::type,3>::type,0>::type::value == true ));
BOOST_STATIC_ASSERT(( at_c<at_c<possible_vectorizations<2>::type,3>::type,1>::type::value == true ));
BOOST_STATIC_ASSERT(( size<possible_vectorizations<3>::type>::type::value == 8 ));

//
// Check disallow_vectorization for given vectorizable flags
//
BOOST_STATIC_ASSERT(( disallow_vectorization<vector<true_ > >::apply<vector<true_ > >::type::value == false  ));
BOOST_STATIC_ASSERT(( disallow_vectorization<vector<true_ > >::apply<vector<false_> >::type::value == false  ));
BOOST_STATIC_ASSERT(( disallow_vectorization<vector<false_> >::apply<vector<true_ > >::type::value == true  ));
BOOST_STATIC_ASSERT(( disallow_vectorization<vector<false_> >::apply<vector<false_> >::type::value == false  ));

BOOST_STATIC_ASSERT(( disallow_vectorization<vector<true_ , true_ > >::apply<vector<true_ , true_ > >::type::value == false  ));
BOOST_STATIC_ASSERT(( disallow_vectorization<vector<true_ , true_ > >::apply<vector<false_, true_ > >::type::value == false  ));
BOOST_STATIC_ASSERT(( disallow_vectorization<vector<true_ , true_ > >::apply<vector<true_ , false_> >::type::value == false  ));
BOOST_STATIC_ASSERT(( disallow_vectorization<vector<true_ , true_ > >::apply<vector<false_, false_> >::type::value == false  ));
BOOST_STATIC_ASSERT(( disallow_vectorization<vector<true_ , false_> >::apply<vector<true_ , true_ > >::type::value == true  ));
BOOST_STATIC_ASSERT(( disallow_vectorization<vector<true_ , false_> >::apply<vector<false_, true_ > >::type::value == true  ));
BOOST_STATIC_ASSERT(( disallow_vectorization<vector<true_ , false_> >::apply<vector<true_ , false_> >::type::value == false  ));
BOOST_STATIC_ASSERT(( disallow_vectorization<vector<true_ , false_> >::apply<vector<false_, false_> >::type::value == false  ));
BOOST_STATIC_ASSERT(( disallow_vectorization<vector<false_, true_ > >::apply<vector<true_ , true_ > >::type::value == true   ));
BOOST_STATIC_ASSERT(( disallow_vectorization<vector<false_, true_ > >::apply<vector<false_, true_ > >::type::value == false  ));
BOOST_STATIC_ASSERT(( disallow_vectorization<vector<false_, true_ > >::apply<vector<true_ , false_> >::type::value == true  ));
BOOST_STATIC_ASSERT(( disallow_vectorization<vector<false_, true_ > >::apply<vector<false_, false_> >::type::value == false  ));
BOOST_STATIC_ASSERT(( disallow_vectorization<vector<false_, false_> >::apply<vector<true_ , true_ > >::type::value == true  ));
BOOST_STATIC_ASSERT(( disallow_vectorization<vector<false_, false_> >::apply<vector<false_, true_ > >::type::value == true  ));
BOOST_STATIC_ASSERT(( disallow_vectorization<vector<false_, false_> >::apply<vector<true_ , false_> >::type::value == true  ));
BOOST_STATIC_ASSERT(( disallow_vectorization<vector<false_, false_> >::apply<vector<false_, false_> >::type::value == false  ));

//
// Check allowable_vectorizations, single argument not vectorizable, and two argument second argument vectorizable.
//
typedef allowable_vectorizations<vector<false_> > AV1f;
BOOST_STATIC_ASSERT(( size<AV1f::possible>::type::value == 2 ));
BOOST_STATIC_ASSERT(( at_c<at_c<AV1f::possible,0>::type,0>::type::value == false ));
BOOST_STATIC_ASSERT(( at_c<at_c<AV1f::possible,1>::type,0>::type::value == true ));
BOOST_STATIC_ASSERT(( size<AV1f::type>::type::value == 1 ));
BOOST_STATIC_ASSERT(( size<at_c<AV1f::type,0>::type>::type::value == 1 ));
BOOST_STATIC_ASSERT(( at_c<at_c<AV1f::type,0>::type,0>::type::value == false ));

typedef allowable_vectorizations<vector<false_,true_> > AV2ft;
BOOST_STATIC_ASSERT(( size<AV2ft::type>::type::value == 2 ));
BOOST_STATIC_ASSERT(( size<at_c<AV2ft::type,0>::type>::type::value == 2 ));
BOOST_STATIC_ASSERT(( at_c<at_c<AV2ft::type,0>::type,0>::type::value == false ));
BOOST_STATIC_ASSERT(( at_c<at_c<AV2ft::type,0>::type,1>::type::value == false ));
BOOST_STATIC_ASSERT(( size<at_c<AV2ft::type,1>::type>::type::value == 2 ));
BOOST_STATIC_ASSERT(( at_c<at_c<AV2ft::type,1>::type,0>::type::value == false ));
BOOST_STATIC_ASSERT(( at_c<at_c<AV2ft::type,1>::type,1>::type::value == true ));

} // namespace detail

} // namespace PyImath
