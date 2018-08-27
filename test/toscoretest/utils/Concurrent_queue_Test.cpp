
#include <toscore/utils/concurrent_queue.h>
#include <boost/test/unit_test.hpp>



BOOST_AUTO_TEST_CASE(queuetest)
{
   dev::concurrent_queue<int> queue;
   queue.push<int>(1);
   int removeValue = queue.pop();
   BOOST_CHECK_EQUAL(1,removeValue);
}
