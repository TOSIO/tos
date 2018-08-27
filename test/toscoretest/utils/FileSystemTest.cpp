#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <toscore/utils/FileSystem.h>

class Test
{
 public:
 boost::filesystem::path getPath()
      {
         return dev::getDataDir();
       }
};


BOOST_AUTO_TEST_CASE(sdas)
{
     Test t;
     BOOST_CHECK(!t.getPath().string().empty());
}
