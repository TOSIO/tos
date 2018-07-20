#include "ArgsManager.h"

using namespace tos;


ArgsManager::ArgsManager(boost::program_options::variables_map& vm):_vm(vm)
{

}


std::vector<std::string> ArgsManager::GetArgs(const std::string& strArg) const
{
    
}


bool ArgsManager::IsArgSet(const std::string& strArg) const
{
    return _vm.count() == 0 ? false: true;
}


std::string ArgsManager::GetArg(const std::string& strArg, const std::string& strDefault) const
{
    
}

int64_t ArgsManager::GetArg(const std::string& strArg, int64_t nDefault) const
{

}

bool ArgsManager::GetBoolArg(const std::string& strArg, bool fDefault) const
{

}

