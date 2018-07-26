#define BOOST_TEST_MODULE maintest

#include <stdio.h>
#include <string>
#include <toscore/utils/RLP.h>
#include <list>
#include <toscore/log/Log.h>
#include <toscore/common/vector_ref.h>
#include <toscore/common/Exceptions.h>
#include <test/tools/TosTestUtils.h>
#include <boost/test/included/unit_test.hpp>
#include <boost/smart_ptr.hpp>

using namespace boost;
using namespace boost::unit_test;
using namespace dev;
using std::cout;
using std::string;

class TestRLP
{
    // dev::bytes rlpEncode();
    // void rlpDecode(dev::bytes output);
    dev::bytes rlpEncode()
    {

        dev::RLPStream root;
        dev::RLPStream stream;

        root.appendList(3);
        stream.appendList(2) << "hello"
                             << "world";
        root.appendRaw(stream.out());

        stream.clear();
        stream.appendList(2) << "xxx"
                             << "chee";
        root.appendRaw(stream.out());

        stream.clear();
        stream.appendList(2) << "ddd"
                             << "dfe";
        root.appendRaw(stream.out());

        const dev::bytes output = root.out();
        string strdemo = dev::toString(output);
        cnote << "encode string : " << strdemo;
        string hex = dev::toHex(output);
        cnote << "encode hex : " << hex.c_str();

        return output;
    }

    void rlpDecode(dev::bytes output)
    {

        dev::RLP rlp(output);
        cnote << "trace1";

        string str1 = rlp[0][0].toString();
        string str2 = rlp[2][1].toString();
        cnote << "str[0][0]: " << str1.c_str() << "  str2[2][1]: " << str2.c_str();
    }

  public:
    void runRlpTest()
    {
        cerror << "rlp error" << std::endl;
        dev::bytes byt = rlpEncode();
        rlpDecode(byt);
    }
};

BOOST_FIXTURE_TEST_SUITE(RlpTests, TestHelperFixture)

BOOST_FIXTURE_TEST_CASE(runRlpTest, TestHelperFixture)
{
    TestRLP test;
    test.runRlpTest();
}



BOOST_AUTO_TEST_SUITE_END()
