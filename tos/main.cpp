#include <stdio.h>
#include <toscore/log/Log.h>
#include <toscore/log/LoggingProgramOptions.h>
#include <toscore/crypto/SHA3.h>
#include <toscore/common/Common.h>
#include <toscore/utils/RLP.h>
#include <vector>
#include <string>
#include <toscore/log/Log.h>
//  #include <boost/program_options/options_description.hpp>

#include <boost/program_options.hpp>
using std::cout;
namespace po = boost::program_options;
using dev::LoggingOptions;
using dev::RLP;

void setupLog();

int main(int argc, char const *argv[])
{
    setupLog();
    cnote << "************start toschain*********";
    cerror << "error test";
    cwarn << "warning test";
    return 0;
}

void setupLog()
{
    int c_lineWidth = 160;
    LoggingOptions loggingOptions;
    po::options_description loggingProgramOptions(
        dev::createLoggingProgramOptions(c_lineWidth, loggingOptions));
    dev::setupLogging(loggingOptions);
}
