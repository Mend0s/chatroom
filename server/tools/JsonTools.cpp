#include "JsonTools.h"
#include <json/json.h>
#include <sstream>
#include <string.h>

JsonTools::JsonTools()
{
}

JsonTools::~JsonTools()
{
}
// 将 JSON 对象序列化为字符串
std::string JsonTools::jsonToString(Json::Value value){
    Json::FastWriter writer;
    std::string jsonString = writer.write(value);
    if(jsonString.empty())
        return "";
    return jsonString;
}

Json::Value JsonTools::stringToJson(std::string jsonString){
    // 解析 JSON 字符串
    Json::Value value;
    Json::CharReaderBuilder builder;
    std::istringstream jsonStream(jsonString);
    std::string parseError;
    bool parsingSuccessful = Json::parseFromStream(builder, jsonStream, &value, &parseError);
    if(parsingSuccessful){
        return value;
    }
    Json::Value value2;
    return value2;
}

