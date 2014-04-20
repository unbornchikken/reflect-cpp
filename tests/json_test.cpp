/* json_test.cpp                                 -*- C++ -*-
   Rémi Attab (remi.attab@gmail.com), 20 Apr 2014
   FreeBSD-style copyright and disclaimer apply

   Json tests.
*/

#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define REFLECT_USE_EXCEPTIONS 1

#include "reflect.h"
#include "types/primitives.h"
#include "types/std/string.h"
#include "types/std/vector.h"
#include "reflect/plumbing.h"
#include "reflect/class.h"
#include "reflect/field.h"
#include "utils/json/parser.h"

#include <boost/test/unit_test.hpp>
#include <fstream>

using namespace std;
using namespace reflect;

/******************************************************************************/
/* BLEH                                                                       */
/******************************************************************************/

struct Bleh
{
    long i;
    bool b;

    Bleh() : i(0), b(false) {}
};

reflectClass(Bleh) 
{
    reflectPlumbing();
    reflectField(i);
    reflectField(b);
}


/******************************************************************************/
/* BLAH                                                                       */
/******************************************************************************/

struct Blah
{
    std::string str;
    std::vector<Bleh> bleh;
};

reflectClass(Blah)
{
    reflectPlumbing();
    reflectField(str);
    reflectField(bleh);
}


/******************************************************************************/
/* PARSING                                                                    */
/******************************************************************************/

BOOST_AUTO_TEST_CASE(parsing)
{
    std::ifstream ifs("tests/json/basics.json");
    BOOST_CHECK(ifs);

    auto blah = json::parse<Blah>(ifs);

    auto checkBleh = [] (const Bleh& bleh, long i, bool b) {
        BOOST_CHECK_EQUAL(bleh.i, i);
        BOOST_CHECK_EQUAL(bleh.b, b);
    };

    BOOST_CHECK_EQUAL(blah.str, "I like candy");

    BOOST_CHECK_EQUAL(blah.bleh.size(), 3);
    checkBleh(blah.bleh[0], 10, false);
    checkBleh(blah.bleh[1], 0, true);
    checkBleh(blah.bleh[2], 30, false);
}
