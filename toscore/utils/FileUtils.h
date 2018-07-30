#include <stdio.h>
#include <vector>
#include <string>
#include <map>
#include <list>
#include <boost/filesystem.hpp>



namespace dev
{

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
std::list<std::map<std::string, std::string>> readListFromConfFile(boost::filesystem::path const& filePath, std::string key);


}







