#include "argsproxy.h"

using namespace tos;

/**
 * Return a vector of strings of the given argument
 *
 * @param strArg Argument to get (e.g. "-foo")
 * @return command-line arguments
 */
std::vector<std::string> ArgsProxy::GetArgs(const std::string& strArg)
{
    std::vector<std::string> result;
    return result;
}
/**
 * Return true if the given argument has been manually set
 *
 * @param strArg Argument to get (e.g. "-foo")
 * @return true if the argument has been set
 */
bool ArgsProxy::IsArgSet(const std::string& strArg)
{
    return true;
}

/**
 * Return string argument or default value
 *
 * @param strArg Argument to get (e.g. "-foo")
 * @param strDefault (e.g. "1")
 * @return command-line argument or default value
 */
std::string ArgsProxy::GetArg(const std::string& strArg, const std::string& strDefault)
{
    return std::string();
}
/**
 * Return integer argument or default value
 *
 * @param strArg Argument to get (e.g. "-foo")
 * @param nDefault (e.g. 1)
 * @return command-line argument (0 if invalid number) or default value
 */
int64_t ArgsProxy::GetArg(const std::string& strArg, int64_t nDefault)
{
    return 0;
}

/**
 * Return boolean argument or default value
 *
 * @param strArg Argument to get (e.g. "-foo")
 * @param fDefault (true or false)
 * @return command-line argument or default value
 */
bool ArgsProxy::GetBoolArg(const std::string& strArg, bool fDefault)
{
    return false;
}
/**
 * Set an argument if it doesn't already have a value
 *
 * @param strArg Argument to set (e.g. "-foo")
 * @param strValue Value (e.g. "1")
 * @return true if argument gets set, false if it already had a value
 */
bool ArgsProxy::SoftSetArg(const std::string& strArg, const std::string& strValue)
{
    return false;
}

/**
 * Set a boolean argument if it doesn't already have a value
 *
 * @param strArg Argument to set (e.g. "-foo")
 * @param fValue Value (e.g. false)
 * @return true if argument gets set, false if it already had a value
 */
bool ArgsProxy::SoftSetBoolArg(const std::string& strArg, bool fValue)
{
    return false;
}

// Forces an arg setting. Called by SoftSetArg() if the arg hasn't already
// been set. Also called directly in testing.
void ForceSetArg(const std::string& strArg, const std::string& strValue)
{
    
}