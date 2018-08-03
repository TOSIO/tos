// Copyright (c) 2012-2017 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#include <addrman.h>
#include "test_tos.h"
#include <string>
#include <boost/test/unit_test.hpp>
#include <hash.h>
//#include <serialize.h>
#include <streams.h>
#include <net.h>
#include <netbase.h>
//#include <chainparams.h>
#include <util.h>
#include <deps/clientversion.h>
#include <toscore/crypto/Hash.h>

using namespace tos::P2P;

class CAddrManSerializationMock : public CAddrMan
{
public:
    virtual void Serialize(DataStream& s) = 0;

    //! Ensure that bucket placement is always the same for testing purposes.
    void MakeDeterministic()
    {
        nKey.SetNull();
        insecure_rand = FastRandomContext(true);
    }
};

class CAddrManUncorrupted : public CAddrManSerializationMock
{
public:
    void Serialize(DataStream& s) override
    {
        CAddrMan::Serialize(s);
    }
};

class CAddrManCorrupted : public CAddrManSerializationMock
{
public:
/*     void Serialize(CDataStream& s) const override
    {
        // Produces corrupt output that claims addrman has 20 addrs when it only has one addr.
        unsigned char nVersion = 1;
        s << nVersion;
        s << ((unsigned char)32);
        s << nKey;
        s << 10; // nNew
        s << 10; // nTried

        int nUBuckets = ADDRMAN_NEW_BUCKET_COUNT ^ (1 << 30);
        s << nUBuckets;

        CService serv;
        Lookup("252.1.1.1", serv, 7777, false);
        CAddress addr = CAddress(serv, NODE_NONE);
        CNetAddr resolved;
        LookupHost("252.2.2.2", resolved, false);
        CAddrInfo info = CAddrInfo(addr, resolved);
        s << info;
    } */

    void Serialize(DataStream& out) override
    {
        printf("Trace | void CAddrManCorrupted::Serialize(DataStream& out) override \n");
        unsigned char nVersion = 1;
        out.stream()->appendList(7);
        out.stream()->append(nVersion);
        out.stream()->append(32);
        //out.stream()->append(u256(nKey.ToString()));
        vector_ref<uint8_t> keyRef(nKey.begin(),nKey.size());
        out.stream()->append(keyRef);
        out.stream()->append(10);
        out.stream()->append(10);

        int nUBuckets = ADDRMAN_NEW_BUCKET_COUNT ^ (1 << 30);
        //s << nUBuckets;
        out.stream()->append(nUBuckets);

        CService serv;
        Lookup("252.1.1.1", serv, 7777, false);
        CAddress addr = CAddress(serv, NODE_NONE);
        CNetAddr resolved;
        LookupHost("252.2.2.2", resolved, false);
        CAddrInfo info = CAddrInfo(addr, resolved);
        //s << info;
        out.stream()->appendList(1);
        info.Serialize(out);
    }

};

DataStream AddrmanToStream(CAddrManSerializationMock& _addrman)
{
    DataStream ssPeersIn(SER_DISK, CLIENT_VERSION);

    DataStream rlpstream(SER_DISK, CLIENT_VERSION);
    
    vector_ref<const unsigned char> msgStart(Params().MessageStart(),4);
    rlpstream.stream()->appendList(2);
    rlpstream.stream()->append(msgStart);
    _addrman.Serialize(rlpstream);

    bytes out =  rlpstream.stream()->out();

    h256 hash = dev::hash(&out);

    ssPeersIn.stream()->appendList(2);
    ssPeersIn.stream()->append(out);
    ssPeersIn.stream()->append(hash);

    //ssPeersIn.stream()->append();

/*     std::string str = ssPeersIn.str();
    std::vector<unsigned char> vchData(str.begin(), str.end());
    return CDataStream(vchData, SER_DISK, CLIENT_VERSION); * */
    return ssPeersIn;
}

//BOOST_FIXTURE_TEST_SUITE(net_tests, BasicTestingSetup)
//BOOST_AUTO_TEST_SUITE(net_test)

BOOST_AUTO_TEST_CASE(cnode_listen_port)
{
    // test default
/*     unsigned short port = GetListenPort();
    BOOST_CHECK(port == Params().GetDefaultPort());
    // test set port
    unsigned short altPort = 12345;
    Args().SoftSetArg("-port", std::to_string(altPort));
    port = GetListenPort();
    BOOST_CHECK(port == altPort); */
}

BOOST_AUTO_TEST_CASE(caddrdb_read)
{
    printf("Trace | BOOST_AUTO_TEST_CASE(caddrdb_read) begin execute net test case.\n");
    CAddrManUncorrupted addrmanUncorrupted;
    addrmanUncorrupted.MakeDeterministic();

    CService addr1, addr2, addr3;
    Lookup("250.7.1.1", addr1, 8333, false);
    Lookup("250.7.2.2", addr2, 9999, false);
    Lookup("250.7.3.3", addr3, 9999, false);

    // Add three addresses to new table.
    CService source;
    Lookup("252.5.1.1", source, 8333, false);
    addrmanUncorrupted.Add(CAddress(addr1, NODE_NONE), source);
    addrmanUncorrupted.Add(CAddress(addr2, NODE_NONE), source);
    addrmanUncorrupted.Add(CAddress(addr3, NODE_NONE), source);

    // Test that the de-serialization does not throw an exception.
    DataStream ssPeers1 = AddrmanToStream(addrmanUncorrupted);
    bool exceptionThrown = false;
    CAddrMan addrman1;

    BOOST_CHECK(addrman1.size() == 0);
    try {
         //unsigned char pchMsgTmp[4];
        //ssPeers1 >> FLATDATA(pchMsgTmp); 
        //ssPeers1 >> addrman1;
       // vector_ref<unsigned char> msgRef(pchMsgTmp,4);

        bytes stream = ssPeers1.stream()->out();
        RLP rlp(stream);
        bytes content = rlp[0].toBytes();
        RLP rlpContent(content);
        bytes msgStart = rlpContent[0].toBytes();
        
        //rlp[0].toVectorRef<unsigned char>(msgRef);
        bytesConstRef addrStream = rlpContent[1].data();
        addrman1.UnSerialize(addrStream,ssPeers1.getType(),ssPeers1.getVersion());
    } catch (const std::exception& e) {
        printf("catch exception :%s\n",e.what());
        exceptionThrown = true;
    }

    BOOST_CHECK(addrman1.size() == 3);
    BOOST_CHECK(exceptionThrown == false);

    // Test that CAddrDB::Read creates an addrman with the correct number of addrs.
    DataStream ssPeers2 = AddrmanToStream(addrmanUncorrupted);

    CAddrMan addrman2;
    CAddrDB adb;
    BOOST_CHECK(addrman2.size() == 0);
    adb.Read(addrman2, ssPeers2);
    BOOST_CHECK(addrman2.size() == 3);
}


BOOST_AUTO_TEST_CASE(caddrdb_read_corrupted)
{
    CAddrManCorrupted addrmanCorrupted;
    addrmanCorrupted.MakeDeterministic();

    // Test that the de-serialization of corrupted addrman throws an exception.
    DataStream ssPeers1 = AddrmanToStream(addrmanCorrupted);
    bool exceptionThrown = false;
    CAddrMan addrman1;
    BOOST_CHECK(addrman1.size() == 0);
    try {
/*         vector_ref<const unsigned char> msgStart(Params().MessageStart(),4);
        ssPeers1.stream()->append(msgStart); */
        //addrman1.Serialize(ssPeers1);
   /*      bytes stream = ssPeers1.stream()->out();
        RLP rlp(stream);
        bytesConstRef addrStream = rlp[0][1].data();
 */
         bytes stream = ssPeers1.stream()->out();
        RLP rlp(stream);
        bytes content = rlp[0].toBytes();
        RLP rlpContent(content);
        bytes msgStart = rlpContent[0].toBytes();
        
        //rlp[0].toVectorRef<unsigned char>(msgRef);
        bytesConstRef addrStream = rlpContent[1].data();

        addrman1.UnSerialize(addrStream,ssPeers1.getType(),ssPeers1.getVersion());

    } catch (const std::exception& e) {
        exceptionThrown = true;
    }
    // Even through de-serialization failed addrman is not left in a clean state.
    BOOST_CHECK(addrman1.size() == 1);
    BOOST_CHECK(exceptionThrown);

    // Test that CAddrDB::Read leaves addrman in a clean state if de-serialization fails.
    DataStream ssPeers2 = AddrmanToStream(addrmanCorrupted);

    CAddrMan addrman2;
    CAddrDB adb;
    BOOST_CHECK(addrman2.size() == 0);
    adb.Read(addrman2, ssPeers2);
    BOOST_CHECK(addrman2.size() == 0);
}

BOOST_AUTO_TEST_CASE(cnode_simple_test)
{
    SOCKET hSocket = INVALID_SOCKET;
    NodeId id = 0;
    int height = 0;

    in_addr ipv4Addr;
    ipv4Addr.s_addr = 0xa0b0c001;
    
    CAddress addr = CAddress(CService(ipv4Addr, 7777), NODE_NETWORK);
    std::string pszDest = "";
    bool fInboundIn = false;

    // Test that fFeeler is false by default.
    std::unique_ptr<CNode> pnode1(new CNode(id++, NODE_NETWORK, height, hSocket, addr, 0, 0, CAddress(), pszDest, fInboundIn));
    BOOST_CHECK(pnode1->fInbound == false);
    BOOST_CHECK(pnode1->fFeeler == false);

    fInboundIn = true;
    std::unique_ptr<CNode> pnode2(new CNode(id++, NODE_NETWORK, height, hSocket, addr, 1, 1, CAddress(), pszDest, fInboundIn));
    BOOST_CHECK(pnode2->fInbound == true);
    BOOST_CHECK(pnode2->fFeeler == false);
}

//BOOST_AUTO_TEST_SUITE_END()
/* BOOST_AUTO_TEST_SUITE_END()


boost::unit_test::test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    return 0;
} */