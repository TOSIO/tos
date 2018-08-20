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

class Test
{
    public:
    string sub;
    string sub2;
};

class TestRLP
{
    // dev::bytes rlpEncode();
    // void rlpDecode(dev::bytes output);
    dev::bytes rlpEncode()
    {

        Test t;
        t.sub = "3333";
        t.sub2 = "4444";
        dev::RLPStream stream1;
stream1.appendList(2);
// stream1 << t;
stream1.append(t.sub).append(t.sub2);

        dev::RLPStream root;
        dev::RLPStream stream;

        root.appendList(4);

root.appendRaw(stream1.out());

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

        // stream1.appendList(4);
        // streaml.appendList(2);
        // stream1<<t.sub<<t.sub2;
        // stream1.appendList(2);
        // stream1<<"xxx"<<"chee";
        // stream1.appendList(2);
        return output;
    }

    void rlpDecode(dev::bytes output)
    {

        dev::RLP rlp(output);
        cnote << "trace1  " << rlp[0][0].toString().c_str();
// rlp[0][0].toString();
        string str1 = rlp[1][0].toString();
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
