#include <stdio.h>
#include <stdlib.h>
#include "TosTestUtils.h"
#include <boost/filesystem.hpp>

using namespace std;
using namespace dev;
using namespace dev::test;
namespace fs = boost::filesystem;


boost::filesystem::path dev::test::getTestPath()
{
    if (!Options::get().testpath.empty())
        return Options::get().testpath;

    string testPath;
    const char* ptestPath = getenv("ETHEREUM_TEST_PATH");

    if (ptestPath == nullptr)
    {
        clog(VerbosityWarning, "test")
            << " could not find environment variable ETHEREUM_TEST_PATH \n";
        testPath = "../../test/jsontests";
    }
    else
        testPath = ptestPath;

    return boost::filesystem::path(testPath);
}
