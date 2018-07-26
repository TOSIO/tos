#include <netaddress.h>
#include <string>
#include <boost/test/unit_test.hpp>
#include <deps/streams.h>

// #define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE netadress_test

class NetAddrTest : public CNetAddr
{
    public:
    NetAddrTest()
    {
        strcpy((char*)ip,"::FFFF:0:0/96");
        scopeId = 10;
    } 

    void clear()
    {
        memset((char*)ip,0,16);
        scopeId = 0;
    }

    unsigned char* getIp()
    {
        return ip;
    }
};

BOOST_AUTO_TEST_SUITE(netadress_test)

BOOST_AUTO_TEST_CASE(netadress_serialize)
{
    NetAddrTest netAddr;
    DataStream  stream(0,0);
    netAddr.Serialize(stream);
    netAddr.clear();
    
    bytes source = stream.stream()->out();
    netAddr.UnSerialize(source);
    BOOST_CHECK_EQUAL(strcmp((const char*)netAddr.getIp(),"::FFFF:0:0/96"), 0);
}

BOOST_AUTO_TEST_SUITE_END()


boost::unit_test::test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    return 0;

}