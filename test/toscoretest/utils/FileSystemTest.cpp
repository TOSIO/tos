#define  BOOST_TEST_MODULE test
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <toscore/utils/FileSystem.h>

BOOST_AUTO_TEST_SUITE(test)
class Test
{
 public:
 boost::filesystem::path getPath()
      {
         return dev::getDataDir();
       }
};


BOOST_FIXTURE_TEST_CASE(mytest,Test)
{
     Test t;
     BOOST_CHECK(!t.getPath().string().empty());
}

BOOST_AUTO_TEST_SUITE_END()