
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
#include <toscore/jsonspirit/json_spirit.h>
#include <toscore/jsonspirit/JsonSpiritHeaders.h>
#include <set>
#include <toscore/utils/FileUtils.h>
using std::cout;
using namespace boost;
using namespace boost::unit_test;
using namespace dev;
using namespace json_spirit;

class Test
{
  public:
    bool validateFieldNames1(json_spirit::mObject const &_obj, std::set<std::string> const &_allowedFields)
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

BOOST_AUTO_TEST_CASE(jsontests)
{

    // Test t;
    boost::filesystem::path filePath("demo.json");
    json_spirit::Value_impl<json_spirit::Config_map<std::string> > value;

    std::string fileName = filePath.string();
    std::ifstream is(fileName);

    json_spirit::read(is, value);
    std::set<std::string> mySet;
    mySet.insert("Code");
    mySet.insert("Description");

    // t.validateFieldNames1(value, mySet);
}

