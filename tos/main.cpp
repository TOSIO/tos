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
//template<typename T>
int main(int argc, char const *argv[])
{
int c_lineWidth = 160;
LoggingOptions loggingOptions;
    po::options_description loggingProgramOptions(
        dev::createLoggingProgramOptions(c_lineWidth, loggingOptions));
dev::setupLogging(loggingOptions);
cnote<<"sfs123232" << "***8***";
 clog(dev::VerbosityError, "error")<<"twststs3213423sdfasdf";

    /* code */
    printf("asdfasdf\n");
    //BOOST_LOG_INLINE_GLOBAL_LOGGER_CTOR_ARGS('dsfdfsd', 12);
    const std::string str = "13123weriouweoruweo";

    cout << str << std::endl;

    std::vector<unsigned char> valList = {'0', '1', '3', '4'};
    // std::string str1;// = dev::toBase64(valList);
    // cout<<str1<< std::endl;

    dev::bytesRef vec(valList.data(), valList.size());
    dev::RLP rlp(vec, dev::RLP::AllowNonCanon);
    // rlp::isInt();
    cout << valList.data() << std::endl;
    cout << rlp.sizeStrict() << std::endl;

    return 0;
}
