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

tos::ArgsManager g_args;
unsigned const _lineWidth = 160;
void setupLog();

int main(int argc, char const *argv[])
{
    //init log
    setupLog();

    cnote << "************start toschain*********";
    po::options_description defaultMode("TOS CLIENT MODE (default)", _lineWidth);
    auto addClientOption = defaultMode.add_options();
    addClientOption("test", "Use the main network protocol");
   
    po::options_description clientNetworking("CLIENT NETWORKING", _lineWidth);
    auto addNetworkingOption = clientNetworking.add_options();
    addNetworkingOption("bootstrap,b",
        "Connect to the default Tos peer servers (default unless --no-discovery used)");

    addNetworkingOption("no-bootstrap",
        "Do not connect to the default Tos peer servers (default only when --no-discovery is "
        "used)");

    addNetworkingOption("listen-ip", po::value<std::string>()->value_name("<ip>(:<port>)"),
        "Listen on the given IP for incoming connections (default: 0.0.0.0)");

    addNetworkingOption("listen", po::value<unsigned short>()->value_name("<port>"),
        "Listen on the given port for incoming connections (default: 80909)");

    addNetworkingOption("remote,r", po::value<std::string>()->value_name("<host>(:<port>)"),
        "Connect to the given remote host (default: none)");

    addNetworkingOption("port", po::value<short>()->value_name("<port>"),
        "Connect to the given remote port (default: 30303)");


    po::options_description allowedOptions("Allowed options");
    allowedOptions.add(defaultMode)
    .add(clientNetworking);
    
    po::variables_map vm;
    std::vector<std::string> unrecognisedOptions;

    try
    {
        po::parsed_options parsed = po::command_line_parser(argc, argv).options(allowedOptions).allow_unregistered().run();
        unrecognisedOptions = collect_unrecognized(parsed.options, po::include_positional);
        po::store(parsed, vm);
        po::notify(vm);
    }
    catch (po::error const& e)
    {
        std::cerr << e.what();
        return -1;
    }

    g_args.Set(vm);
    return 0;
}

/*
init log
*/
void setupLog()
{
    LoggingOptions loggingOptions;
    po::options_description loggingProgramOptions(
        dev::createLoggingProgramOptions(_lineWidth, loggingOptions));
    dev::setupLogging(loggingOptions);
}


