#define  BOOST_TEST_MODULE argtest
#include <boost/test/unit_test.hpp>
#include <boost/program_options.hpp>
#include <string>
#include <toscore/utils/args_manager.h>
BOOST_AUTO_TEST_SUITE(mytest)

BOOST_AUTO_TEST_CASE(test)
{
      namespace bpo=boost::program_options;

     bpo::options_description opt;
      bpo::variables_map vm;

     opt.add_options()("help", "this is a program to find a specified file");


   tos::ArgsManager argManager;
    BOOST_CHECK(argManager.IsArgSet("help"));



}


BOOST_AUTO_TEST_SUITE_END()