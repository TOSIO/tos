#pragma once
#include <boost/program_options.hpp>
#include <utility>
#include <mutex>

namespace tos
{
class ArgsManager
{
  public:
    ArgsManager(){};

    void Set(boost::program_options::variables_map &agrsMap);

    /**
     * Return true if the given argument has been manually set
     *
     * @param strArg Argument to get (e.g. "-foo")
     * @return true if the argument has been set
     */
    bool IsArgSet(const std::string &strArg);

    template <class T>
    T GetArg(const std::string& strArg, const T& default_t)
    {
        std::lock_guard<decltype(_vm_lock)> guard{_vm_lock};
        if(_vm.count(strArg))
        {
            return _vm[strArg].as<T>();
        }
        return default_t;
    }
   

    private:
    boost::program_options::variables_map _vm;
    std::mutex _vm_lock;
};

}

extern tos::ArgsManager g_args;