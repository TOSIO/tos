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
/* 
bool static TestParse(std::string src, std::string canon)
{
    CService addr(LookupNumeric(src.c_str(), 65535));
    return canon == addr.ToString();
}
 */

BOOST_AUTO_TEST_SUITE(netadress_test)

BOOST_AUTO_TEST_CASE(netadress_serialize)
{
    CNetAddr addr = ResolveIP("127.0.0.1");
    DataStream  stream(0,0);
    addr.Serialize(stream);

    bytes source = stream.stream()->out();
    CNetAddr dec_addr;
    dec_addr.UnSerialize(&source,0,0);
    BOOST_CHECK(dec_addr.ToString() == "127.0.0.1"); 
}

BOOST_AUTO_TEST_CASE(csubnet_serialize)
{
    CSubNet subNet(ResolveIP("1.2.3.4"),8);
    printf("BOOST_AUTO_TEST_CASE(csubnet_serialize) | Before encode,subNet :%s\n",subNet.ToString().c_str());
    DataStream  stream(0,0);
    subNet.Serialize(stream);
    
    bytes source = stream.stream()->out();
    bytesConstRef sourceRef(source.data(),source.size());

    CSubNet decSubNet;
    decSubNet.UnSerialize(sourceRef,0,0);
    printf("toString() :%s\n",decSubNet.ToString().c_str());
    BOOST_CHECK(decSubNet.ToString() == "1.0.0.0/8"); 

    subNet = CSubNet(ResolveIP("1.2.3.4"), 24);
    decSubNet = CSubNet();
    decSubNet.UnSerialize(sourceRef,0,0);
    printf("BOOST_AUTO_TEST_CASE(csubnet_serialize) | After decode,subnet :%s\n",decSubNet.ToString().c_str());
    BOOST_CHECK(decSubNet.ToString() != "1.2.3.0/24");
}

BOOST_AUTO_TEST_CASE(cservice_serialize)
{
     CService encService(LookupNumeric("1.26.38.1", 1554));
     BOOST_CHECK(encService.ToString() == "1.26.38.1:1554");
     printf("BOOST_AUTO_TEST_CASE(cservice_serialize) | Before encode,service :%s\n",encService.ToString().c_str());

     DataStream stream(0,0);
     encService.Serialize(stream);
     bytes encStream =stream.stream()->out();
     bytesConstRef encStreamRef(encStream.data(),encStream.size());

     CService dncService;
     dncService.UnSerialize(encStreamRef,0,0);
     printf("BOOST_AUTO_TEST_CASE(cservice_serialize) | After decode,service  :%s\n",dncService.ToString().c_str());
     BOOST_CHECK(dncService.ToString() == "1.26.38.1:1554");
}



BOOST_AUTO_TEST_SUITE_END()


boost::unit_test::test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    return 0;
}