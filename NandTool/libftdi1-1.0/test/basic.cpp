/**@file
@brief Test basic FTDI functionality

@author Thomas Jarosch
*/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License           *
 *   version 2.1 as published by the Free Software Foundation;             *
 *                                                                         *
 ***************************************************************************/

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <ftdi.h>

BOOST_AUTO_TEST_SUITE(Basic)

BOOST_AUTO_TEST_CASE(SimpleInit)
{
    ftdi_context ftdi;

    int rtn_init = ftdi_init(&ftdi);
    BOOST_REQUIRE_EQUAL(0, rtn_init);

    ftdi_deinit(&ftdi);
}

BOOST_AUTO_TEST_SUITE_END()
