#define BOOST_TEST_MODULE test

#include <boost/test/unit_test.hpp>
#include <toscore/utils/JsonUtils.h>
#include <stdio.h>
#include <toscore/log/Log.h>
#include <toscore/crypto/SHA3.h>
#include <toscore/common/Common.h>
#include <toscore/log/Log.h>
#include <toscore/crypto/Hash.h>
#include <secp256k1_sha256.h>
#include <toscore/crypto/FixedHash.h>
#include <toscore/common/vector_ref.h>
#include <test/tools/TosTestUtils.h>
#include <boost/test/included/unit_test.hpp>
#include <toscore/crypto/Base64.h>
#include <boost/filesystem.hpp>
#include <set>
using std::cout;
using namespace boost;
using namespace boost::unit_test;
using namespace dev;

BOOST_AUTO_TEST_SUITE(test)

class Test
{
  public:
    bool validateFieldNames(json_spirit::mObject const &_obj, std::set<std::string> const &_allowedFields)
    {
        for (auto const &elm : _obj)
        {
            if (_allowedFields.find(elm.first) == _allowedFields.end())
            {
                std::string const comment = "Unknown field in config: " + elm.first;
                std::cerr << comment << "\n";
                return false;
            }
        }
        return true;
    }
};

BOOST_FIXTURE_TEST_CASE(mytest, Test)
{
    Test t;
    boost::filesystem::path filePath("demo.json");
    json_spirit::Value_impl<json_spirit::Config_map<std::string>> value;
    std::string fileName = filePath.string();
    std::ifstream is(fileName);
    json_spirit::read(is, value);
    std::set<string> mySet;
    mySet.insert("Code");
    mySet.insert("Description");

    BOOST_CHECK(t.validateFieldNames(value, mySet));
}

BOOST_AUTO_TEST_SUITE_END()
