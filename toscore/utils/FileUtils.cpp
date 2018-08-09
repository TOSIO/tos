#include "FileUtils.h"
#include <cstring>
#include <fstream>


json_spirit::Value_impl<json_spirit::Config_map<std::string> > dev::getValue(boost::filesystem::path const &filePath)
{
    json_spirit::Value_impl<json_spirit::Config_map<std::string> > value;
    std::string fileName = filePath.string();
    std::ifstream is(fileName);
    json_spirit::read(is, value);
    return value;
}

boost::property_tree::ptree dev::getContentOfIni(boost::filesystem::path const &filePath)
 {
     std::string fileName=filePath.string();
    boost::property_tree::ptree pt;
     boost::property_tree::ini_parser::read_ini(fileName,pt);
     return pt;
 }

/**
 *     读取 比如这样的json文件
 *     {"name":"admin","sex":"nan"}
 *     读不一样的类型的json文件会出错。错在get_obj(),get_str()上
 * */
std::map<std::string, std::string> dev::readJsonFile(boost::filesystem::path const &filePath)
{
    std::map<std::string, std::string> jsonToMap;
    json_spirit::Value_impl<json_spirit::Config_map<std::string> > value;
    value=dev::getValue(filePath);
    std::map<std::string, json_spirit::Value_impl<json_spirit::Config_map<std::string> >>::iterator beg = value.get_obj().begin();
    for (; beg != value.get_obj().end(); beg++)
    {
        jsonToMap.insert(std::make_pair(beg->first, beg->second.get_str()));
    }
    return jsonToMap;
}

//调用上面的函数
std::string dev::readStringFromJsonFile(boost::filesystem::path const &filePath, std::string key)
{
    std::map<std::string, std::string>::iterator iter = dev::readJsonFile(filePath).find(key);
    if (iter != dev::readJsonFile(filePath).end())
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
std::list<std::map<std::string, std::string> > dev::readListFromJsonFile(boost::filesystem::path const &filePath, std::string key)
{

    std::list<std::map<std::string, std::string> > jsonToList;
    json_spirit::Value_impl<json_spirit::Config_map<std::string> > value;
   value=dev::getValue(filePath);
    std::map<std::string, json_spirit::Value_impl<json_spirit::Config_map<std::string> >>::iterator beg = value.get_obj().begin();
    for (; beg != value.get_obj().end(); beg++)
    {
        if (strcmp(beg->first.c_str(), key.c_str()) == 0)
        {

                std::map<std::string,json_spirit::Value_impl<json_spirit::Config_map<std::string> >>::iterator iter= beg->second.get_obj().begin();
                 for(;iter!=beg->second.get_obj().end();iter++)
                 {
                        std::map<std::string, std::string> jsonToMap;
                        jsonToMap.insert(std::make_pair(iter->first,iter->second.get_str()));
                        jsonToList.push_back(jsonToMap);
                 }
        }
    }
    return jsonToList;
}

/**
 *     ini:
 *     [Section1]
       Value1 = 10
        Value2 = a_text_string
 *
 *       section.first=Section1
 *
 * */
std::map<std::string, std::string> dev::readConfFile(boost::filesystem::path const &filePath)
{
    std::map<std::string, std::string> mapOfConf;

   boost::property_tree::ptree pt=dev::getContentOfIni(filePath);

    for(auto& section :pt)
    {
        for(auto &key:section.second)
        {
             mapOfConf.insert(std::make_pair(key.first,key.second.get_value<std::string>()));
        }
    }
    return mapOfConf;
}

//调用上面的函数
std::string dev::readStringFromConfFile(boost::filesystem::path const &filePath, std::string key)
{
    std::map<std::string, std::string>::iterator iter = dev::readConfFile(filePath).find(key);
    if (iter != dev::readConfFile(filePath).end())
    {
        return iter->second;
    }
    return NULL;
}

/**
         ini:
 *     [Section1]
       Value1 = 10
        Value2 = a_text_string
 *
 *       section.first=Section1
 * */
std::list<std::map<std::string, std::string>> dev::readListFromConfFile(boost::filesystem::path const &filePath, std::string sectionName)
{
     std::map<std::string, std::string> mapOfConf;
    std::list<std::map<std::string, std::string>> listOfMapValue;
   boost::property_tree::ptree pt=dev::getContentOfIni(filePath);
    for(auto& section :pt)
    {
        if(strcmp(section.first.c_str(),sectionName.c_str())==0)
        {
            for(auto &key:section.second)
           {
             mapOfConf.insert(std::make_pair(key.first,key.second.get_value<std::string>()));
           }
        }

    }
    listOfMapValue.push_back(mapOfConf);
    return listOfMapValue;
}