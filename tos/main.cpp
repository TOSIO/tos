#include <stdio.h>
#include <toscore/log/Log.h>
#include <toscore/log/LoggingProgramOptions.h>
#include <toscore/crypto/SHA3.h>
#include <toscore/common/Common.h>
#include <toscore/utils/RLP.h>
#include <vector>
#include <string>
#include <toscore/log/Log.h>
#include "args_manager.h"

//  #include <boost/program_options/options_description.hpp>

#include <boost/program_options.hpp>

namespace po = boost::program_options;
using dev::LoggingOptions;

void setupLog();

int main(int argc, char const *argv[])
{
    setupLog();
    cnote << "************start toschain*********";
    cerror << "error test";
    cwarn << "warning test";
    /*
    unsigned const lineWidth = 160;
    po::options_description defaultMode("TOS CLIENT MODE (default)", lineWidth);
    auto addClientOption = defaultMode.add_options();
    addClientOption("test", "Use the main network protocol");
   
    po::options_description allowedOptions("Allowed options");
    allowedOptions.add(defaultMode)
        .add(clientTransacting);

    po::variables_map vm;
    vector<string> unrecognisedOptions;

    try
    {
        po::parsed_options parsed = po::command_line_parser(argc, argv).options(allowedOptions).allow_unregistered().run();
        unrecognisedOptions = collect_unrecognized(parsed.options, po::include_positional);
        po::store(parsed, vm);
        po::notify(vm);
    }
    catch (po::error const& e)
    {
        cerr << e.what();
        return -1;
    }

    for (size_t i = 0; i < unrecognisedOptions.size(); ++i)
    {
        if (!m.interpretOption(i, unrecognisedOptions))
        {
            cerr << "Invalid argument: " << unrecognisedOptions[i] << "\n";
            return -1;
        }
    }*/

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
