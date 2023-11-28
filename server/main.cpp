#include <string>
#include <iostream>
#include "controller/Server.h"

int main() {
    // 创建服务器线程
    Server server;
    std::cout << "hello world\n";
    server.start();

    // 随意添加

    return 0;
}

