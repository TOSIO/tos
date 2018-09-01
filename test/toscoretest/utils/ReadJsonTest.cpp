#define BOOST_TEST_MODULE ReadTEST
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
#include <toscore/utils/FileUtils.h>
using std::cout;
using namespace boost;
using namespace boost::unit_test;
using namespace dev;

  class HashAndBaseTest
  {
    public:
    void testReadJsonFile()
    {
        boost::filesystem::path  filePath("demo.json");

       std::map<std::string, std::string> m=readJsonFile(filePath);
        std::map<std::string, std::string>::iterator iter=m.begin();
        for(;iter!=m.end();iter++)
       {
           cout<<iter->first<<"  "<<iter->second<<std::endl;
       }
    }

    void test_readStringFromJsonFile()
    {
       boost::filesystem::path  filePath("demo.json");

     cout<<dev::readStringFromJsonFile(filePath,"Code");
    }

    void test_readListFromJsonFile()
    {
      boost::filesystem::path  filePath("test.json");
      std::list<std::map<std::string, std::string> > li=dev::readListFromJsonFile(filePath,"Package");
      std::list<std::map<std::string, std::string> >::iterator iter=li.begin();
       for(;iter!=li.end();iter++)
       {
          cout<<iter->begin()->first<<" "<<iter->begin()->second<<std::endl;
       }
    }

    void test_readConfFile(const unsigned char * teststring)
    {
         boost::filesystem::path  filePath("demo.json");
       std::map<std::string, std::string> m=dev::readJsonFile(filePath);
        std::map<std::string, std::string>::iterator iter=m.begin();
        for(;iter!=m.end();iter++)
       {
           cout<<iter->first<<"  "<<iter->second<<std::endl;
       }
    }

     void test_readStringFromConfFile()
    {
        boost::filesystem::path  filePath("demo.json");

     cout<<dev::readStringFromJsonFile(filePath,"Code");
    }

    void test_readListFromConfFile(const unsigned char *teststring)
    {
         boost::filesystem::path  filePath("test.json");
      std::list<std::map<std::string, std::string> > li=dev::readListFromJsonFile(filePath,"Package");
      std::list<std::map<std::string, std::string> >::iterator iter=li.begin();
       for(;iter!=li.end();iter++)
       {
          cout<<iter->begin()->first<<" "<<iter->begin()->second<<std::endl;
       }
    }
  };


BOOST_FIXTURE_TEST_SUITE(DASDEA, TestHelperFixture)

BOOST_FIXTURE_TEST_CASE(hashTestRun, TestHelperFixture)
{
  HashAndBaseTest h;
  h.testReadJsonFile();
}



BOOST_AUTO_TEST_SUITE_END()








