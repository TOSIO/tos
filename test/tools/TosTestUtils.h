#pragma once

#include <string>
#include <json/json.h>
#include <toscore/log/Log.h>
#include <toscore/log/Log.h>

#include <boost/filesystem.hpp>
#include <toscore/log/LoggingProgramOptions.h>
#include <boost/program_options.hpp>
namespace po = boost::program_options;
using dev::LoggingOptions;


namespace dev
{
namespace test
{

boost::filesystem::path getTestPath();


}

}


class TestHelperFixture
{


// void setupLog();

public:
	TestHelperFixture() { 
        setupLog();
        // cnote<<"TestHelperFixture init"; 
    }
	~TestHelperFixture() { 
        // cnote<<"TestHelperFixture finish";
    }





void setupLog()
{
    int c_lineWidth = 160;
    LoggingOptions loggingOptions;
    po::options_description loggingProgramOptions(
        dev::createLoggingProgramOptions(c_lineWidth, loggingOptions));
    dev::setupLogging(loggingOptions);
}



};


