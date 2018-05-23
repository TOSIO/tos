// Copyright (c) 2011-2017 The Bitcoin Core developers
// Copyright (c) 2017-2018 The Toschain Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <test/data/base58_encode_decode.json.h>

#include <base58.h>
#include <test/test_toschain.h>
#include <utilstrencodings.h>

#include <univalue.h>

#include <boost/test/unit_test.hpp>


extern UniValue read_json(const std::string& jsondata);

BOOST_FIXTURE_TEST_SUITE(base58_tests, BasicTestingSetup)

// Goal: test low-level base58 encoding functionality
BOOST_AUTO_TEST_CASE(base58_EncodeBase58)
{
    UniValue tests = read_json(std::string(json_tests::base58_encode_decode, json_tests::base58_encode_decode + sizeof(json_tests::base58_encode_decode)));
    for (unsigned int idx = 0; idx < tests.size(); idx++) {
        UniValue test = tests[idx];
        std::string strTest = test.write();
        if (test.size() < 2) // Allow for extra stuff (useful for comments)
        {
            BOOST_ERROR("Bad test: " << strTest);
            continue;
        }
        std::vector<unsigned char> sourcedata = ParseHex(test[0].get_str());
        std::string base58string = test[1].get_str();
        BOOST_CHECK_MESSAGE(
                    EncodeBase58(sourcedata.data(), sourcedata.data() + sourcedata.size()) == base58string,
                    strTest);
    }
}

BOOST_AUTO_TEST_SUITE_END()
