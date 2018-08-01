#include <stdio.h>
#include <vector>
#include <string>
#include <map>
#include <list>
#include "../jsonspirit/json_spirit.h"
#include "../jsonspirit/json_spirit_reader_template.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/filesystem.hpp>



namespace dev
{
 /*
     起个别名，方便后面用,其实源码库里有一个别名的
*/
   typedef json_spirit::Value_impl<json_spirit::Config_map<std::string> > Value_of_Map;

   typedef  boost::property_tree::ptree  ptree;

/*
    get the value of the json file
*/
    json_spirit::Value_impl<json_spirit::Config_map<std::string> > getValue(boost::filesystem::path const &filePath);
/*
    Read the json file to get the file content
    @param:filePath
    @return:std::map
 */
   std::map<std::string, std::string> readJsonFile(boost::filesystem::path const& filePath);

/*
    Read the json file to get the file content
    @param:filePath
    @param:key
    @return:std::string
 */
   std::string readStringFromJsonFile(boost::filesystem::path const& filePath, std::string key);

/*
    Read the json file to get the file content
    @param:filePath
    @param:key
    @return: std::list
 */
std::list<std::map<std::string, std::string>> readListFromJsonFile(boost::filesystem::path const& filePath, std::string key);


 boost::property_tree::ptree getContentOfIni(boost::filesystem::path const &filePath);

/*
    Read the conf file to get the file content
    @param:filePath
    @return:std::map
*/
std::map<std::string, std::string> readConfFile(boost::filesystem::path const& filePath);

/*
    Read the conf file to get the file content by key
    @param:filePath
    @param:key
    @return:std::string
 */
std::string readStringFromConfFile(boost::filesystem::path const& filePath, std::string key);

/*
    Read the conf file to get the file content by key
    @param:filePath
    @param:key
    @return: std::list
 */
std::list<std::map<std::string, std::string>> readListFromConfFile(boost::filesystem::path const& filePath, std::string sectionName);



}







