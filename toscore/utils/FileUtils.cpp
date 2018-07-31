#include "FileUtils.h"
#include "../jsonspirit/json_spirit.h"
#include "../jsonspirit/json_spirit_reader_template.h"
#include <map>
#include <cstring>
#include <fstream>
/**
 *     读取 比如这样的json文件
 *     {"name":"admin","sex":"nan"}
 *     读不一样的类型的json文件会出错。错在get_obj(),get_str()上
 * */
std::map<std::string, std::string> readJsonFile(boost::filesystem::path const &filePath)
{
    typedef json_spirit::Value_impl<json_spirit::Config_map<std::string> > Value_of_Map;
    std::map<std::string, std::string> Json_To_Map;
    Value_of_Map value;
    std::string fileName = filePath.string();
    std::ifstream is(fileName);
    json_spirit::read(is, value);
    std::map<std::string, Value_of_Map>::iterator beg = value.get_obj().begin();
    for (; beg != value.get_obj().end(); beg++)
    {
        Json_To_Map.insert(std::make_pair(beg->first, beg->second.get_str()));
    }
    return Json_To_Map;
}

//调用上面的函数
std::string readStringFromJsonFile(boost::filesystem::path const &filePath, std::string key)
{
    std::map<std::string, std::string>::iterator iter = readJsonFile(filePath).find(key);
    if (iter != readJsonFile(filePath).end())
    {
        return iter->second;
    }
    return NULL;
}

/**
 *    读取 比如 这样的json文件     {"Package": { "Description": "Desription",
 *                                                               "Code":    "02",
 *                                                                 "Decription": "Description"  },
                                             "x": { "Description": "Dcription",  "Code2": "022", "Decrip2tion": "Descrip2tion"  }}
 *
 * */
std::list<std::map<std::string, std::string> > readListFromJsonFile(boost::filesystem::path const &filePath, std::string key)
{

   typedef  json_spirit::Value_impl<json_spirit::Config_map<std::string> > Value_of_Map;
    std::list<std::map<std::string, std::string> > Json_To_List;
    Value_of_Map value;
    std::string fileName = filePath.string();
    std::ifstream is(fileName);
    json_spirit::read(is, value);
    std::map<std::string, Value_of_Map>::iterator beg = value.get_obj().begin();
    for (; beg != value.get_obj().end(); beg++)
    {
        if (strcmp(beg->first.c_str(), key.c_str()) == 0)
        {

                std::map<std::string,Value_of_Map>::iterator iter= beg->second.get_obj().begin();
                 for(;iter!=beg->second.get_obj().end();iter++)
                 {
                        std::map<std::string, std::string> Json_To_Map;
                        Json_To_Map.insert(std::make_pair(iter->first,iter->second.get_str()));
                        Json_To_List.push_back(Json_To_Map);
                 }
        }
    }
    return Json_To_List;
}

/**
 *     读取 比如这样的json文件
 *     {"name":"admin","sex":"nan"}
 *
 * */
std::map<std::string, std::string> readConfFile(boost::filesystem::path const &filePath)
{
    typedef json_spirit::Value_impl<json_spirit::Config_map<std::string> > Value_of_Map;
    std::map<std::string, std::string> Json_To_Map;
    Value_of_Map value;
    std::string fileName = filePath.string();
    std::ifstream is(fileName);
    json_spirit::read(is, value);
    std::map<std::string, Value_of_Map>::iterator beg = value.get_obj().begin();
    for (; beg != value.get_obj().end(); beg++)
    {
        Json_To_Map.insert(std::make_pair(beg->first, beg->second.get_str()));
    }
    return Json_To_Map;
}

//调用上面的函数
std::string readStringFromConfFile(boost::filesystem::path const &filePath, std::string key)
{
    std::map<std::string, std::string>::iterator iter = readConfFile(filePath).find(key);
    if (iter != readConfFile(filePath).end())
    {
        return iter->second;
    }
    return NULL;
}

/**
 *    读取 比如 这样的json文件     {"Package": { "Description": "Desription",
 *                                                               "Code":    "02",
 *                                                                 "Decription": "Description"  },
                                             "x": { "Description": "Dcription",  "Code2": "022", "Decrip2tion": "Descrip2tion"  }}
 *
 * */
std::list<std::map<std::string, std::string>> readListFromConfFile(boost::filesystem::path const &filePath, std::string key)
{
     typedef  json_spirit::Value_impl<json_spirit::Config_map<std::string> > Value_of_Map;
    std::list<std::map<std::string, std::string> > Json_To_List;
    Value_of_Map value;
    std::string fileName = filePath.string();
    std::ifstream is(fileName);
    json_spirit::read(is, value);
    std::map<std::string, Value_of_Map>::iterator beg = value.get_obj().begin();
    for (; beg != value.get_obj().end(); beg++)
    {
        if (strcmp(beg->first.c_str(), key.c_str()) == 0)
        {

                std::map<std::string,Value_of_Map>::iterator iter= beg->second.get_obj().begin();
                 for(;iter!=beg->second.get_obj().end();iter++)
                 {
                        std::map<std::string, std::string> Json_To_Map;
                        Json_To_Map.insert(std::make_pair(iter->first,iter->second.get_str()));
                        Json_To_List.push_back(Json_To_Map);
                 }
        }
    }
    return Json_To_List;
}