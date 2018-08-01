#include "FileUtils.h"
#include <cstring>
#include <fstream>


json_spirit::Value_impl<json_spirit::Config_map<std::string> > getValue(boost::filesystem::path const &filePath)
{
    dev::Value_of_Map value;
    std::string FileName = filePath.string();
    std::ifstream is(FileName);
    json_spirit::read(is, value);
    return value;
}

 dev::ptree getContentOfIni(boost::filesystem::path const &filePath)
 {
     std::string FileName=filePath.string();
     dev::ptree pt;
     boost::property_tree::ini_parser::read_ini(FileName,pt);
     return pt;
 }

/**
 *     读取 比如这样的json文件
 *     {"name":"admin","sex":"nan"}
 *     读不一样的类型的json文件会出错。错在get_obj(),get_str()上
 * */
std::map<std::string, std::string> readJsonFile(boost::filesystem::path const &filePath)
{
    std::map<std::string, std::string> Json_To_Map;
    dev::Value_of_Map value;
    value=getValue(filePath);
    std::map<std::string, dev::Value_of_Map>::iterator beg = value.get_obj().begin();
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

    std::list<std::map<std::string, std::string> > Json_To_List;
    dev::Value_of_Map value;
   value=getValue(filePath);
    std::map<std::string, dev::Value_of_Map>::iterator beg = value.get_obj().begin();
    for (; beg != value.get_obj().end(); beg++)
    {
        if (strcmp(beg->first.c_str(), key.c_str()) == 0)
        {

                std::map<std::string,dev::Value_of_Map>::iterator iter= beg->second.get_obj().begin();
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
 *     ini:
 *     [Section1]
       Value1 = 10
        Value2 = a_text_string
 *
 *       section.first=Section1
 *
 * */
std::map<std::string, std::string> readConfFile(boost::filesystem::path const &filePath)
{
    std::map<std::string, std::string> Map_of_Conf;

    dev::ptree pt=getContentOfIni(filePath);

    for(auto& section :pt)
    {
        for(auto &key:section.second)
        {
             Map_of_Conf.insert(std::make_pair(key.first,key.second.get_value<std::string>()));
        }
    }
    return Map_of_Conf;
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
         ini:
 *     [Section1]
       Value1 = 10
        Value2 = a_text_string
 *
 *       section.first=Section1
 * */
std::list<std::map<std::string, std::string>> readListFromConfFile(boost::filesystem::path const &filePath, std::string sectionName)
{
    std::map<std::string, std::string> Map_of_Conf;
    std::list<std::map<std::string, std::string>> list_of_MapValue;
    dev::ptree pt=getContentOfIni(filePath);
    for(auto& section :pt)
    {
        if(strcmp(section.first.c_str(),sectionName.c_str())==0)
        {
            for(auto &key:section.second)
           {
             Map_of_Conf.insert(std::make_pair(key.first,key.second.get_value<std::string>()));
           }
        }

    }
    list_of_MapValue.push_back(Map_of_Conf);
    return list_of_MapValue;
}