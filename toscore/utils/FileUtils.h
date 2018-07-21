#include <stdio.h>
#include <vector>
#include <string>
#include <map>
#include <list>
#include <boost/filesystem.hpp>



namespace dev
{


std::map<std::string, std::string> readJson(boost::filesystem::path const& _dataDir);
std::string readStringFromJson(boost::filesystem::path const& _dataDir, std::string key);
std::list<map<std::string, std::string>> readListFromJson(boost::filesystem::path const& _dataDir, std::string key);


std::map<std::string, std::string> readConf(boost::filesystem::path const& _dataDir);
std::string readStringFromConf(boost::filesystem::path const& _dataDir, std::string key);
std::list<> readListFromConf(boost::filesystem::path const& _dataDir, std::string key);
}







