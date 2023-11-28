#include "../tools/JsonTools.h"
#include <iostream>
#include <string>
#include <json/json.h>

// g++ JsonToolsTest.cpp ../tools/JsonTools.cpp -I/usr/include/jsoncpp -ljsoncpp 

int main(){

    JsonTools jsonTools;

    Json::Value people;
    people["username"] = "张三";
    people["age"] = 20;

    // 测试 jsonToString
    std::string jsonString = jsonTools.jsonToString(people);
    std::cout << "jsonToString: jsonString = " << jsonString << std::endl;
    
    // 测试 stringToJson
    Json::Value people2 = jsonTools.stringToJson(jsonString);
    std::cout << "Received JSON:" << std::endl;
    std::cout << "Username: " << people2["username"].asString() << std::endl;
    std::cout << "Age: " << people2["age"].asInt() << std::endl;

    return 0;
}