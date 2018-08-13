#define  BOOST_TEST_MODULE argtest
#include <toscore/utils/concurrent_queue.h>
#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_SUITE(mytest)

BOOST_AUTO_TEST_CASE(test)
{
   dev::concurrent_queue<int> queue;
   queue.push<int>(1);
   int removeValue = queue.pop();
   BOOST_CHECK_EQUAL(1,removeValue);
}

BOOST_AUTO_TEST_SUITE_END()