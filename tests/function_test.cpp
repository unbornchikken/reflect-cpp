/* function_test.cpp                                 -*- C++ -*-
   Rémi Attab (remi.attab@gmail.com), 28 Mar 2014
   FreeBSD-style copyright and disclaimer apply

   Tests for function.h
*/

#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK

#include "reflect.h"
#include "types/primitives.h"

#include <boost/test/unit_test.hpp>

using namespace std;
using namespace reflect;

unsigned foo(unsigned i, int j)
{
    return i + 1 * j;
}

BOOST_AUTO_TEST_CASE(basics)
{
    using reflect::reflect;

    Function fn("foo", &foo);

    BOOST_CHECK_EQUAL(fn.returnType(), reflect<unsigned>());
    BOOST_CHECK_EQUAL(fn.size(), 2);
    BOOST_CHECK_EQUAL(fn[0], reflect<unsigned>());
    BOOST_CHECK_EQUAL(fn[1], reflect<int>());

    BOOST_CHECK( fn.test(fn));
    BOOST_CHECK( fn.test<unsigned(unsigned, int)>());
    BOOST_CHECK( fn.test<void    (unsigned, int)>());
    BOOST_CHECK(!fn.test<unsigned(int, unsigned)>());

    BOOST_CHECK_EQUAL(fn.call<unsigned>(1u, 2), foo(1, 2));
    fn.call<void>(1u, 2);
}
