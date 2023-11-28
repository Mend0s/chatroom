#include <iostream>
#include <string>
#include <json/json.h>

// using namespace std;
// using namespace Json;

// https://subingwen.cn/cpp/jsoncpp/
// sudo apt-get update
// sudo apt-get install libjsoncpp-dev

// g++ JsonCppServerTest.cpp -I/usr/include/jsoncpp -ljsoncpp
// g++ JsonCppClientTest.cpp -I/usr/include/jsoncpp -ljsoncpp



//  JSON --> string
int main() {
    // 假设需要传输的数据为一个简单的用户信息
    std::string username = "Alice";
    int age = 25;

    // 创建 JSON 对象，并添加字段和值
    Json::Value root;
    root["username"] = username;
    root["age"] = age;

    // 将 JSON 对象序列化为字符串
    Json::FastWriter writer;
    std::string jsonStr = writer.write(root);

    std::cout << "Serialized JSON: " << jsonStr << std::endl;

    // 在这里将 jsonStr 发送给客户端进行数据传输

    return 0;
}


