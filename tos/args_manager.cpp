#include "args_manager.h"
#include <boost/program_options.hpp>
using namespace tos;


void ArgsManager::Set(boost::program_options::variables_map& agrsMap)
{
    std::lock_guard<decltype(_vm_lock)> guard{_vm_lock};
    _vm.clear();
    _vm = agrsMap;
}
    

bool ArgsManager::IsArgSet(const std::string& strArg) 
{
    std::lock_guard<decltype(_vm_lock)> guard{_vm_lock};
    return _vm.count(strArg) == 0 ? false: true;
}


