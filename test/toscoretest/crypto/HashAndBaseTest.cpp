#define BOOST_TEST_MODULE hash_test
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
using std::cout;
using namespace boost;
using namespace boost::unit_test;
using namespace dev;

  class HashAndBaseTest
  {
    public:
    void testHashWrite()
    {
      dev::Hash256 h;
      h.Write((const unsigned char*)"2",5);
    }

    void testHashFinalize()
    {
      dev::Hash256 h;
      dev::h256 hash;
      h.Finalize((unsigned char*)"5");
    }

    void testHashreset()
    {
      dev::Hash256 h;
      h.Reset();
    }

    h256 testsha256(const unsigned char * teststring)
    {
      h256 h;
      dev::bytesConstRef test=dev::bytesConstRef(teststring,16);   
      h = dev::sha256(test);
      return h;
    }

     void testToBase64()
    {
        dev::bytesConstRef btc((const unsigned char *)"1", 16);
        dev::toBase64(btc);
    }

    bytes testFromBase64(const unsigned char *teststring)
    {
        bytes bts = dev::fromBase64("qqqqqqqq");
        return bts;
    }
  };


BOOST_FIXTURE_TEST_SUITE(hashTest, TestHelperFixture)

BOOST_FIXTURE_TEST_CASE(hashTestRun, TestHelperFixture)
{
  HashAndBaseTest h;
  h.testsha256((const unsigned char*)"w");
}



BOOST_AUTO_TEST_SUITE_END()








