#include <chainparams_proxy.h>
#include <argsproxy.h>

using namespace tos;

const std::vector<std::string>& ChainParamsProxy::DNSSeeds() const
{
    static std::vector<std::string> seeds = g_argsProxy->GetArgs("seeds");
    return seeds;
}
