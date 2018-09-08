// Copyright (c) 2011-2017 The Bitcoin Core developers
// Copyright (c) 2017-2018 The Toschain Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#include <arith_uint256.h>
#include <uint256.h>
#include <version.h>
#include <test/test_toschain.h>

#include <boost/test/unit_test.hpp>
#include <stdint.h>
#include <sstream>
#include <iomanip>
#include <limits>
#include <cmath>
#include <string>
#include <stdio.h>

BOOST_FIXTURE_TEST_SUITE(uint256_tests, BasicTestingSetup)

const unsigned char R1Array[] =
    "\x9c\x52\x4a\xdb\xcf\x56\x11\x12\x2b\x29\x12\x5e\x5d\x35\xd2\xd2"
    "\x22\x81\xaa\xb5\x33\xf0\x08\x32\xd5\x56\xb1\xf9\xea\xe5\x1d\x7d";
const char R1ArrayHex[] = "7D1DE5EAF9B156D53208F033B5AA8122D2d2355d5e12292b121156cfdb4a529c";
const uint256 R1L = uint256(std::vector<unsigned char>(R1Array,R1Array+32));
const uint160 R1S = uint160(std::vector<unsigned char>(R1Array,R1Array+20))

const unsigned char OneArray[] =
    "\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";
const uint256 OneL = uint256(std::vector<unsigned char>(OneArray,OneArray+32));
const uint160 OneS = uint160(std::vector<unsigned char>(OneArray,OneArray+20));

const unsigned char MaxArray[] =
    "\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
    "\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff";
const uint256 MaxL = uint256(std::vector<unsigned char>(MaxArray,MaxArray+32));
const uint160 MaxS = uint160(std::vector<unsigned char>(MaxArray,MaxArray+20));

std::string ArrayToString(const unsigned char A[], unsigned int width)
{
    std::stringstream Stream;
    Stream << std::hex;
    for (unsigned int i = 0; i < width; ++i)
    {
        Stream<<std::setw(2)<<std::setfill('0')<<(unsigned int)A[width-i-1];
    }
    return Stream.str();
}

BOOST_AUTO_TEST_SUITE_END()
