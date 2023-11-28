#ifndef __JSONTOOLS_H
#define __JSONTOOLS_H

#include <string>
#include <json/json.h>

class JsonTools
{

public:
    JsonTools();
    ~JsonTools();

    /**
     * @brief 将 JSON 对象序列化为字符串
     *
     * 将 JSON 对象序列化为字符串
     *
     * @param value Json::Value
     *
     * @return std::string  如果将 JSON 对象序列化为字符串成功，则返回转换后的std::string,
     *                      否则返回一个空的std::string
     * @author 侯顺智
     */
    std::string jsonToString(Json::Value value);


    // 
    /**
     * @brief 将 字符串 序列化为JSON 对象
     *
     * 将 字符串 序列化为JSON 对象
     *
     * @param jsonString std::string 
     *
     * @return Json::Value  如果将 字符串 序列化为JSON 对象成功，则返回转换后的JSON 对象
     *                      否则返回一个空的Json::Value
     * @author 侯顺智
     */
    Json::Value stringToJson(std::string jsonString);
};



#endif