#include <iostream>
#include <string>
#include <sstream>  
#include <json/json.h>

using namespace std;
using namespace Json;

//  string --> JSON
int main() {
    // 假设从服务端接收到的 JSON 字符串
    std::string receivedJsonStr = "{\"username\":\"Alice\",\"age\":25}";

    // 解析 JSON 字符串
    Json::Value root;
    Json::CharReaderBuilder builder;
    std::istringstream jsonStream(receivedJsonStr);
    std::string parseError;
    bool parsingSuccessful = Json::parseFromStream(builder, jsonStream, &root, &parseError);

    if (parsingSuccessful) {
        // 获取字段值
        std::string username = root["username"].asString();
        int age = root["age"].asInt();

        // 打印解析后的值
        std::cout << "Received JSON:" << std::endl;
        std::cout << "Username: " << username << std::endl;
        std::cout << "Age: " << age << std::endl;
    }
    else {
        std::cout << "Failed to parse JSON: " << parseError << std::endl;
    }

    return 0;
}
