//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#ifdef NDEBUG
#    undef NDEBUG
#endif

#include <PyImathStringTable.h>
#include <iostream>
#include <assert.h>

using namespace std;
using namespace PyImath;

namespace {

void
testString()
{
    StringTable st;

    size_t size = st.size();
    assert(size == 0);
    assert(!st.hasString("foo"));

    StringTableIndex index = st.intern("foo");
    assert(index == st.lookup("foo"));
    assert(st.lookup(index) == "foo");
    assert(st.hasString("foo"));

    size = st.size();
    assert(size == 1);

    bool thrown = false;
    try {
        index = st.lookup("bar");
    } catch (std::exception &e) {
        thrown = true;
    }
    assert (thrown);

    thrown = false;
    try {
        std::string s = st.lookup(StringTableIndex(1));
    } catch (std::exception &e) {
        thrown = true;
    }
    assert (thrown);
    
    assert(!st.hasString("bar"));

    index = st.intern("bar");
    assert(index == st.lookup("bar"));
    assert(st.lookup(index) == "bar");
    assert(0 == st.lookup("foo").index());
    assert(st.lookup(StringTableIndex(0)) == "foo");
    assert(st.hasString("bar"));
}

void
testWString()
{
    WStringTable st;

    size_t size = st.size();
    assert(size == 0);
    assert(!st.hasString(L"foo"));

    StringTableIndex index = st.intern(L"foo");
    assert(index == st.lookup(L"foo"));
    assert(st.lookup(index) == L"foo");
    assert(st.hasString(L"foo"));

    size = st.size();
    assert(size == 1);

    bool thrown = false;
    try {
        index = st.lookup(L"bar");
    } catch (std::exception &e) {
        thrown = true;
    }
    assert (thrown);

    thrown = false;
    try {
        std::wstring s = st.lookup(StringTableIndex(1));
    } catch (std::exception &e) {
        thrown = true;
    }
    assert (thrown);
    
    assert(!st.hasString(L"bar"));

    index = st.intern(L"bar");
    assert(index == st.lookup(L"bar"));
    assert(st.lookup(index) == L"bar");
    assert(0 == st.lookup(L"foo").index());
    assert(st.lookup(StringTableIndex(0)) == L"foo");
    assert(st.hasString(L"bar"));
}

} // namespace


void
testStringTable()
{
    cout << "Testing string table:" << endl;
    
    try
    {
	testString();
        testWString();
    }
    catch (const exception &e)
    {
	cerr << "Unexpected exception: " << e.what() << endl;
	assert(false);
    }

    cout << "ok\n" << endl;
}
