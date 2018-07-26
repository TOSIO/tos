#include <netaddress.h>
#include <netbase.h>
#include <string>
#include <boost/test/unit_test.hpp>
#include <deps/streams.h>

//#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE netadress_test

class NetAddrTest : public CNetAddr
{
    public:
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

static CNetAddr ResolveIP(const char* ip)
{
    CNetAddr addr;
    LookupHost(ip, addr, false);
    return addr;
}

BOOST_AUTO_TEST_SUITE(netadress_test)

BOOST_AUTO_TEST_CASE(netadress_serialize)
{
    CNetAddr addr = ResolveIP("127.0.0.1");
    DataStream  stream(0,0);
    addr.Serialize(stream);

    bytes source = stream.stream()->out();
    CNetAddr dec_addr;
    dec_addr.UnSerialize(source);
    printf("after decode,address : %s\n",dec_addr.ToString().c_str());
    BOOST_CHECK_EQUAL(strcmp(dec_addr.ToString().c_str(),"127.0.0.1"), 0);
}

BOOST_AUTO_TEST_SUITE_END()


boost::unit_test::test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    return 0;
}