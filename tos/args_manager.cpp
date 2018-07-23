#include "ArgsManager.h"

using namespace tos;


void ArgsManager::Set(boost::program_options::variables_map& agrsMap)
{
    std::lock_guard<decltype(_vm_lock)> guard{_vm_lock};
    _vm.clear();
    _vm.insert(_vm.end(), agrsMap.begin(), args.end());
}
    

bool ArgsManager::IsArgSet(const std::string& strArg) const
{
    std::lock_guard<decltype(_vm_lock)> guard{_vm_lock};
    return _vm.count(strArg) == 0 ? false: true;
}


std::string ArgsManager::GetArg(const std::string& strArg, const std::string& strDefault) const
{
    std::lock_guard<decltype(_vm_lock)> guard{_vm_lock};
    if(_vm.count(strArg))
    {
        return _vm[strArg].as<std::string>();
    }
    return strDefault;
}

int64_t ArgsManager::GetArg(const std::string& strArg, int64_t nDefault) const
{
    std::lock_guard<decltype(_vm_lock)> guard{_vm_lock};
    if(_vm.count(strArg))
    {
        return _vm[strArg].as<int64_t>();
    }
    return nDefault;
}

bool ArgsManager::GetBoolArg(const std::string& strArg, bool fDefault) const
{
    std::lock_guard<decltype(_vm_lock)> guard{_vm_lock};
    if(_vm.count(strArg))
    {
        return _vm[strArg].as<bool>();
    }
    return fDefault;
}

