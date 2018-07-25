#include "argsproxy.h"
#include "../../tos/args_manager.h"

using namespace tos;


ArgsProxy::ArgsProxy(ArgsManager* manager):_args_manager(manager)
{

}

/**
 * Return true if the given argument has been manually set
 *
 * @param strArg Argument to get (e.g. "-foo")
 * @return true if the argument has been set
 */
bool ArgsProxy::IsArgSet(const std::string& strArg) const
{
    return _args_manager->IsArgSet(strArg);
}

/**
 * Return string argument or default value
 *
 * @param strArg Argument to get (e.g. "-foo")
 * @param strDefault (e.g. "1")
 * @return command-line argument or default value
 */
std::string ArgsProxy::GetArg(const std::string& strArg, const std::string& strDefault) const
{
    return _args_manager->GetArg(strArg,strDefault);
}
/**
 * Return integer argument or default value
 *
 * @param strArg Argument to get (e.g. "-foo")
 * @param nDefault (e.g. 1)
 * @return command-line argument (0 if invalid number) or default value
 */
int64_t ArgsProxy::GetArg(const std::string& strArg, int64_t nDefault) const
{
    return _args_manager->GetArg(strArg,nDefault);
}

/**
 * Return boolean argument or default value
 *
 * @param strArg Argument to get (e.g. "-foo")
 * @param fDefault (true or false)
 * @return command-line argument or default value
 */
bool ArgsProxy::GetBoolArg(const std::string& strArg, bool fDefault) const
{
    return _args_manager->GetArg<bool>(strArg,fDefault);
}
